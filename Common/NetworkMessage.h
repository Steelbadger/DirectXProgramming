#pragma once

#include <WinSock2.h>

#define CONNECT		1
#define UPDATE		2
#define CONFIRM		3
#define RESEND		4
#define CLOSE		5

#pragma pack(push, 1)
struct MessageType
{
	char type;
	unsigned int clientID;
	unsigned int messageNumber;
	union {
		struct {
			unsigned int updateClientID;
			float xpos;
			float ypos;
			float zpos;
			float s;
			float xorient;
			float yorient;
			float zorient;
		};
	};
	double timestamp;
};
#pragma pack(pop)


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

template<> void NetworkByte<MessageType>(MessageType &message);