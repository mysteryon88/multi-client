#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// for getting file size using stat()
#include <sys/stat.h>

// for sendfile()
#include <sys/sendfile.h>

// for O_RDONLY
#include <fcntl.h>

#define FILENAME_LEN 20
#define BUF_LEN 100
#define COM_LEN 5
#define TRUE 1

// mutex to avoid unexpected situations
pthread_mutex_t files;

int sock = 0;

void end(void);
void signal_INT(int signum);
void *recvcom(void *client_sock);

#endif // SERVER_H