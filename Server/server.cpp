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

// Prototypes
void die(const char *message);

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
	Server server;
	printf("Echo Server\n");
	server.Initialise(SERVERIP, SERVERPORT);

	while (true)
	{
		server.Run();
	}

	WSACleanup();
	return 0;
}


// Print an error message and exit.
void die(const char *message) {
	fprintf(stderr, "Error: %s\n", message);
	exit(1);
}