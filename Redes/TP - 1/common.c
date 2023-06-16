#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> //uint16_t
#include <sys/socket.h> //socket, connect
#include <sys/types.h> //socket, connect
#include <unistd.h> //close
#include <arpa/inet.h> //inet_pton
#define BUFFER_SIZE 500


void removeNewline(char* str) {
    size_t len = strcspn(str, "\n");
    str[len] = '\0';
}

void Erro(const char* erro){
    perror(erro);
    exit(EXIT_FAILURE);
}

uint16_t Version(char* ip){
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

void AdrsToString(const struct sockaddr* a, char* adrsString, size_t size){
    uint16_t version;
    uint16_t port;
    char addrStr[INET6_ADDRSTRLEN+1]="";
    //printf("Vamos ver se o erro foi aqui\na->sa_family=%hu\n",a->sa_family);
    if(a->sa_family==AF_INET){
        //printf("Chegou aqui?\n");
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