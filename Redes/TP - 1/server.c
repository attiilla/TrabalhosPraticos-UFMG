#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //uint16_t
#include <sys/socket.h> //socket, connect
#include <sys/types.h> //socket, connect
#include <unistd.h> //close
#include "common.c"
#include <arpa/inet.h> //inet_pton
#include <netinet/in.h> 
#define BUFFER_SIZE 500
#define MAX_PENDING 0


void ServerV4(char* param){
    char buff[BUFFER_SIZE];
    char *fileName;
    char *fileContent;
    int data;
    char *aux;
    FILE* arq;
    int bound;
    int listened;
    int nameSize;
    char oneLetterExtension[3];
    char twoLetterExtension[4];
    char threeLetterExtension[5];
    struct sockaddr_in client;
    socklen_t clientLen = sizeof(client);
    int accepted;
    char adrStr[BUFFER_SIZE];
    char clientAdrStr[BUFFER_SIZE];
    //Inicialização
    struct sockaddr_in servAdrs;
    memset(&servAdrs, 0, sizeof(servAdrs));
    uint16_t servPort = (uint16_t) atoi(param);
    if(ServSockInit(AF_INET,param,(struct sockaddr_storage *) &servAdrs)!=0){
        Erro("Erro ao iniciar o endereço\n");
    }
    int sock = socket(AF_INET, SOCK_STREAM,0);
    if(sock<0)    Erro("Não foi possível inicializar o socket\n");
    //Abertura passiva
    bound = bind(sock,(struct sockaddr *) &servAdrs, sizeof(struct sockaddr_in));
    if(bound!=0){
        Erro("Erro na função bind\n");
    }
    listening:
    listened = listen(sock,MAX_PENDING);
    AdrsToString((struct sockaddr *) &servAdrs, adrStr, BUFFER_SIZE);
    (listened!=0) ? Erro("Erro na função listen\n") : printf("Ligado a %s\n",adrStr);
    
    accepted = accept(sock,(struct sockaddr *) &client, &clientLen);
    //AdrsToString((struct sockaddr *) &client, clientAdrStr, BUFFER_SIZE);
    if(accepted==-1){
        Erro("Erro ao aceitar conexão");
    }
    
    //Comunicação
    //printf("Conectado\n");
    while(1){
        memset(buff,0,BUFFER_SIZE);
        //printf("Recebendo\n");
        data = recv(accepted,buff,BUFFER_SIZE,0);
        if(strcmp(buff,"exit")==0){
            data = send(accepted,"\x06",1,0);
            close(accepted);
            printf("Connection closed\n");
            accepted = -1;
            goto listening;
        }else if(strcmp(buff,"ERROR")==0){
            data = send(accepted,"\x06",1,0);
            close(accepted);
            goto listening;
        }else{
            if(strcmp(&buff[strlen(buff)-4],"\\end")!=0){
                data = send(accepted,"\n",1,0);
            } else {
                aux = buff;
                do{
                    aux = strchr(aux,'.');
                    if((aux==NULL)||(aux[1]=='\0')){
                        printf("Não foi possível encontrar o nome do arquivo.\nA extensão está correta?");
                        break;
                    }
                    strncpy(oneLetterExtension, aux, 2);
                    oneLetterExtension[2] = '\0';
                    strncpy(twoLetterExtension, aux, 3);
                    twoLetterExtension[3] = '\0';
                    strncpy(threeLetterExtension, aux, 4);
                    threeLetterExtension[4] = '\0';
                    aux = aux+1;
                }while(!(VerifyExt(oneLetterExtension)||VerifyExt(twoLetterExtension)||VerifyExt(threeLetterExtension))) ;
                if(aux!=NULL){
                    aux = aux-1;
                    if(VerifyExt(oneLetterExtension)){
                        fileContent = aux+2;
                        nameSize = aux-buff+2;
                    }
                    if(VerifyExt(twoLetterExtension)){
                        fileContent = aux+3;
                        nameSize = aux-buff+3;
                    }
                    if(VerifyExt(threeLetterExtension)){
                        fileContent = aux+4;
                        nameSize = aux-buff+4;
                    }
                    fileName = (char*) malloc((nameSize+1)*sizeof(char));
                    strncpy(fileName,buff,nameSize);
                    fileName[nameSize] = '\0';
                    aux = strchr(fileContent,'\\');
                    aux[0] = '\0';
                    int over = 0;
                    if (access(fileName, F_OK) == 0){
                        remove(fileName);
                        over = 1;
                    }  
                    arq = fopen(fileName,"wb");
                    fprintf(arq,"%s",fileContent);
                    fclose(arq);
                    free(fileName);
                    over ? printf("file %s overwritten\n",fileName):printf("file %s received\n",fileName);
                    data = send(accepted,"\x06",1,0);
                }
            }
        }
    }
}

