#pragma once

#ifndef _WINSOCK_DEFINED_
#define _WINSOCK_DEFINED_
#include <WinSock2.h>
#endif
#include <queue>
#include "GraphicsWindow.h"
#include "../../Common/NetworkMessage.h"

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void Initialise(HINSTANCE hInstance);
	void SetServer(const char * serveraddress, unsigned short portnumber);
	void Connect();
	bool Recieve(MessageType &output);
	void Send(MessageType message);
	void Update();
	bool IsConnected();

	void TEST_SEND();
	void TEST_RECIEVE();

	void SEND_OBJECT_DATA();

	void MessageHandler(Window* window, UINT message, WPARAM wParam, LPARAM lParam);

	void Disconnect();

private:


	void NewConnection(MessageType message);
	void RecieveConfirmation(MessageType message);
	void SendConfirmation(MessageType message);

	SOCKET sock;
	Window window;
	bool connected;
	bool connecting;
	bool readable;
	bool writable;
	sockaddr_in address;
	unsigned int uniqueID;
	unsigned int messageNumber;

	std::map<unsigned int, MessageType> sentMessages;
	std::queue<MessageType> recievedMessages;

	static const int WM_SOCKET = WM_USER+1;
};
