#include <cstdlib>
#include "Defines.h"

#include "Console.h"
#include "Server.h"

#ifdef _WIN32
	#pragma comment(lib, "ws2_32.lib")
#endif

static Server *server;

static void handle_exit() {
	delete server;
}

int main() {
	#ifdef ENABLE_DEBUG
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	Console::PrintSplash();

	server = new Server();

	if(atexit(handle_exit)) {
		printf("FATAL ERROR: Failed to set exit handler! Closing... {%s}\n", __LINE__);
		#ifdef _WIN32
			system("PAUSE");
		#endif
		exit(1);
		return 1;
	}

	while(!server->Step());
}