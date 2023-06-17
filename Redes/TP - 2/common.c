#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //uint16_t
#include <sys/socket.h> //socket, connect
#include <sys/types.h> //socket, connect
#include <unistd.h> //close
#include <arpa/inet.h> //inet_pton
#include <pthread.h>
#define BUFFER_SIZE 2048
#define MAX_PENDING 30
#define MAX_USERS 15
void InitThreadID(pthread_t* t){
    for(int i=0;i<MAX_USERS*2;i++){
        t[i] = i;
    }
}


void InitIdList(uint16_t* ids){
    for(int i = 0;i<MAX_USERS;i++){
        ids[i] = -1;
    }
}

uint16_t newID(uint16_t* arr){
    for(int i=0;i<MAX_USERS;i++){
        if(arr[i]< 1){
            arr[i] = 1;
            return i;
        }
    }
    return -1;
}

struct time{
    uint8_t hour;
    uint8_t minute;
};

struct queue{
    struct cell* first;
    struct cell* last;
    uint32_t size;
};

struct streamHeader{
    uint16_t code;
    int16_t idSender;
    int16_t idRec;
    char* Message;
};

struct cell{
    struct cell* next;
    struct streamHeader data;
};

struct Connection{
    int socketDesc;
    int* count;
    int userID;
    struct queue* queue;
};

void InitCell(struct cell* c, struct streamHeader hd){
    c->next = NULL;
    c->data = hd;
}

/*void InitQueues(struct queue** fila, int n){
    fila = (struct queue**) malloc(sizeof(struct queue*)*n);
    for(int i = 0; i<n; i++){
        fila[i] = (struct queue*) malloc(sizeof(struct queue));
        fila[i]->first = NULL;
        fila[i]->last = NULL;
        fila[i]->size = 0;
    }
}*/

void InitQueues(struct queue** fila, int n) {
    *fila = (struct queue*) malloc(sizeof(struct queue) * n);
    for (int i = 0; i < n; i++) {
        fila[i]->first = NULL;
        fila[i]->last = NULL;
        fila[i]->size = 0;
    }
}

void EnQueue(struct queue* fila, struct streamHeader hd){
    struct cell* node = malloc(sizeof(struct cell));
    InitCell(node,hd);
    if(fila->first==NULL){  //caso 1: fila vazia
        fila->first = node;
        fila->last = node;
    } else if(fila->first==fila->last){
        fila->last = node;
        fila->first->next = node;
    } else {
        fila->last->next = node;
        fila->last = node;
    }
    fila->size++;
}

void DestroyHeader(struct streamHeader hd){
    free(hd.Message);
}

void DestroyCell(struct cell* node){
    free(node->next);
    //DestroyHeader(node->data);
}

struct streamHeader deQueue(struct queue* fila){
    struct cell* temp;
    /*if(fila->first==NULL){
        printf("Operação impossível, fila vazia.\n");
    }*/
    struct streamHeader* ans = (struct streamHeader*) malloc(sizeof(struct streamHeader));
    *ans = fila->first->data;
    if(fila->first==fila->last){
        DestroyCell(fila->first);
        fila->first = NULL;
        fila->last = NULL;
    } else {
        temp = fila->first;
        fila->first = fila->first->next;
    }
    fila->size--;
    return *ans;
}

void InitHeader(struct streamHeader* h, int c, int s, int r, const char* m){
    h->code = c;
    h->idSender = s;
    h->idRec = r;
    h->Message = (char*) malloc(sizeof(char)*strlen(m));
    strcpy(h->Message,m);
}

void InitConn(struct Connection* c,int socket,uint16_t* possibleIDs,struct queue *fila,int* i){
    c->socketDesc = socket;
    c->userID = newID(possibleIDs);
    c->count = i;
    c->queue = &fila[c->userID];
}

struct streamHeader ToStreamHead(char* buff){
    struct streamHeader ans;
    int ptr = 3;
    int temp;
    char* aux = (char*) malloc(sizeof(char)*BUFFER_SIZE);
    memset(aux,0,BUFFER_SIZE);
    strncpy(aux, buff, 2);
    ans.code = atoi(aux);             //setando código da mensagem
    memset(aux,0,BUFFER_SIZE);
    if(buff[ptr]=='\n'){          //verificando se o campo está vazio e caso contrário setando os valores do idSender
        ans.idSender = -1;
    } else {
        temp = ptr;
        ptr = ptr+strcspn(&buff[ptr], "\n");
        strncpy(aux,&buff[temp],ptr-temp);
        ans.idSender = atoi(aux);
    }
    ptr++;
    memset(aux,0,BUFFER_SIZE);
    if(buff[ptr]=='\n'){                    //verificando se o campo está vazio e caso contrário setando os valores do idRec
        ans.idRec = -1;
    } else {
        temp = ptr;
        ptr = ptr+strcspn(&buff[ptr], "\n");
        strncpy(aux,&buff[temp],ptr-temp);
        ans.idRec = atoi(aux);
    }
    ptr++;
    memset(aux,0,BUFFER_SIZE);
    strncpy(aux,&buff[ptr],strlen(&buff[ptr]));          //setando a mensagem
    ans.Message = aux;
    return ans;
}

char* toDataStream(struct streamHeader* hd){
    char* buff = (char*) malloc(BUFFER_SIZE*sizeof(char));
    buff[0] = '0';
    buff[1] = hd->code+48;
    buff[2] = '\n';
    uint16_t temp = 3;
    int16_t id = hd->idSender;
    for(int i = 0; i<2; i++){
        if(id>0){
            sprintf(&buff[temp], "%i", id);
            temp = strlen(buff);
        }
        buff[temp] = '\n';
        temp++;
        id = hd->idRec;
    }
    strcpy(&buff[temp],hd->Message);
    return buff;
}

