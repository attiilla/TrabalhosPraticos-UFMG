import concurrent from futures

import grpc

import client_server_pb2, client_server_pb2_grpc

import sys

data_base = dict()

class Operations(client_server_pb2_grpc.OperationsServicer):
    def insertion(self, key, str, context):
        if key in data_base:
            data_base[key] = str
            return client_server_pb2.Ret_val(ret = 0)
        else:
            return client_server_pb2.Ret_val(ret = -1)

    def check(self, key, context):
        if key in data_base:
            return client_server_pb2.Text(s = data_base[key])
        else:
            return client_server_pb2.Text(s = "")

    def activation(self, str, context):
    def terminate(self, context):



def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=4))
    client_server_pb2_grpc.add_OperationsServicer_to_server(Operations(),server)
    server.add_insecure_port("localhost:8888")
    server.start()
    server.wait_for_termination()

if __name__=='__main__':
    serve()