from __future__ import print_function
import sys
import grpc
import client_server_pb2, client_server_pb2_grpc
import server_server_pb2, server_server_pb2_grpc

def run():
    location = sys.argv[1]
    print(f"location: {location}")
    channel = grpc.insecure_channel(location, options=[('grpc.connect_timeout_ms', 500000)])
    stub = server_server_pb2_grpc.CentralizedOperationsStub(channel)
    while True:
        try:
            user_input = input()
            tokens = user_input.split(",")
            if tokens[0]=="C":
                ch = int(tokens[1])
                response = stub.mapping(server_server_pb2.Key(k = ch))
                addrss = response.ret
                if addrss:
                    #print(f"address: {addrss}:")
                    tokens = addrss.split(":")
                    ad = "localhost:"+tokens[1]
                    #print(f"ad:\n   {ad}")
                    sub_channel = grpc.insecure_channel(ad, options=[('grpc.connect_timeout_ms', 500000)])
                    sub_stub = client_server_pb2_grpc.OperationsStub(sub_channel)
                    response = sub_stub.check(client_server_pb2.Key(k = ch))
                    print(response.ret)
                    sub_channel.close()
            elif tokens[0]=="T":
                response = stub.terminate(server_server_pb2.Empty())
                channel.close()
                print(response.ret)
                break
        except EOFError:
            break


if __name__=='__main__':
    run()