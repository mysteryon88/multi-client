# multi-client servers
Multi-client server with streams

## client-server

The server sends messages to all users every 30 seconds if its message buffer is not empty.
The client writes his message to the north buffer.

Commands:
```
dis202 (Ctrl+C) - disconnect from the server

Ctrl+C - shut down the server
```
## archival center

Server/client implementation with network file transfer

Commands:
```
get - get the file
put - put the file on the server
rm - delete file from server
ls - list files
quit (Ctrl+C) - disconnect from the server

Ctrl+C - shut down the server
```
