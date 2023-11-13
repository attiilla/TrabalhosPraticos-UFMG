from __future__ import print_function

import os

import grpc

import client_server_pb2, client_server_pb2_grpc

def run():
    channel = grpc.insecure_channel('localhost:8888')
    stub = client_server_pb2_grpc.OperationsStub(channel)
    key 
    str
    response = stub.insertion(client_server_pb2.Key(k = key).Text(s = str))