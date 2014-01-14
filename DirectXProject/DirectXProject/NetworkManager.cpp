#include "NetworkManager.h"
#include <iostream>
#include "Hardware.h"

NetworkManager::NetworkManager()
{
	connected = false;
	connecting = false;
	readable = false;
	writable = true;
	uniqueID = 0;
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::Disconnect()
{
	
}

void NetworkManager::Initialise(HINSTANCE hInstance)
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
		return;
	}

	// Create the socket.
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == SOCKET_ERROR)
	{
		std::cout << "socket failed" << std::endl;
	}


	//  Before creating the Windows window, tell the container what Message Handler function to use.
	window.SetMessageHandler<NetworkManager>((*this), std::mem_fn(&NetworkManager::MessageHandler));
	window.CreateMessageWindow("MESSAGES", hInstance);

	if (WSAAsyncSelect(sock, window.GetHandleToWindow(), WM_SOCKET, FD_CLOSE | FD_CONNECT | FD_READ) == SOCKET_ERROR)
	{
		std::cout << "WSAAsyncSelect failed" << std::endl;
		return;
	}
}

void NetworkManager::SetServer(const char * serverAddress, unsigned short portNumber)
{
		// The address to connect to.
		address.sin_family = AF_INET;
		address.sin_port = htons(portNumber);
		address.sin_addr.s_addr = inet_addr(serverAddress);	
}

void NetworkManager::Connect()
{
	if (connecting == false && connected == false) {
		connecting = true;
		MessageType message;
		messageNumber = 0;
		std::cout << "Attempting to Connect..." << std::endl;
		message.type = CONNECT;
		Send(message);
	}
}

void NetworkManager::Send(MessageType message)
{
	message.messageNumber = messageNumber++;
	message.timestamp = clock();
	message.clientID = uniqueID;
	NetworkByte<MessageType>(message);

	// Send the message to the server.
	sendto(sock, (char*)&message, sizeof(MessageType), 0, (const sockaddr *) &address, sizeof(address));
	sentMessages[message.messageNumber] = message;
}

bool NetworkManager::Recieve(MessageType &output)
{
	if (readable) {
		// Read a response back from the server (or from anyone, in fact).
		sockaddr_in fromAddr;
		int fromAddrSize = sizeof(fromAddr);
		int count = recvfrom(sock, (char*)&output, sizeof(MessageType), 0,
								(sockaddr *) &fromAddr, &fromAddrSize);
		// FIXME: check for error from recvfrom

		if (count == SOCKET_ERROR) {

			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				std::cout << "Could Not Read" << std::endl;
				readable = false;
				return false;
			}
			else
			{
				// Something went wrong.
				std::cout << "RECIEVE FAILED" << std::endl;
				return false;
			}

		}

		std::cout << "Received Message" << std::endl;
		NetworkByte<MessageType>(output);

		switch(output.type) {
			case CONNECT:
				NewConnection(output);
				break;
			case UPDATE:
				SendConfirmation(output);
				break;
			case CONFIRM:
				RecieveConfirmation(output);
				break;
			case RESEND:
				break;
			case CLOSE:
				break;
		}

		return true;
	}
	return false;
}

bool NetworkManager::IsConnected()
{
	return connected;
}

void NetworkManager::SendConfirmation(MessageType message)
{
	message.type = CONFIRM;
	NetworkByte<MessageType>(message);

	// Send the message to the server.
	sendto(sock, (char*)&message, sizeof(MessageType), 0, (const sockaddr *) &address, sizeof(address));
}

void NetworkManager::RecieveConfirmation(MessageType message)
{
	if (sentMessages.count(message.messageNumber)) {
		if (sentMessages[message.messageNumber].type == CONNECT) {
			uniqueID = message.clientID;
			std::cout << "CONNECTED.  Client ID Assigned: " << uniqueID << std::endl;
			connecting = false;
			connected = true;
		}
		sentMessages.erase(message.messageNumber);
	} else {
		std::cout << "Unrecognised Confirmation Recieved" << std::endl;
	}
}


void NetworkManager::NewConnection(MessageType message)
{
	if (message.clientID == 0) {
		std::cout << "Illegal Client Connection Notification" << std::endl;	
	} else {
		std::cout << "Make a new client object!  Client: " << message.updateClientID << std::endl;
	}
}


void NetworkManager::TEST_SEND()
{
	// We'll use this buffer to hold the messages we exchange with the server.
	char buffer[40];
	// Read a line of text from the user.
	std::string line = "HALP";
	// Now "line" contains what the user typed.

	// Copy the line into the buffer, filling the rest with dashes.
	memset(buffer, '-', 40);
	memcpy(buffer, line.c_str(), line.size());
	// FIXME: if line.size() is bigger than the buffer it'll overflow (and likely corrupt memory)

	// Send the message to the server.
	sendto(sock, buffer, 40, 0,
			(const sockaddr *) &address, sizeof(address));
}

void NetworkManager::TEST_RECIEVE()
{
	// We'll use this buffer to hold the messages we exchange with the server.
	char buffer[40];
	// Read a response back from the server (or from anyone, in fact).
	sockaddr_in fromAddr;
	int fromAddrSize = sizeof(fromAddr);
	int count = recvfrom(sock, buffer, 40, 0,
			                (sockaddr *) &fromAddr, &fromAddrSize);
	// FIXME: check for error from recvfrom

	if (count == SOCKET_ERROR) {

		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			std::cout << "Could Not Read" << std::endl;
			readable = false;
			return;
		}
		else
		{
			// Something went wrong.
			std::cout << "RECIEVE FAILED" << std::endl;
		}

	}

	std::cout << "Received " << count << "bytes from address " << inet_ntoa(fromAddr.sin_addr) << " port " << ntohs(fromAddr.sin_port) << ": '" << buffer << "'" << std::endl;
}

void NetworkManager::SEND_OBJECT_DATA()
{
	// We'll use this buffer to hold the messages we exchange with the server.
	MessageType message;
	message.xpos = 13.2f;
	message.ypos = 0.3f;
	message.zpos = 5.2f;
	message.type = 0;
	message.timestamp = 0;
	int thingy;
	thingy = htonl(*(unsigned int*)&message.xpos);
	message.xpos = *(float*)&thingy;

	thingy = htonl(*(unsigned int*)&thingy);

	message.xpos = *(float*)&thingy;

	SwapByteOrder<float>(message.ypos);
	SwapByteOrder<float>(message.ypos);

	SwapByteOrder<MessageType>(message);
	SwapByteOrder<MessageType>(message);

	NetworkByte<float>(message.zpos);

	NetworkByte<float>(message.zpos);

	// Send the message to the server.
	sendto(sock, (char*)&message, sizeof(MessageType), 0,
			(const sockaddr *) &address, sizeof(address));
}

void NetworkManager::MessageHandler(Window* window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SOCKET) {
		std::cout << "Windows Message Socket: ";

		// What kind of event was it?
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_CONNECT:
			// connect() completed.
			std::cout << "FD_CONNECT" << std::endl;
			connected = true;
			break;

		case FD_READ:
			// It may be possible to receive.
			std::cout << "FD_READ" << std::endl;
			readable = true;
			break;

		case FD_WRITE:
			// It may be possible to send.
			// We will only get this notification if we've already tried to send
			// and been told that it would block (which is different from select's behaviour).
			std::cout << "FD_WRITE" << std::endl;
			break;
		}
	}
}