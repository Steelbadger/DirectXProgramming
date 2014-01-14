#pragma once

#include <WinSock2.h>
#include <map>
#include "../Common/NetworkMessage.h"

class Server
{
public:
	Server();
	~Server();

	void Initialise(const char* ipAddr, unsigned short port);

	void Run();
private:
	void NewConnection(MessageType &message, sockaddr_in address);
	void SendConfirmation(MessageType message);
	void BounceToClients(MessageType message);
	void Send(MessageType message);
	void RecieveConfirmation(MessageType message);
	void SendClientList(MessageType message);

	unsigned int clientIDCounter;
	unsigned int messageCounter;
	SOCKET sock;

	std::map<unsigned int, double> clientUpdates;
	std::map<unsigned int, sockaddr_in> clientAddresses;
	std::map<unsigned int, MessageType> sentMessages;
};