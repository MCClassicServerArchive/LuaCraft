#ifndef _PLUGIN_H
#define _PLUGIN_H
/*	============================================
		The Plugin class is for handling
		a single plugin/gamemode
	============================================ */

#include "Various.h"

namespace Console {

	void PrintSplash();
	void DrawDivider(const char *catagoryText);
	void PrintText(const char *format, ...);
}

#endif