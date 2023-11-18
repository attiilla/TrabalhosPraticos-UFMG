from __future__ import print_function
import sys
import grpc
import client_server_pb2, client_server_pb2_grpc

def run():
    location = sys.argv[1]
    channel = grpc.insecure_channel(location, options=[('grpc.connect_timeout_ms', 500000)])
    stub = client_server_pb2_grpc.OperationsStub(channel)
    #print("Chegou no cliente")
    keep_running = True
    while keep_running:
        try:
            #user_input = input("Escreve:\n")
            user_input = input()
            tokens = user_input.split(",")
            if tokens[0]=="T":
                #print("tesou")
                keep_running = False
                response = stub.terminate(client_server_pb2.Empty())
                channel.close()
                print(response.ret)
            elif tokens[0]=="A":
                #print("asou")
                response = stub.activate(client_server_pb2.Text(s=tokens[1]))
                print(response.ret)
            elif tokens[0]=="C":
                #print("cesou")
                ch = int(tokens[1])
                response = stub.check(client_server_pb2.Key(k=ch))
                print(response.ret)
            elif tokens[0]=="I":
                #print("isou")
                ch = int(tokens[1])
                response = stub.insert(client_server_pb2.Pair(key_numb=ch, msg_text=tokens[2]))
        except EOFError:
            break

        #print(response.ret)

if __name__=='__main__':
    run()