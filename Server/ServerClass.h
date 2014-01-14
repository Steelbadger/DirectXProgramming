#include <WinSock2.h>
#include <map>

class Server
{
public:
	Server();
	~Server();

	void Initialise(const char* ipAddr, unsigned short port);

	void Run();
private:
	void NewConnection();
	void RecievedUpdate();

	unsigned int clientIDCounter;
	SOCKET sock;

	std::map<unsigned int, double> clientUpdates;
};