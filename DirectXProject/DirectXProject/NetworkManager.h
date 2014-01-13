#ifndef _WINSOCK_DEFINED_
#define _WINSOCK_DEFINED_
#include <WinSock2.h>
#endif
#include "GraphicsWindow.h"

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void Initialise(HINSTANCE hInstance);
	void SetServer(const char * serveraddress, unsigned short portnumber);

	void TEST_SEND();
	void TEST_RECIEVE();

	void SEND_OBJECT_DATA();

	void MessageHandler(Window* window, UINT message, WPARAM wParam, LPARAM lParam);

private:
	SOCKET sock;
	Window window;
	bool connected;
	bool readable;
	bool writable;
	sockaddr_in address;

	static const int WM_SOCKET = WM_USER+1;

	struct MessageType {
		char flags;
		float x_pos;
		float y_pos;
		float z_pos;
		float time;
	};
};

 template<class T> void SwapByteOrder(T &value)
 {
	 int size = sizeof(T);
	 T keep = value;
	 char* point = (char*)&value;
	 char* point_keep = (char*)&keep;
	 for (int i = 0; i < size; i++) {
		 point[i] = point_keep[(size-1)-i];
	 }
 }

  template<class T> void NetworkByte(T &value)
 {
	 if (1 != htonl(1)) {
		SwapByteOrder<T>(value);
	 }
 }