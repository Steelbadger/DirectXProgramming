#include "ServerClass.h"
#include <WinSock2.h>
#include <iostream>
#include <time.h>


Server::Server()
{
	clientIDCounter = 1;
	messageCounter = 0;
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
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cout << "Invalid Socket, Error: " << WSAGetLastError() << std::endl;
	}
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
	sockaddr_in fromAddr;
	MessageType message;
	int fromAddrSize = sizeof(fromAddr);
	int count = recvfrom(sock, (char*)&message, sizeof(MessageType), 0,
			                (sockaddr *) &fromAddr, &fromAddrSize);

	if (count == SOCKET_ERROR) {
		std::cout << "Socket Error: " << WSAGetLastError() << std::endl;
	}

	std::map<unsigned int, double>::iterator it;
	for (it = clientUpdates.begin(); it != clientUpdates.end(); it++) {
		if ((*it).second > clock()-CLOCKS_PER_SEC) {
			clientAddresses.erase((*it).first);
			clientUpdates.erase((*it).first);
			break;
		}
	}

	NetworkByte<MessageType>(message);
	std::cout << "Received Message from Client[" << message.clientID << "]: ";
	switch(message.type) {
		case CONNECT:
			std::cout << "CONNECT";
			NewConnection(message, fromAddr);
			BounceToClients(message);
			break;
		case UPDATE:
			std::cout << "UPDATE";
//			SendConfirmation(message);
			BounceToClients(message);
			break;
		case CONFIRM:
			std::cout << "CONFIRM";
			RecieveConfirmation(message);
			break;
		case RESEND:
			std::cout << "RESEND";
			break;
		case CLOSE:
			std::cout << "CLOSE";
			break;
		default:
			std::cout << "UNKNOWN - " << message.type;
	}
	std::cout << std::endl;

}

void Server::SendConfirmation(MessageType message)
{
	if (clientAddresses.count(message.clientID)) {
		sockaddr_in address = clientAddresses[message.clientID];
		message.type = CONFIRM;
		NetworkByte<MessageType>(message);

		// Send the message to the server.
		int count = sendto(sock, (char*)&message, sizeof(MessageType), 0, (const sockaddr *) &address, sizeof(address));
		if (count == SOCKET_ERROR) {
			std::cout << "Unable To Send, Error: " << WSAGetLastError() << std::endl;
		}

	} else {
		std::cout << "Message Recieved from unrecognised client" << std::endl;
	}
}

void Server::RecieveConfirmation(MessageType message)
{
	if (sentMessages.count(message.messageNumber)) {
		sentMessages.erase(message.messageNumber);
	} else {
		std::cout << "Unrecognised Confirmation Recieved" << std::endl;
	}
}


void Server::NewConnection(MessageType &message, sockaddr_in address)
{
	if (message.clientID == 0) {
		message.clientID = clientIDCounter++;
		clientAddresses[message.clientID] = address;
		std::cout << "New Client Connected, assigned ID: " << message.clientID << std::endl;
		SendConfirmation(message);
		SendClientList(message);
	} else {
		std::cout << "Attempting to connect to an illegal client" << std::endl;
	}
}

void Server::BounceToClients(MessageType message)
{
	std::map<unsigned int, sockaddr_in>::iterator it;
	clientUpdates[message.clientID] = clock();
	unsigned int sourceClient = message.clientID;
	message.updateClientID = sourceClient;
	for(it = clientAddresses.begin(); it != clientAddresses.end(); it++) {
		if (sourceClient != (*it).first) {
			message.clientID = (*it).first;
			Send(message);
		}
	}
}

void Server::SendClientList(MessageType message)
{
	std::map<unsigned int, sockaddr_in>::iterator it;
	message.type = CONNECT;
	std::cout << "Sending Client List[" << clientAddresses.size() << "]: ";
	for(it = clientAddresses.begin(); it != clientAddresses.end(); it++) {
		if (message.clientID != (*it).first) {
			message.updateClientID = (*it).first;
			std::cout << message.updateClientID << ", ";
			Send(message);
		}
	}
	std::cout << std::endl;
}

void Server::Send(MessageType message)
{
	message.messageNumber = messageCounter++;
	message.timestamp = clock();
	sockaddr_in address = clientAddresses[message.clientID];
	std::cout << "Sending Message: ";
	switch(message.type) {
		case CONNECT:
			std::cout << "CONNECT";
			break;
		case UPDATE:
			std::cout << "UPDATE";
			break;
		case CONFIRM:
			std::cout << "CONFIRM";
			break;
		case RESEND:
			std::cout << "RESEND";
			break;
		case CLOSE:
			std::cout << "CLOSE";
			break;
		default:
			std::cout << "UNKNOWN - " << message.type;
	}
	std::cout << std::endl;
	NetworkByte<MessageType>(message);

	// Send the message to the server.
	sendto(sock, (char*)&message, sizeof(MessageType), 0, (const sockaddr *) &address, sizeof(address));
	sentMessages[message.messageNumber] = message;
}