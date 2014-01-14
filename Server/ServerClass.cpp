#include "ServerClass.h"
#include <WinSock2.h>
#include <iostream>

Server::Server()
{

}

Server::~Server()
{

}

void Server::Initialise(const char* ipAddr, unsigned short port)
{
	// Initialise the WinSock library -- we want version 2.2.
	WSADATA w;
	int error = WSAStartup(0x0202, &w);
	if (error != 0)
	{
		std::cout << "WSAStartup failed" << std::endl;
		return;
	}
	if (w.wVersion != 0x0202)
	{
		std::cout << "Wrong WinSock version" << std::endl;
	}

	// Create a UDP socket.
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	// FIXME: we should test for error here

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(ipAddr);
	// htons converts the port number to network byte order (big-endian).
	serverAddr.sin_port = htons(port);

	// Bind the socket to that address.
	if (bind(sock, (LPSOCKADDR) &serverAddr, sizeof(serverAddr)) != 0)
	{
		std::cout << "Bind Failed" << std::endl;
		return;
	}
}

void Server::Run()
{

}


void Server::NewConnection()
{

}

void Server::RecievedUpdate()
{

}