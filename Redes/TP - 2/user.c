#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //uint16_t
#include <sys/socket.h> //socket, connect
#include <sys/types.h> //socket, connect
#include <unistd.h> //close
#include <arpa/inet.h> //inet_pton
#include "common.c"


void User(uint8_t v, char** param){
    char buff[BUFFER_SIZE];         //buffer para enviar e receber arquivos
    uint8_t protocol;
    char* servIP = param[1];
    uint16_t servPort = (uint16_t) atoi(param[2]);
    struct sockaddr_storage* servAdrs; 
    if(v==4){
        protocol = AF_INET;
        struct sockaddr_in servAdrs4;
        servAdrs = &servAdrs4;
    } else {
        protocol = AF_INET6;
        struct sockaddr_in6 servAdrs6;
        servAdrs = &servAdrs6;
    }
    //Inicialização
    int sock = socket(protocol, SOCK_STREAM,0);
    if(sock<0)    Erro("Não foi possível inicializar o socket\n");
    
    //Abertura ativa
    if(AddressFill(param[1],param[2],servAdrs)!=0){
        Erro("Erro ao popular o endereço\n");
    }
    if(connect(sock,(const struct sockaddr *) servAdrs, sizeof(*servAdrs))!=0)      Erro("Não foi possível conectar\n");
    //Comunicação
    printf("Usuario funcionando :D\n");
    /*while(1){
    }*/
}

int main(int argc, char** argv){
    if(argc<3){
        Erro("Número de parâmetros insuficientes\n./user <Server Address> <Server Port>\n");
        exit(EXIT_FAILURE);
    } else if(argc>3){
        Erro("Número de parâmetros excedidos\n");
    }
    uint8_t version = Version(argv[1]);
    if((version==4)||(version==6)){
        User(version, argv);
    }else{
        Erro("Isso não é um endereço IP\n");
    }
    exit(EXIT_SUCCESS);
    return 0;
}