#include <stdio.h>
#include <stdarg.h>
#include "Console.h"

// Display splash screen (called during startup)
void Console::PrintSplash() {
char *splashText[] = {	"      _                  ____            __ _",
						"     | |    _   _  __ _ / ___|_ __ __ _ / _| |_",
						"     | |   | | | |/ _` | |   | '__/ _` | |_| __|",
						"     | |___| |_| | (_| | |___| | | (_| |  _| |_",
						"     |_____|\\__,_|\\__,_|\\____|_|  \\__,_|_|  \\__|",
						"",
						"                   Version: 0.3B",
						"",
						"      Website   : sourceforge.net/projects/luacraft",
						"      Forum     : luacraft.minipenguin.com",
						"      IRC       : #luacraft @ irc.esper.net" };
                                            
for(int i = 0; i < sizeof(splashText) / sizeof(char*); i++)
	printf("%s\n", splashText[i]);
printf("\n");                                    

}

// Draw a divider on the console with catagory text
void Console::DrawDivider(const char *catagoryText) {
	printf("\n --- [ %s ] ", catagoryText);
	for(int i = 0; i < (71 - strlength(catagoryText) - 20); i++)
		printf("-");
	printf("\n\n");
}

// Output text to console
void Console::PrintText(const char *format, ...) {
	time_t t;
	t = time(0);
	tm *localTime = localtime(&t);

	if(format[0] == '\0')
		return;
	char vstr[512];
	va_list args;

	va_start(args, format);
	vsprintf(vstr, format, args);
	va_end(args);

	printf("[%d:%d:%d] %s\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec, vstr);
}