void ServerV6(char* param){
    char buff[BUFFER_SIZE];
    char *fileName;
    char *fileContent;
    int data;
    char *aux;
    FILE* arq;
    int bound;
    int listened;
    int nameSize;
    char oneLetterExtension[3];
    char twoLetterExtension[4];
    char threeLetterExtension[5];
    struct sockaddr_in6 client;
    socklen_t clientLen = sizeof(client);
    int accepted;
    char adrStr[BUFFER_SIZE];
    char clientAdrStr[BUFFER_SIZE];
    //Inicialização
    struct sockaddr_in6 servAdrs;
    memset(&servAdrs, 0, sizeof(servAdrs));
    uint16_t servPort = (uint16_t) atoi(param);
    if(ServSockInit(AF_INET6,param,(struct sockaddr_storage *) &servAdrs)!=0){
        Erro("Erro ao iniciar o endereço\n");
    }
    int sock = socket(AF_INET6, SOCK_STREAM,0);
    if(sock<0)    Erro("Não foi possível inicializar o socket\n");
    //Abertura passiva
    bound = bind(sock,(struct sockaddr *) &servAdrs, sizeof(struct sockaddr_in6));
    if(bound!=0){
        Erro("Erro na função bind\n");
    }
    listening:
    listened = listen(sock,MAX_PENDING);
    AdrsToString((struct sockaddr *) &servAdrs, adrStr, BUFFER_SIZE);
    (listened!=0) ? Erro("Erro na função listen\n") : printf("Ligado a %s\n",adrStr);
    
    accepted = accept(sock,(struct sockaddr *) &client, &clientLen);
    //AdrsToString((struct sockaddr *) &client, clientAdrStr, BUFFER_SIZE);
    if(accepted==-1){
        Erro("Erro ao aceitar conexão");
    }
    
    //Comunicação
    //printf("Conectado\n");
    while(1){
        memset(buff,0,BUFFER_SIZE);
        //printf("Recebendo\n");
        data = recv(accepted,buff,BUFFER_SIZE,0);
        if(strcmp(buff,"exit")==0){
            data = send(accepted,"\x06",1,0);
            close(accepted);
            printf("Connection closed\n");
            accepted = -1;
            goto listening;
        }else if(strcmp(buff,"ERROR")==0){
            data = send(accepted,"\x06",1,0);
            close(accepted);
            goto listening;
        }else{
            if(strcmp(&buff[strlen(buff)-4],"\\end")!=0){
                data = send(accepted,"\n",1,0);
            } else {
                aux = buff;
                do{
                    aux = strchr(aux,'.');
                    if((aux==NULL)||(aux[1]=='\0')){
                        printf("Não foi possível encontrar o nome do arquivo.\nA extensão está correta?");
                        break;
                    }
                    strncpy(oneLetterExtension, aux, 2);
                    oneLetterExtension[2] = '\0';
                    strncpy(twoLetterExtension, aux, 3);
                    twoLetterExtension[3] = '\0';
                    strncpy(threeLetterExtension, aux, 4);
                    threeLetterExtension[4] = '\0';
                    aux = aux+1;
                }while(!(VerifyExt(oneLetterExtension)||VerifyExt(twoLetterExtension)||VerifyExt(threeLetterExtension))) ;
                if(aux!=NULL){
                    aux = aux-1;
                    if(VerifyExt(oneLetterExtension)){
                        fileContent = aux+2;
                        nameSize = aux-buff+2;
                    }
                    if(VerifyExt(twoLetterExtension)){
                        fileContent = aux+3;
                        nameSize = aux-buff+3;
                    }
                    if(VerifyExt(threeLetterExtension)){
                        fileContent = aux+4;
                        nameSize = aux-buff+4;
                    }
                    fileName = (char*) malloc((nameSize+1)*sizeof(char));
                    strncpy(fileName,buff,nameSize);
                    fileName[nameSize] = '\0';
                    aux = strchr(fileContent,'\\');
                    aux[0] = '\0';
                    int over = 0;
                    if (access(fileName, F_OK) == 0){
                        remove(fileName);
                        over = 1;
                    }  
                    arq = fopen(fileName,"wb");
                    fprintf(arq,"%s",fileContent);
                    fclose(arq);
                    free(fileName);
                    over ? printf("file %s overwritten\n",fileName):printf("file %s received\n",fileName);
                    data = send(accepted,"\x06",1,0);
                }
            }
        }
    }
}

int main(int argc, char** argv){
    if(argc<3){
        Erro("Número de parâmetros insuficientes\n./server v4|v6 <Server Port>\n");
    } else if(argc>3){
        Erro("Número de parâmetros excedidos\n");
    }
    uint16_t version = Version(argv[1]);
    if(version==4){
        ServerV4(argv[2]);
    }else if(version==6){
        ServerV6(argv[2]);
    }else{
        Erro("Essa não é uma versão do protocolo IP");
    }
    return 0;
}