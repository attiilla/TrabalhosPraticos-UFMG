from concurrent import futures
import grpc, sys, threading, socket
import client_server_pb2, client_server_pb2_grpc
import server_server_pb2, server_server_pb2_grpc

class Operations(client_server_pb2_grpc.OperationsServicer):
    data_base = dict()

    def __init__(self, stop_event, port, flag=False):
        self._stop_event = stop_event
        self.port = port
        self.flag = flag

    def insert(self, request, context):
        key = request.key_numb
        msg = request.msg_text
        if key in Operations.data_base:
            return client_server_pb2.Ret_val(ret = -1)
        else:
            Operations.data_base[key] = msg
            return client_server_pb2.Ret_val(ret = 0)

    def check(self, request, context):
        print("checando")
        key = request.k
        print(f"peguei a chave {key}")
        if key in Operations.data_base:
            print("ela está no servidor")
            return client_server_pb2.Ret_text(ret = Operations.data_base[key])
        else:
            print("ela não está aqui")
            return client_server_pb2.Ret_text(ret = "")

    def terminate(self, request, context):
        self._stop_event.set()
        return client_server_pb2.Ret_val(ret = 0)

    def activate(self, request, context):
        if self.flag:
            name = socket.getfqdn()
            self_addrss = name + ":"+str(self.port)
            tgt_addrss = request.s
            channel = grpc.insecure_channel(tgt_addrss, options=[('grpc.connect_timeout_ms', 500000)])
            stub = server_server_pb2_grpc.CentralizedOperationsStub(channel)
            response = stub.register(server_server_pb2.Text_keys(s = self_addrss,k =list(Operations.data_base.keys())))
            channel.close()
            return client_server_pb2.Ret_val(ret = response.ret)
        else:
            return client_server_pb2.Ret_val(ret = 0)
            



def serve():
    port = sys.argv[1]
    stop_event = threading.Event()
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    if len(sys.argv)<3:
        client_server_pb2_grpc.add_OperationsServicer_to_server(Operations(stop_event,port),server)
    elif len(sys.argv)==3:
        client_server_pb2_grpc.add_OperationsServicer_to_server(Operations(stop_event,port,flag=True),server)
    server.add_insecure_port("localhost:"+port)
    print('Iniciando servidor gRPC na porta {}'.format(port))
    try:
        server.start()
        stop_event.wait()
        server.stop(grace=0)
    except KeyboardInterrupt:
        print('\nServidor encerrado pelo usuário')
        print(Operations.data_base)
    except Exception as e:
        print('\nErro durante a execução do servidor:', e)
    finally:
        print("Finalmente")
        server.stop(0)
        
        

        

if __name__=='__main__':
    serve()