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
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")


// The IP address for the server
#define LOCALHOST "127.0.0.1"
//#define SERVERIP "193.60.172.115"
//#define SERVERIP "193.60.172.131"

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
	printf("Initializing Server\n");
	std::string addr;
	do {
		std::cout << "Set Server to localhost?[y/n]: ";
		std::cin >> addr;
	} while (addr == "y\n" || addr == "n\n");

	server.Initialise();
	std::vector<in_addr> addresses = GetIPAddress();
	//for (std::vector<in_addr>::iterator it = addresses.begin(); it != addresses.end(); it++) {
	//	std::cout << "Address: " << inet_ntoa((*it)) << std::endl;
	//}
	if (addr == "y") {
		server.BindSocket(LOCALHOST, SERVERPORT);
		std::cout << "Server bound on: " << LOCALHOST << std::endl;
	} else {
		server.BindSocket(inet_ntoa(addresses[0]), SERVERPORT);
		std::cout << "Server bound on: " << inet_ntoa(addresses[0]) << std::endl;
	}

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