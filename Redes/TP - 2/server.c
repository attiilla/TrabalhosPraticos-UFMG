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
#include <pthread.h>
#include <time.h>

struct time GetTime(){
    struct time ans;
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);                 // Get current time
    timeinfo = localtime(&rawtime);
    ans.hour = timeinfo->tm_hour;
    ans.minute = timeinfo->tm_min;
}

void ServerPrint(struct streamHeader s, struct time now){
    switch(s.code){
        case 2:
            printf("User %02d removed\n",s.idSender+1);
        case 6:
            if(s.idRec<0){
                printf("[%02d:%02d] %02d: %s",now.hour, now.minute, s.idSender+1, s.Message);
            }
        
    }
}


void* Receiver(void* arg){
    pthread_detach(pthread_self());
    int data;
    struct time now;
    struct Connection* conn = (struct Connection*) arg;
    int sock = conn->socketDesc;
    uint8_t id = conn->userID;
    struct queue* q = conn->queue;
    char buff[BUFFER_SIZE];
    struct streamHeader s;
    while(1){
        data = recv(sock,buff,BUFFER_SIZE,0);
        s = ToStreamHead(buff);
        printHeader(s);
        switch (s.code) {
            /*case 1:
                break;        //Código 1 deve ser resolvido antes da abertura da thread
            */
            case 2:
                now = GetTime();
                //Printa aqui
                ServerPrint(s,now);
                InitHeader(&s,8,0,id,"Code\n01: \"Removed Successfully\"");
                //Coloca na fila a tarefa a realizar
                EnQueue(&q[id],s);
                break;
            case 4:
                
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
        }
    }
    return NULL;
}

void* Sender(void* arg){
    pthread_detach(pthread_self());
    int data;
    struct Connection* conn = (struct Connection*) arg;
    char buff[BUFFER_SIZE];
    struct streamHeader s;
    struct queue* q = conn->queue;
    char* m = (char*) malloc(BUFFER_SIZE*sizeof(char));
    while(1){
        if(q->size!=0){
            s = deQueue(q);
            m = toDataStream(&s);
            send(conn->socketDesc, m, strlen(m), 0);
            printf("Enviado: %s\n", m);
        }
    }
    return NULL;
}

void Server(uint8_t v, char* param){
    //////////////////////////////////////////////////////////////////////////////////////////
    int* userCount;
    int* ID;
    uint8_t protocol;
    socklen_t clientLen;
    uint16_t servPort = (uint16_t) atoi(param);
    struct Connection dataHead;
    pthread_t threadIDs[MAX_USERS];
    int threadRec;
    int threadSend;
    char buff[BUFFER_SIZE];                                 //declaração de variáveis
    int userSock;
    char adrStr[BUFFER_SIZE];
    char clientAdrStr[BUFFER_SIZE];
    struct sockaddr_storage* Adrs;
    struct sockaddr* client;
    struct queue dataQueue[MAX_USERS];
    uint16_t connectedIDs[MAX_USERS];
    struct streamHeader hand;
    ///////////////////////////////////////////////////////////////////////////////////////
    InitIdList(connectedIDs);
    InitQueues(dataQueue,MAX_USERS);
    InitThreadIDs(threadIDs);
    if(v==4){                                       //setup caso a rede use ipv4
        struct sockaddr_in client4;
        clientLen = sizeof(client4);
        struct sockaddr_in servAdrs;
        memset(&servAdrs, 0, sizeof(servAdrs));
        client = &client4;
        Adrs = &servAdrs;
        protocol = AF_INET;
    }else{                                          //setup caso a rede use ipv6
        struct sockaddr_in6 client6;
        clientLen = sizeof(client6);
        struct sockaddr_in6 servAdrs6;
        memset(&servAdrs6, 0, sizeof(servAdrs6));
        client = &client6;
        Adrs = &servAdrs6;
        protocol = AF_INET6;
    }
    *userCount = 0;
    //Inicialização
    if(ServSockInit(protocol,param,Adrs)!=0){
        Erro("Erro ao iniciar o endereço\n");
    }
    int sock = socket(protocol, SOCK_STREAM,0);
    if(sock<0)    Erro("Não foi possível inicializar o socket\n");
    //Abertura passiva
    if(bind(sock, Adrs, clientLen)!=0){
        Erro("Erro na função bind\n");
    }
    while(1){
        if(listen(sock,MAX_PENDING)!=0){
            Erro("Erro na função listen\n");
        } else {
            AdrsToString(Adrs, adrStr, BUFFER_SIZE);
            printf("Ligado a %s\n",adrStr);
        }
        userSock = accept(sock,client, &clientLen);
        if(userSock==-1){
            printf("Erro ao aceitar uma conexão\n");
        }
        recv(userSock,buff,BUFFER_SIZE,0);          //recebe o pedido de inclusão de usuário
        *userCount++;
        if(*userCount>MAX_USERS){                   //caso o número de usuários tenha sido excedido, impede a inclusão de um novo usuário
            InitHeader(&hand,7,-1,-1,"User limit exceeded");
            send(userSock,toDataStream(&hand),1,0); //informa ao usuário
            close(userSock);
            *userCount--;
        } else {        
            InitConn(&dataHead,userSock,connectedIDs,&dataQueue,userCount);//carrega em dataHead os dados do usuário que devem ser passados para a thread
            threadRec = pthread_create(&threadIDs[dataHead.userID],NULL,Receiver,&dataHead);
            threadSend = pthread_create(&threadIDs[dataHead.userID+MAX_USERS],NULL,Sender,&dataHead);
            if(threadRec!=0){
                printf("Erro ao abrir a thread %lu\n",(unsigned long int) dataHead.userID);
                connectedIDs[dataHead.userID] = -1;
                close(userSock);
                *userCount--;
            }else{
                for(int i = 0; i<MAX_USERS;i++){    //informa a todos os usuários conectados a adição de um novo usuário
                    InitHeader(&hand,6,-1,-1,"User 02 joined the group!");
                    if(connectedIDs[i] == 1){
                        hand.idRec = i;
                        EnQueue(&dataQueue[i],hand);
                    }
                }
            }
        }
    }
}
        /*data = recv(accepted,buff,BUFFER_SIZE,0);
        data = send(accepted,"\x06",1,0);*/


int main(int argc, char** argv){
    if(argc<3){
        Erro("Número de parâmetros insuficientes\n./server v4|v6 <Server Port>\n");
    } else if(argc>3){
        Erro("Número de parâmetros excedidos\n");
    }
    uint8_t version = Version(argv[1]);
    if((version==4)||(version==6)){
        Server(version, argv[2]);
    }else{
        Erro("Essa não é uma versão do protocolo IP");
    }
    return 0;
}