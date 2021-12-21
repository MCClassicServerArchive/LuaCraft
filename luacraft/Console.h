#ifndef _CONSOLE_H
#define _CONSOLE_H
/*	============================================
		The Console namespace is for interacting
		with the console
	============================================ */

#include <cstdio>
#include <ctime>

#include "Various.h"

namespace Console {

	void PrintSplash();
	void DrawDivider(const char *catagoryText);
	void PrintText(const char *format, ...);
}

#endif