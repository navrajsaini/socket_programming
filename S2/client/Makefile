# Makefile for the socket programming tutorial 
#

#server = ServerSocket.o Socket.o server.o ServerSocket.h Socket.h SocketException.h
client = ClientSocket.o Socket.o client.o ServerSocket.h Socket.h SocketException.h


all : client

#server: $(server)
#	g++ -std=c++11 $(server) -o server

client: $(client)
	g++ -std=c++11 $(client) -pthread -o client


#Socket: Socket.cpp 
#ServerSocket: ServerSocket.cpp 
ClientSocket: ClientSocket.cpp
#server: server.cpp
client: client.cpp


clean:
	rm -f *.o simple_client
