/*	AG0907 UDP server example - by Henry Fortuna and Adam Sampson

	When the user types a message, the client sends it to the server
	as a UDP packet. The server then sends a packet back to the
	client, and the client prints it out.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <iostream>
#include <vector>
#include "ServerClass.h"

#pragma comment(lib, "ws2_32.lib")


// The IP address for the server
#define SERVERIP "127.0.0.1"
//#define SERVERIP "193.60.172.115"
//#define SERVERIP "193.60.172.123"

// The UDP port number for the server
#define SERVERPORT 4444

// The (fixed) size of message that we send between the two programs
#define MESSAGESIZE 40


// Prototypes
void die(const char *message);

struct MessageType {
	char flags;
	float x_pos;
	float y_pos;
	float z_pos;
	float time;
};


std::vector<in_addr> GetIPAddress()
{
    char hostname[80];
	std::vector<in_addr> out;
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		std::cerr << "Error " << WSAGetLastError() << " when getting local host name." << std::endl;
		return out;
	}
	
	struct hostent *hostInfo = gethostbyname(hostname);
	if (hostInfo == 0) {
		std::cerr << "Bad host lookup." << std::endl;
		return out;
    }

	for (int i = 0; hostInfo->h_addr_list[i] != 0; ++i) {
		in_addr temp;
		memcpy(&temp, hostInfo->h_addr_list[i], sizeof(struct in_addr));
		out.push_back(temp);
	}	
	
	return out;
}

int main()
{
	printf("Echo Server\n");

	// Initialise the WinSock library -- we want version 2.2.
	WSADATA w;
	int error = WSAStartup(0x0202, &w);
	if (error != 0)
	{
		die("WSAStartup failed");
	}
	if (w.wVersion != 0x0202)
	{
		die("Wrong WinSock version");
	}
	
	std::vector<in_addr> localAddress = GetIPAddress();

	for (int i = 0; i < localAddress.size(); i++) {
		std::cout << "Address " << i << ": " << inet_ntoa(localAddress[i]) << std::endl;
	}

	// Create a UDP socket.
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	// FIXME: we should test for error here

	// Fill out a sockaddr_in structure to describe the address we'll listen on.
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(SERVERIP);
	// htons converts the port number to network byte order (big-endian).
	serverAddr.sin_port = htons(SERVERPORT);

	// Bind the socket to that address.
	if (bind(sock, (LPSOCKADDR) &serverAddr, sizeof(serverAddr)) != 0)
	{
		die("bind failed");
	}

	// ntohs does the opposite of htons.
	printf("Server socket bound to port %d\n", ntohs(serverAddr.sin_port));

	// We'll use this array to hold the messages we exchange with the client.
	char buffer[MESSAGESIZE];
	MessageType* message;

	while (true)
	{
		printf("Waiting for a message...\n");

		sockaddr_in fromAddr;
		int fromAddrSize = sizeof(fromAddr);
		int count = recvfrom(sock, buffer, MESSAGESIZE, 0,
			                 (sockaddr *) &fromAddr, &fromAddrSize);
		// FIXME: check for error from recvfrom

		printf("Received %d bytes from address %s port %d: '",
			   count, inet_ntoa(fromAddr.sin_addr), ntohs(fromAddr.sin_port));
		message = (MessageType*)&buffer;
		std::cout << "Message: " << std::endl;
		std::cout << "X - " << message->x_pos << std::endl;
		std::cout << "Y - " << message->y_pos << std::endl;
		std::cout << "Z - " << message->z_pos << std::endl;

		//fwrite(buffer, 1, count, stdout);
		//printf("'\n");

		// Send the same data back to the address it came from.
		sendto(sock, buffer, MESSAGESIZE, 0,
			   (const sockaddr *) &fromAddr, sizeof(fromAddr));
		// FIXME: check for errors from sendto
	}

	// We won't actually get here, but if we did then we'd want to clean up...
	printf("Quitting\n");
	closesocket(sock);
	WSACleanup();
	return 0;
}


// Print an error message and exit.
void die(const char *message) {
	fprintf(stderr, "Error: %s\n", message);
	exit(1);
}