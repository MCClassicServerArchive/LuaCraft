#include <stdarg.h>
#include "Network.h"
#include "Defines.h"
#include "Console.h"

Network::Network(char srvPort[8]) {
	int result;

#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

	addrinfo *listener = (struct addrinfo*) malloc(sizeof(struct addrinfo));
	memset(&sockAddr, 0, sizeof(struct addrinfo));

	sockAddr.ai_family = PF_INET;
	sockAddr.ai_socktype = SOCK_STREAM;
	sockAddr.ai_protocol = IPPROTO_TCP;
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
}

NetworkReturn *Network::acceptConnections() {
	NetworkReturn *Ret = new NetworkReturn();
	SOCKET tmpSock;
	fd_set readfds;
	struct sockaddr_in client_ip;
	int i;

	timeval timeout;
	timeout.tv_usec = 50000;
	timeout.tv_sec = 0;
	readfds = master;

	Ret->tmpSock = INVALID_SOCKET;

	if(select(this->hisock+1, &readfds, 0, 0, &timeout) > 0) {
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

char *Network::HttpPost(char *hostname, char *ext, bool getData, char *data, ...) {
	addrinfo *request;
	char vstr[768], *httpheader, *received;
	va_list args;
	int sock;

	memset(vstr, 0x00, 768);

	va_start(args, data);
	vsprintf(vstr, data, args);
	va_end(args);

	httpheader = (char*)malloc(300);
	memset(httpheader, 0x00, 300);

	if(getData) { // Malloc and set if its expecting a return
		received = (char*)malloc(310);
		memset(received, 0x00, 301);
	}
	

	sprintf(httpheader, "POST %s HTTP/1.1\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:%d\r\nContent-Language: en-US\r\nCache-Control: no-cache\r\nPragma:no-cache\r\nUser-Agent: LuaCraft\r\nHost:%s\r\nAccept: text/html, image/gif, image/jpeg, *; q=.2,*/*; q=.2\r\nConnection: keep-alive\r\n\r\n", ext, strlength(vstr), hostname);

	try {
		getaddrinfo(hostname, "80", &sockAddr, &request);
		sock = socket(request->ai_family, request->ai_socktype, request->ai_protocol);
		connect(sock, request->ai_addr, request->ai_addrlen);

		send(sock, httpheader, strlength(httpheader), 0);
		send(sock, vstr, strlength(vstr), 0);
	
		if(getData)
			recv(sock, received, 301, 0);
	closesock(sock);
	} catch (...) {};

	free(httpheader);
	if(getData) 
		return received;

	return 0;
}

char *Network::HttpGet(char *hostname, char *ext, bool getData, char *data, ...) {
	addrinfo *request;
	char vstr[256], httpheader[300], recived[500];
	va_list args;
	int sock;

	va_start(args, data);
	vsprintf(vstr, data, args);
	va_end(args);

	sprintf(httpheader, "GET %s HTTP/1.1\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:%d\r\nContent-Language: en-US\r\nCache-Control: no-cache\r\nPragma:no-cache\r\nUser-Agent: LuaCraft\r\nHost:%s\r\nAccept: text/html, image/gif, image/jpeg, *; q=.2,*/*; q=.2\r\nConnection: keep-alive\r\n\r\n", ext, strlength(vstr), hostname);

	getaddrinfo(hostname, "80", &sockAddr, &request);
	sock = socket(request->ai_family, request->ai_socktype, request->ai_protocol);
	connect(sock, request->ai_addr, request->ai_addrlen);

	send(sock, httpheader, strlength(httpheader), 0);
	send(sock, vstr, strlength(vstr), 0);

	if(getData)
		recv(sock, recived, sizeof(recived), 0);

	closesock(sock);
	return recived;
}

Network::~Network() {
	closesock(listener_sock);
}