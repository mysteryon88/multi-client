#include "server.h"

int main(int argc,char *argv[])
{
    if (argc == 1)
    {
        printf("Please, enter server port!\n");
        return 0;
    }

    if (atexit(end))  
    {
        puts("Function registration error");	
        return 0;
    }

    // termination signal (ctrl+C)
    signal(SIGINT, signal_INT);

    struct sockaddr_in server;
    pthread_t recv;
    int Client_sock = 0;

    //create a socket   
    server.sin_port = atoi(argv[1]); //port is entered at the meeting
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //our ip
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == -1)  
    {  
        printf("Socket creation failed\n");
		return 0;
    }

    if(bind(sock,(struct sockaddr*)&server,sizeof(server)) == -1)
    {  
        printf("Binding error\n");
       	return 0;
    }

    if(listen(sock, SOMAXCONN) == -1)    
    {
		printf("Listen failed\n");
		return 0;   
    }
    while(TRUE)    
    {  
		//Waiting for client connection
		if((Client_sock = accept(sock, (struct sockaddr *)NULL, NULL)) < 0) 
			printf("Accept failed\n");
		else printf("Accept successfully\n");
		//create a thread for each client
		pthread_create(&recv, NULL,(void *)recvcom, &Client_sock);
    }
    return 0;

}

void *recvcom(void *client_sock)
{
	int c_sock = *((int *)client_sock);
	char buf[BUF_LEN], command[COM_LEN], filename[FILENAME_LEN];
	int size = 0, len = 0, c = 0, filehandle = 0;
	struct stat obj;
	//we receive commands from the client in an endless loop
	while((len = recv(c_sock, buf, BUF_LEN, 0)) > 0) 
	{	
		//[command] [filename] 
		sscanf(buf, "%s", command);  //got a command from a string
		if(!strcmp(command, "quit")) break;
		else if(!strcmp(command, "ls"))
		{
			pthread_mutex_lock(&files);
			//get a list of files in a folder and write to a temporary file
			system("ls>temp.txt"); 
			//get file size
			stat("temp.txt",&obj);
			size = obj.st_size;
			send(c_sock, &size, sizeof(int), 0);
			filehandle = open("temp.txt", O_RDONLY);
			sendfile(c_sock,filehandle, NULL, size);
		   	close(filehandle);
		    remove("temp.txt");
			pthread_mutex_unlock(&files);
		}
		else if(!strcmp(command,"get"))
		{
			sscanf(buf, "%s%s", filename, filename);
			//we block resources for other threads so that there are no conflicts
			//for example, someone deleted the file we requested
			pthread_mutex_lock(&files);
			stat(filename, &obj); //get file size
			filehandle = open(filename, O_RDONLY);
			size = obj.st_size;

			if(filehandle == -1) size = 0;

			send(c_sock, &size, sizeof(int), 0);
			//sending a file
			if(size) 
			{
				sendfile(c_sock, filehandle, NULL, size);
				printf("The file (%s) has been sent\n", filename);
			}
			else printf("No such file on the remote directory (%s)\n", filename);
			pthread_mutex_unlock(&files);  

		}
		else if(!strcmp(command, "put"))    
		{
			pthread_mutex_lock(&files);
			int c = 0, len = 0;
			char *f;
			
			sscanf(buf + strlen(command), "%s", filename);
			recv(c_sock, &size, sizeof(int), 0);
			
			//Rewriting file
			filehandle = open(filename, O_CREAT | O_WRONLY, 0666);
			f = malloc(size);
			recv(c_sock, f, size, 0);
			c = write(filehandle, f, size);
			close(filehandle);
			free(f);
			send(c_sock, &c, sizeof(int), 0);
			pthread_mutex_unlock(&files);
			printf("The file (%s) has been saved\n", filename);

		}
		else if(!strcmp(command, "rm"))
		{
			pthread_mutex_lock(&files);
			sscanf(buf + strlen(command), "%s", filename);
			//deleting a file
			int status = remove(filename);
			send(c_sock, &status, sizeof(int), 0);
			pthread_mutex_unlock(&files);
			if(status) printf("Failed to delete file (%s)\n", filename);
			else printf("The file (%s) has been deleted\n", filename);
		}
	}
	//send a disconnect notification from the server
	int i = 1;
	send(c_sock, &i, sizeof(int), 0);
	//close client's socket
	close(c_sock);
	printf("User left the server\n");
}

void end(void)
{
	close(sock);
	puts("\nServer shutdown");
}

void signal_INT(int signum) 
{
	exit(signum);
}
