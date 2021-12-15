#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include "list.h"

#define PORT      2315
#define TRUE      1
#define MSG_COUNT 100
#define MSG_LEN   100
#define TIMEOUT   30

pthread_mutex_t mutex;

list *clients_list; // list of clients

char buffer[MSG_COUNT][MSG_LEN];

int ind = 0; //counter msg
int sock = 0;

void sendtoall(char *msg);
void *recvmg(void *client_sock);
void *allsend(void);
void end(void);
void signal_INT(int signum);