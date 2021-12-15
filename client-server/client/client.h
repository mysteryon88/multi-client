#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#define PORT      2315
#define NAME      20
#define MSG_LEN   100

char msg[MSG_LEN];
int sock = 0;

void *recvmg(void *my_sock);
void end(void);
void signal_INT(int signum);