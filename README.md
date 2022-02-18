# multi-client servers
Multi-client server with streams

Folder "client-server":
Server / client implementation.
The server sends messages to all users every 30 seconds if its message buffer is not empty.
The client writes his message to the north buffer.

Server:

Several threads work, one monitors the buffer and sends messages every 30 seconds. Also, when a server client connects, it creates its own thread for each client. The server buffer size is 100 messages of 100 characters. The list of clients is stored in the list, where a new client is added when connecting. If the command "dis202" came from the client, then the server releases the connection number and the list element, the flow for the client ends.
The server can be closed with the ctrl+C command, a signal handler (SIGINT) will run, which will send a message to all clients from the server shutdown, send the "dis202" command, close the socket and clear the list of clients.
To avoid unexpected situations with the server buffer, when sending and receiving messages, we block all threads with a common mutex

Customer:

Connects to the server, creates a separate thread for receiving messages, sends messages to main. To disconnect from the server, you need to send the "dis202" command. It will call break, which will end the message input loop and the client process itself. When the server shuts down, clients receive the "dis202" command, send themselves a signal (SIGINT), close the socket, and exit.
If the client disconnects from the server using the ctrl+C command, the signal handler catches (SIGINT) where the "dis202" command is sent to disconnect the client from the server
