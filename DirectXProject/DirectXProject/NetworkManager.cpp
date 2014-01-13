#include "NetworkManager.h"
#include <iostream>

NetworkManager::NetworkManager()
{
	connected = false;
	readable = false;
	writable = true;
}

NetworkManager::~NetworkManager()
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
	message.x_pos = 13.2f;
	message.y_pos = 0.3f;
	message.z_pos = 5.2f;
	message.flags = 0;
	message.time = 0;
	int thingy;
	thingy = htonl(*(unsigned int*)&message.x_pos);
	message.x_pos = *(float*)&thingy;

	thingy = htonl(*(unsigned int*)&thingy);

	message.x_pos = *(float*)&thingy;

	SwapByteOrder<float>(message.y_pos);
	SwapByteOrder<float>(message.y_pos);

	SwapByteOrder<MessageType>(message);
	SwapByteOrder<MessageType>(message);

	NetworkByte<float>(message.z_pos);

	NetworkByte<float>(message.z_pos);

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