from __future__ import print_function

import sys

import os

import grpc

import client_server_pb2, client_server_pb2_grpc

def run():
    location = sys.argv[1]
    channel = grpc.insecure_channel(location)
    stub = client_server_pb2_grpc.OperationsStub(channel)
    print("Chegou no cliente")
    keep_running = True
    while keep_running:
        user_input = input("Escreve:\n")
        tokens = user_input.split(",")
        if tokens[0]=="T":
            keep_running = False
            response = stub.terminate(client_server_pb2.Empty())
            channel.close()
        elif tokens[0]=="A":
            response = stub.activate(client_server_pb2.Text(s=tokens[1]))
        elif tokens[0]=="C":
            ch = int(tokens[1])
            response = stub.check(client_server_pb2.Key(k=ch))
        elif tokens[0]=="I":
            ch = int(tokens[1])
            print(f"ch = {ch}\ntexto = {tokens[2]}")
            response = stub.insert(client_server_pb2.Pair(key_numb=ch),client_server_pb2.Pair(msg_text=tokens[2]))
        else:
            response = -20
            print("error")
        print(response)


if __name__=='__main__':
    run()