#include "client.h"

int main(int argc, char *argv[])
{
    pthread_t recvt;
    int len;
    char send_msg[MSG_LEN], client_name[NAME];
    struct sockaddr_in ServerIp;

    printf("\n");

    if (argc < 2)
    {
        printf("Enter %s [your name] \n\n", argv[0]);
        return 0;
    }

    signal(SIGINT, signal_INT);

    if (atexit(end))
        puts("function registration error");

    strcpy(client_name, argv[1]);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    ServerIp.sin_port = htons(PORT);
    ServerIp.sin_family = AF_INET;
    ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ((connect(sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp))) == -1)
    {
        printf("Connection to socket failed. Try later...\n");
        return 0;
    }
    printf("Connection is successful, enter messages, %s\n", argv[1]);

    // stream for receiving messages
    pthread_create(&recvt, NULL, (void *)recvmg, &sock);

    while (fgets(msg, MSG_LEN, stdin) > 0)
    {

        strcpy(send_msg, client_name);
        strcat(send_msg, ":");
        strcat(send_msg, msg);

        len = write(sock, send_msg, strlen(send_msg));

        if (len < 0)
            printf("\n message not sent \n");

        if (strstr(send_msg, "dis202"))
            break;
    }
    return 0;
}

void *recvmg(void *my_sock)
{
    int sock = *((int *)my_sock), len;

    while ((len = recv(sock, msg, MSG_LEN, 0)) > 0)
    {
        msg[len] = '\0';
        printf("%s", msg);
        fflush(stdout);
        if (strstr(msg, "dis202"))
            raise(SIGINT);
    }
}

void end(void)
{
    char send_msg[] = "-dis202";
    write(sock, send_msg, strlen(send_msg));
    close(sock);
    puts("\nGoodbye!");
}

void signal_INT(int signum)
{
    exit(signum);
}
