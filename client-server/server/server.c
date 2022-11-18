#include "server.h"

int main()
{
	if (atexit(end))
		puts("function registration error");

	signal(SIGINT, signal_INT);

	struct sockaddr_in ServerIp;
	pthread_t recvt, sendall;
	int Client_sock = 0, j = 0;

	for (j = 0; j <= MSG_COUNT; j++)
		memset(buffer[j], '\0', MSG_LEN);

	// create list of clients
	clients_list = create(-1);

	ServerIp.sin_family = AF_INET;
	ServerIp.sin_port = htons(PORT);
	ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (bind(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp)) == -1)
	{
		printf("cannot bind, error!! \n");
		return 0;
	}
	else
		printf("Server started working\n");

	if (listen(sock, SOMAXCONN) == -1)
	{
		printf("listening failed \n");
		return 0;
	}

	// created a stream to send to everyone
	pthread_create(&sendall, NULL, (void *)allsend, NULL);

	while (TRUE)
	{
		// wait connect
		if ((Client_sock = accept(sock, (struct sockaddr *)NULL, NULL)) < 0)
			printf("accept failed \n");

		pthread_mutex_lock(&mutex);

		add_element_end(Client_sock, clients_list);

		// stream for receiving messages
		pthread_create(&recvt, NULL, (void *)recvmg, &Client_sock);

		pthread_mutex_unlock(&mutex);
	}
	return 0;
}

void sendtoall(char *msg)
{
	pthread_mutex_lock(&mutex);
	list *tmp = clients_list;
	tmp = tmp->next;
	while (tmp != NULL)
	{
		if (send(tmp->value, msg, strlen(msg), 0) < 0)
		{
			printf("sending failure \n");
			continue;
		}
		tmp = tmp->next;
	}
	pthread_mutex_unlock(&mutex);
}

void *recvmg(void *client_sock)
{
	int sock = *((int *)client_sock);
	char msg[MSG_LEN];
	int len;
	while ((len = recv(sock, msg, MSG_LEN, 0)) > 0)
	{
		pthread_mutex_lock(&mutex);
		msg[len] = '\0';
		// disconnect and end stream
		if (strstr(msg, "dis202"))
		{
			clients_list = remove_element(sock, clients_list);
			break;
		}
		strcpy(buffer[ind], msg);
		printf("%d %s", ind, buffer[ind]); // log msg
		ind++;
		pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_unlock(&mutex);
	printf("%s\n", msg);
	close(sock);
}

void *allsend(void)
{
	size_t i = 0;
	while (TRUE)
	{
		sleep(TIMEOUT);
		for (i = 0; i <= ind; i++)
		{
			sendtoall(buffer[i]);
			memset(buffer[i], '\0', MSG_LEN); // clear
		}
		ind = 0;
	}
}

void end(void)
{
	size_t i = 0;
	char end[] = "Server shutdown\ndis202";

	sendtoall(end);

	close(sock);
	remove_all(clients_list);
	puts("\nServer shutdown");
}

void signal_INT(int signum)
{
	exit(signum);
}