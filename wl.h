#ifndef WL_H
#define WL_H

#include <stdio.h>
#include <stdlib.h>     // 标准库函数，如 exit
#include <string.h>     // 字符串处理函数，如 bzero
#include <time.h>
#include <stdarg.h>
#include <unistd.h>     // UNIX 标准函数，如 read
#include <arpa/inet.h>  // 网络编程函数，如 inet_pton
#include <sys/socket.h> // 套接字编程函数，如 socket, connect
#include <netinet/in.h> // 定义网络地址结构体，如 struct sockaddr_in

#define MAXLINE 4096
#define LISTENQ 10

void err_quit(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    exit(1);
}

#endif