void printHeader(struct streamHeader hd){
    printf("Code: %i\n",hd.code);
    printf("SenderID: %i\n",hd.idSender);
    printf("ReceiverID: %i\n",hd.idRec);
    printf("Message: %s\n",hd.Message);
}
void printCell(struct cell *c){
    printHeader(c->data);
    if(c->next == NULL) printf("Não há próxima célula\n");
    else printf("O endereço da próxima célula é: %p\n",c->next);
}

void printQueue(struct queue* Q){
    if(Q->first==NULL){
        printf("Fila vazia\n");
        return;
    }
    struct cell *temp;
    temp->next = Q->first;
    for(int i=0;i<Q->size;i++){
        temp = temp->next;
        printf("O elemento %i da fila é: \n",i);
        printHeader(temp->data);
        printf("\n\n");
    }
}

void removeNewline(char* str) {
    size_t len = strcspn(str, "\n");
    str[len] = '\0';
}

void Erro(const char* erro){
    perror(erro);
    exit(EXIT_FAILURE);
}

uint8_t Version(char* ip){
    int i = 0;
    if(ip[0]=='v'){
        if(ip[1]=='4') return 4;
        if(ip[1]=='6') return 6;
        printf("%s\n",ip);
        Erro("Isso não é um protocolo IP válido\n");
    }
    while(1){
        if(ip[i]=='.'){
            return 4;
        } else if (ip[i]==':'){
            return 6;
        } else if (ip[i]=='\0'){
            printf("%s\n",ip);
            Erro("Isso não é um endereço IP\n");
        } else {
            i = i+1;
        }
    }
}

int AddressFill(const char* adrStr, const char* portStr, struct sockaddr_storage* sockAd){
    if(adrStr==NULL || portStr==NULL)   return -1;
    uint16_t port = (uint16_t) atoi(portStr);
    if(port==0)    return -1;
    port = htons(port); //converter dispositivo para rede
    memset(sockAd,0,sizeof(struct sockaddr_storage));
    struct in_addr inAddr4;
    if (inet_pton(AF_INET,adrStr,&inAddr4)){
        struct sockaddr_in *ip4 = (struct sockaddr_in *) sockAd;
        ip4->sin_family = AF_INET;
        ip4->sin_port = port;
        memcpy(&(ip4->sin_addr),&inAddr4,sizeof(inAddr4));
        return 0;
    }
    struct in6_addr inAddr6;
    if (inet_pton(AF_INET6,adrStr,&inAddr6)){
        struct sockaddr_in6 *ip6 = (struct sockaddr_in6 *) sockAd;
        ip6->sin6_family = AF_INET6;
        ip6->sin6_port = port;
        memcpy(&(ip6->sin6_addr),&inAddr6,sizeof(inAddr6));
        return 0;
    }
    return -1;
}

int ServSockInit(uint16_t v, const char* portStr, struct sockaddr_storage* sockAd){
    uint16_t port = (uint16_t) atoi(portStr);
    if(port==0){
        printf("Erro 1\nport=%i\n",port);
        return -1;
    }    
    port = htons(port);
    memset(sockAd,0,sizeof(struct sockaddr_storage));
    if(v==AF_INET){
        struct sockaddr_in *ip4 = (struct sockaddr_in *) sockAd;
        ip4->sin_family = AF_INET;
        ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        ip4->sin_port = port;
        return 0;
    }
    if(v==AF_INET6){
        struct sockaddr_in6 *ip6 = (struct sockaddr_in6 *) sockAd;
        ip6->sin6_family = AF_INET6;
        ip6->sin6_port = port;
        ip6->sin6_addr = in6addr_any;
        return 0;
    }
    return -1;
}

void AdrsToString(struct sockaddr* a, char* adrsString, size_t size){
    uint16_t version;
    uint16_t port;
    char addrStr[INET6_ADDRSTRLEN+1]="";
    if(a->sa_family==AF_INET){
        version = 4;
        struct sockaddr_in *ip4 = (struct sockaddr_in *) a;
        if(!inet_ntop(AF_INET,&(ip4->sin_addr),addrStr,INET6_ADDRSTRLEN+1)){
            Erro("Erro em inet_ntop na função AdrsToString");
        }
        port = ntohs(ip4->sin_port);
    } else if(a->sa_family==AF_INET6){
        version = 6;
        struct sockaddr_in6 *ip6 = (struct sockaddr_in6 *) a;
        if(!inet_ntop(AF_INET6,&(ip6->sin6_addr),addrStr,INET6_ADDRSTRLEN+1)){
            Erro("Erro em inet_ntop na função AdrsToString");
        }
        port = ntohs(ip6->sin6_port);
    } else{
        Erro("Erro na função AdrsToString\nA família do socket não é AF_NET ou AF_INET6");
    }
    snprintf(adrsString, size, "IPv%d %s %hu",version,addrStr,port);
}

int VerifyExt(char* file){
    char* ext = strrchr(file, '.');
    if((ext == NULL)||(ext[1] == '\0')){
        return 0;
    }
    ext = ext+1;
    if((strcmp(ext,"c")==0)||(strcmp(ext,"cpp")==0)||(strcmp(ext,"java")==0)||(strcmp(ext,"py")==0)||(strcmp(ext,"tex")==0)||(strcmp(ext,"txt")==0)){
        return 1;
    }
    return 0;
}