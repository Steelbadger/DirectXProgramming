#include "NetworkMessage.h"

template<> void NetworkByte<MessageType>(MessageType &message)
{
	NetworkByte<unsigned int>(message.clientID);
	NetworkByte<unsigned int>(message.messageNumber);
	NetworkByte<float>(message.xpos);
	NetworkByte<float>(message.ypos);
	NetworkByte<float>(message.zpos);
	NetworkByte<float>(message.s);
	NetworkByte<float>(message.xorient);
	NetworkByte<float>(message.yorient);
	NetworkByte<float>(message.zorient);
	NetworkByte<double>(message.timestamp);
}