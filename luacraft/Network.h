#ifndef _NETWORK_H
#define _NETWORK_H
/*	============================================
		The Network class is for interacting
		with the network. It contains all code
		to communicate with the clients, and
		handle sockets.
	============================================ */

#ifdef _WIN32
	#include <winsock2.h>
	#include <winsock.h>
	#include <ws2tcpip.h>
#define closesock(sock) closesocket(sock)
#else
	#include <arpa/inet.h>
	#include <sys/socket.h>	
	#include <netinet/in.h>
#define closesock(sock) close(sock)
#endif

#include "Structs.h"

class Network {
public:
	Network(char srvPort[8]);
	~Network();

	NetworkReturn *acceptConnections();

	void run();

	unsigned short nhtons(unsigned short to);
	unsigned short nntohs(unsigned short to);
private:
	typedef int socklen_t;
	SOCKET listener_sock;
	addrinfo *heartBeat;
	fd_set master;
	int hisock;
};

#endif