#include "Network.h"
#include "Console.h"

Network::Network(char srvPort[8]) {
//WSA startup, so on.
	struct addrinfo sockAddr;
	int result;
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
	
	heartBeat = (struct addrinfo*) malloc(sizeof(struct addrinfo));
	addrinfo *listener = (struct addrinfo*) malloc(sizeof(struct addrinfo));
	memset(&sockAddr, 0, sizeof(struct addrinfo));

	sockAddr.ai_family = PF_INET;
	sockAddr.ai_socktype = SOCK_STREAM;
	sockAddr.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo("www.minecraft.net", "80", &sockAddr, &heartBeat);
	if(result != 0) {
		Console::PrintText("Error in Network: %s", gai_strerror(result));
	}

	sockAddr.ai_flags = AI_PASSIVE;
	result = getaddrinfo(NULL, srvPort, &sockAddr, &listener);
	if(result != 0) {
		Console::PrintText("Error in Network: %s", gai_strerror(result));
	}
	listener_sock = socket(listener->ai_family, listener->ai_socktype, listener->ai_protocol);

	bind(listener_sock, listener->ai_addr, listener->ai_addrlen);
	listen(listener_sock, 1);

	FD_ZERO(&master);
	FD_SET(listener_sock, &master);
	hisock = listener_sock;
	freeaddrinfo(listener);
	Console::PrintText("Listening on port %s.", srvPort);
}

NetworkReturn *Network::acceptConnections() {
	NetworkReturn *Ret = new NetworkReturn();
	SOCKET tmpSock;
	fd_set readfds;
	struct sockaddr_in client_ip;
	int num, i;

	timeval timeout;
	timeout.tv_usec = 50000;
	readfds = master;
	num = select(hisock+1, &readfds, 0, 0, &timeout);

	Ret->tmpSock = (int)INVALID_SOCKET;

	if(num > 0) {
		if(FD_ISSET(listener_sock, &readfds)) { // New connection, accept it
			
			i = sizeof(client_ip);

			tmpSock = accept(listener_sock, (struct sockaddr*) &client_ip, (socklen_t*) &i);

			if(tmpSock == INVALID_SOCKET) {
				closesock(tmpSock);
				return Ret;
			}
			if(tmpSock > hisock)
				hisock = tmpSock;

			Ret->tmpSock = (int)tmpSock;
			Ret->IP = inet_ntoa(client_ip.sin_addr);
			return Ret;
		}
		return Ret;
	}
	return Ret;
}

unsigned short Network::nhtons(unsigned short to) {
	return htons(to);
}

unsigned short Network::nntohs(unsigned short to) {
	return ntohs(to);
}

void Network::run() {
	// No run function as of now.
}

Network::~Network() {
	closesock(listener_sock);
	freeaddrinfo(heartBeat);
}