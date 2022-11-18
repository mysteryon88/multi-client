#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
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

int sock = 0;

void *recvmg(void *my_sock);
void signal_INT(int signum);
void end(void);

#endif // CLIENT_H