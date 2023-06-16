#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //uint16_t
#include <sys/socket.h> //socket, connect
#include <sys/types.h> //socket, connect
#include <unistd.h> //close
#include <arpa/inet.h> //inet_pton
#include "common.c"
//todo funcionalidade da 145

int ProcessRequestClient(char** req, int* sock, char* buff, FILE* arq){
    removeNewline(*req);
    char* token = strtok(*req," ");
    if(strcmp(token,"exit")==0){
        return 0;
    } else if (strcmp(token,"select")==0){
        token = strtok(NULL," ");
        if(strcmp(token,"file")==0){
            token = strtok(NULL," ");
            if(VerifyExt(token)==1){
                char* fileName = strdup(token);
                *req = fileName;
                return 1;
            } else {
                printf("%s not valid!\n",token);
                return 2;
            }
        }
    } else if(strcmp(token,"send")==0){
        token = strtok(NULL," ");
        if(strcmp(token,"file")==0){
            return 3;
        }
    }
    return 4;
}


void ClientV4(char** param){
    FILE* arq = NULL;               //ponteiro para o arquivo
    char buff[BUFFER_SIZE];         //buffer para enviar e receber arquivos
    char req[BUFFER_SIZE];          //leitura das requisições
    int off = 0;                    //offset usado para reconstruir os arquivos
    int processPath;                //variável de decisão que determina o caminho de execução do programa
    int data;                       //variável que guarda quantos dados foram recebidos ou enviados em uma operação    
    char* pointerToReq;
    char fileName[BUFFER_SIZE];
    char* servIP = param[1];
    uint16_t servPort = (uint16_t) atoi(param[2]);
    //Inicialização
    int sock = socket(AF_INET, SOCK_STREAM,0);      //cria um socket IPv4
    if(sock<0)    Erro("Não foi possível inicializar o socket\n");
    
    //Abertura ativa
    struct sockaddr_in servAdrs;
    if(AddressFill(param[1],param[2],(struct sockaddr_storage *) &servAdrs)!=0){        //inicializa os campos de servAdrs
        Erro("Erro ao popular o endereço\n");
    }
    int con = connect(sock,(const struct sockaddr *) &servAdrs, sizeof(servAdrs));
    char adrStr[BUFFER_SIZE];
    AdrsToString((struct sockaddr *) &servAdrs, adrStr, BUFFER_SIZE);
    if(con!=0)      Erro("Não foi possível conectar\n");
    //Comunicação
    while(1){
        instruc:
        memset(req,0,BUFFER_SIZE);
        memset(buff,0,BUFFER_SIZE);
        fgets(req,BUFFER_SIZE-1,stdin);    //coloca o conteúdo da entrada padrão no buffer de requisição
        pointerToReq = req;                 //a função abaixo só aceita arrays dinâmicos
        processPath = ProcessRequestClient(&pointerToReq, &sock, buff, arq);      //verifica se a requisição atende o formato esperado e decide o caminho do programa
        switch(processPath){
            case 0: //desconecta do servidor e fecha
                send(sock, "exit", 5, 0);
                data = recv(sock,buff,1,0);
                close(sock);
                return;
            case 1: //seleciona o arquivo
                arq = fopen(pointerToReq, "rb");
                if (arq == NULL) {
                    printf("%s does not exist\n", pointerToReq);
                } else {
                    printf("%s selected\n", pointerToReq);
                }
                strcpy(fileName,pointerToReq);
                break;
            case 2: //erro já foi impresso, nada acontece
                break;
            case 3: //envia dados
                if(arq==NULL){
                    printf("no file selected!\n");
                    break;
                } else {
                    
                    if (fread(buff, 1, BUFFER_SIZE, arq) <= 0)   Erro("Não foi possível ler esse arquivo\n");
                    strcat(fileName,buff);
                    strcat(fileName,"\\end");
                    data = send(sock, fileName, strlen(fileName), 0);     //envia o arquivo
                    if (data < strlen(fileName)) {
                        Erro("Não foi possível enviar esse arquivo\n");
                    }
                    
                    data = recv(sock,buff,1,0);
                    
                    if(buff[0]=='\x06'){
                        fclose(arq);
                    } else {
                        printf("O arquivo não parece ter sido recebido corretamente.\nTente envia-lo novamente.\n");
                        goto instruc;
                    }
                    break;
                }
            case 4://requisição não reconhecida
                strcpy(fileName,"ERROR");
                send(sock, fileName, strlen(fileName), 0);
                recv(sock,buff,1,0);
                return;
        }
    }
}

