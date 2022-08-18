#include "client.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Wrong start! \nEnter %s [port]\n", argv[0]);
        return 0;
    }
	if (atexit(end))  
	{
		printf("Function registration error");	
		return 0;
	}

	// termination signal (ctrl+C)
	signal(SIGINT, signal_INT);
    struct sockaddr_in server;
    struct stat obj;
    char buf[BUF_LEN], command[COM_LEN], filename[FILENAME_LEN], *f;
    int size, status;
    int filehandle = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == -1)
    {
        printf("Socket creation failed\n");
        return 0;
    }
    server.sin_family = AF_INET;
    server.sin_port = atoi(argv[1]); //port is entered at the meeting
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //server address

    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1)
    {
        printf("Connect Error\n");
        return 0;
    }
	
    while(TRUE)
    {
		printf("\nAvaliable command: [get, put, rm, ls, quit]\nEnter the command: ");
        scanf("%s", command);
        //get file from server
		if (!strcmp(command, "get"))
        {
			printf("Enter filename: ");
			scanf("%s", filename);
			strcpy(buf, "get ");
			strcat(buf, filename); 
			send(sock, buf, BUF_LEN, 0); //send a request
			recv(sock, &size, sizeof(int), 0); //waiting for file size
			if(!size)
				printf("No such file on the remote directory\n\n");
			else
			{ 
				//allocate memory for file data
				f = malloc(size); 
				//accept file
				recv(sock, f, size, 0);
				filehandle = open(filename, O_CREAT | O_WRONLY, 0666);
				//write file to folder
				write(filehandle, f, size);
				close(filehandle);
				free(f);
				//output file
				strcpy(buf, "cat ");
				strcat(buf, filename);
				system(buf);
			}

        }
		//upload file from server
        else if (!strcmp(command, "put"))
        {
			printf("Enter filename: ");
			scanf("%s", filename);
			//open file
			filehandle = open(filename, O_RDONLY);

			if(filehandle == -1)
				printf("No such file on the local directory\n\n");
			else
			{
				strcpy(buf, "put ");
				strcat(buf, filename);
				send(sock, buf, BUF_LEN, 0);
				//pass file size
				stat(filename, &obj); 
				size = obj.st_size;
				send(sock, &size, sizeof(int), 0);
				sendfile(sock, filehandle, NULL, size);
				//waiting for the answer
				recv(sock, &status, sizeof(int), 0);
				if(status) printf("File stored successfully\n");
				else printf("File failed to be stored to remote machine\n");  
			}
        }
		//list of server files
        else if (!strcmp(command, "ls"))
        {
			strcpy(buf, "ls");
			send(sock, buf, BUF_LEN, 0); //send a request
			recv(sock, &size, sizeof(int), 0); //waiting for the answer
			f = malloc(size);
			recv(sock, f, size, 0);
			//it stores the answer to the request
			filehandle = open("temp.txt", O_CREAT | O_WRONLY, 0666);
			write(filehandle, f, size);
			close(filehandle);
			free(f);
			printf("The remote directory listing is as follows:\n\n");
			system("cat temp.txt");
			printf("\n");
			remove("temp.txt");
        }
		//delete file from server
        else if (!strcmp(command, "rm"))
        {
			strcpy(buf, "rm ");
			printf("Enter the filename: ");
			scanf("%s", filename);
			strcpy(buf, "rm ");
			strcat(buf, filename);
			send(sock, buf, BUF_LEN, 0);
			recv(sock, &status, sizeof(int), 0);
			if(status) printf("Failed to delete file\n\n");
			else printf("File was successfully deleted!\n");
        }
        //exit from the server
        else if (!strcmp(command, "quit")) break;
        else printf("Incorrect command. Try again\n");
    }
    return 0;
}

void end(void)
{
	char buf[BUF_LEN];
	int status;
	strcpy(buf, "quit");
	send(sock, buf, BUF_LEN, 0);
	recv(sock, &status, BUF_LEN, 0);
	if(status) printf("\nConnection closed\nQuitting...\n");
	else printf("Server failed to close connection\n");
	close(sock);
	puts("Goodbay!");
}

void signal_INT(int signum) 
{
	exit(signum);
}
