#include "Defines.h"
#include "Console.h"
#include "Server.h"

#ifdef _WIN32
	#pragma comment(lib, "ws2_32.lib")
#endif

int main() {
	Console::PrintSplash();

	Server *server = new Server();
	while(!server->Step());
}