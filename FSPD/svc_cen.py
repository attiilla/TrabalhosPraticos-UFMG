import sys, grpc, threading
from concurrent import futures
import server_server_pb2, server_server_pb2_grpc

class CentralizedOperations(server_server_pb2_grpc.CentralizedOperationsServicer):
    data_base = dict()

    def __init__(self, stop_event):
        self._stop_event = stop_event

    def register(self, request, context):
        addrss = request.s
        keys = request.k
        i = 0
        for j in keys:
            i+=1
            CentralizedOperations.data_base[j] = addrss
        print(CentralizedOperations.data_base)
        return server_server_pb2.Ret_val(ret = i)
    
    def mapping(self, request, context):
        key = request.k
        if key in CentralizedOperations.data_base:
            return server_server_pb2.Ret_text(ret = CentralizedOperations.data_base[key])
        else:
            return server_server_pb2.Ret_text(ret = "")
        
    def terminate(self, request, context):
        self._stop_event.set()
        return server_server_pb2.Ret_val(ret = len(CentralizedOperations.data_base))
        

def central_server():
    port = sys.argv[1]
    stop_event = threading.Event()
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    server_server_pb2_grpc.add_CentralizedOperationsServicer_to_server(CentralizedOperations(stop_event),server)
    server.add_insecure_port("localhost:"+port)
    print('Iniciando servidor central gRPC na porta {}'.format(port))
    try:
        server.start()
        stop_event.wait()
        server.stop(grace=0)
    except KeyboardInterrupt:
        print('\nServidor encerrado pelo usuário')
        #print(CentralizedOperations.data_base)
    except Exception as e:
        print('\nErro durante a execução do servidor:', e)
    finally:
        print("Finalmente")
        server.stop(0)



if __name__=='__main__':
    central_server()