void ClientV6(char** param){
    FILE* arq = NULL;               //ponteiro para o arquivo
    char buff[BUFFER_SIZE];         //buffer para enviar e receber arquivos
    char req[BUFFER_SIZE];          //leitura das requisições
    int off = 0;                    //offset usado para reconstruir os arquivos
    int processPath;                //variável de decisão que determina o caminho de execução do programa
    int data;                       //variável que guarda quantos dados foram recebidos ou enviados em uma operação    
    char* pointerToReq;
    char fileName[BUFFER_SIZE];
    char* servIP = param[1];
    uint16_t servPort = (uint16_t) atoi(param[2]);
    //Inicialização
    int sock = socket(AF_INET6, SOCK_STREAM,0);      //cria um socket IPv4
    if(sock<0)    Erro("Não foi possível inicializar o socket\n");
    
    //Abertura ativa
    struct sockaddr_in6 servAdrs;
    if(AddressFill(param[1],param[2],(struct sockaddr_storage *) &servAdrs)!=0){        //inicializa os campos de servAdrs
        Erro("Erro ao popular o endereço\n");
    }
    int con = connect(sock,(const struct sockaddr *) &servAdrs, sizeof(servAdrs));
    if(con!=0)      Erro("Não foi possível conectar\n");
    //Comunicação
    while(1){
        instruc:
        memset(req,0,BUFFER_SIZE);
        memset(buff,0,BUFFER_SIZE);
        fgets(req,BUFFER_SIZE-1,stdin);    //coloca o conteúdo da entrada padrão no buffer de requisição
        pointerToReq = req;                 //a função abaixo só aceita arrays dinâmicos
        processPath = ProcessRequestClient(&pointerToReq, &sock, buff, arq);      //verifica se a requisição atende o formato esperado e decide o caminho do programa
        switch(processPath){
            case 0: //desconecta do servidor e fecha
                send(sock, "exit", 5, 0);
                data = recv(sock,buff,1,0);
                close(sock);
                return;
            case 1: //seleciona o arquivo
                arq = fopen(pointerToReq, "rb");
                if (arq == NULL) {
                    printf("%s does not exist\n", pointerToReq);
                } else {
                    printf("%s selected\n", pointerToReq);
                }
                strcpy(fileName,pointerToReq);
                break;
            case 2: //erro já foi impresso, nada acontece
                break;
            case 3: //envia dados
                if(arq==NULL){
                    printf("no file selected!\n");
                    break;
                } else {
                    
                    if (fread(buff, 1, BUFFER_SIZE, arq) <= 0)   Erro("Não foi possível ler esse arquivo\n");
                    strcat(fileName,buff);
                    strcat(fileName,"\\end");
                    data = send(sock, fileName, strlen(fileName), 0);     //envia o arquivo
                    if (data < strlen(fileName)) {
                        Erro("Não foi possível enviar esse arquivo\n");
                    }
                    
                    recv(sock,buff,1,0);
                    
                    if(buff[0]=='\x06'){
                        fclose(arq);
                    } else {
                        printf("O arquivo não parece ter sido recebido corretamente.\nTente envia-lo novamente.\n");
                        goto instruc;
                    }
                    break;
                }
            case 4://requisição não reconhecida
                strcpy(fileName,"ERROR");
                send(sock, fileName, strlen(fileName), 0);
                recv(sock,buff,1,0);
                return;
        }
    }
}

int main(int argc, char** argv){
    if(argc<3){
        Erro("Número de parâmetros insuficientes\n./client <Server Address> <Server Port>\n");
        exit(EXIT_FAILURE);
    } else if(argc>3){
        Erro("Número de parâmetros excedidos\n");
    }
    uint16_t version = Version(argv[1]);
    if(version==4){
        ClientV4(argv);
    }else if(version==6){
        ClientV6(argv);
    }else{
        Erro("Isso não é um endereço IP\n");
    }
    return 0;
}