#include "wl.h"

int main(int argc,char** argv){
    int listenfd,connfd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    time_t ticks;

    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if (listenfd < 0) 
        err_quit("socket error");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8000);
    if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
        err_quit("bind error");
    if (listen(listenfd, LISTENQ) < 0)
        err_quit("listen error");
    for( ; ; ){
        connfd = accept(listenfd,(struct sockaddr*)NULL,NULL);
        if (connfd < 0) {
            perror("accept error");
            continue;  // Continue to accept next connection
        }
        ticks = time(NULL);
        snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
        write(connfd,buff,strlen(buff));
        close(connfd);
    }
}