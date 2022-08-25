package main

import (
	"fmt"
	"net"
)

func main() {
	listener, err := net.Listen("tcp", ":4545")

	if err != nil {
		fmt.Println(err)
		return
	}
	defer listener.Close()
	fmt.Println("Server is listening...")
	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println(err)
			conn.Close()
			continue
		}
		go cli(conn)

	}
}

func cli(conn net.Conn) {
	defer conn.Close()
	fmt.Println("New client connected")
	for {
		input := make([]byte, (1024 * 4))
		n, err := conn.Read(input)
		if n == 0 || err != nil {
			fmt.Println("Client disconnected...")
			break
		}
		source := string(input[0:n])
		conn.Write([]byte(source))
	}
}
