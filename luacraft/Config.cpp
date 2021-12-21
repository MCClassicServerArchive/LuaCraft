#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include "Config.h"
#include "Defines.h"

void *Config::getOption(const char *name, bool addIfMissing, const char *_default, const char *comment) {
	char config_buf[128], *_name;
	void *out;
	FILE *config;
	unsigned char *rem_newline;

	config = fopen("server.cfg", "r");

	_name = (char*)malloc(strlen(name)+2);
	strcpy(_name, name);
	*(_name+strlen(name)) = '=';
	*(_name+strlen(name)+1) = 0;

	if(config != NULL) {
		while(!feof(config)) {
			fgets(config_buf, sizeof(config_buf), config);
			if(strncmp(config_buf, _name, strlen(_name)) == 0) {
				if(strncmp(config_buf+strlen(_name), "true", 4) == 0) {
					out = (void*)malloc(1);
					memcpy(out, "\x01", 1);
				} else if(strncmp(config_buf+strlen(_name), "false", 5) == 0) {
					out = (void*)malloc(1);
					memcpy(out, "\x00", 1);
				} else {
					out = (void*)malloc(strlen(config_buf)-strlen(_name)+1);
					strcpy((char*)out, config_buf+strlen(_name));
				}
			
				free(_name);
				fclose(config);

				rem_newline = (unsigned char*)strchr((const char*)out, '\n');
				if(rem_newline) *rem_newline = 0;
				return out;
			}
		}
	}

	this->addOption(name, _default, comment);
	if(*_default == true) {
		out = (void*)malloc(1);
		memcpy(out, (const void*)true, 1);
	} else if(*_default == false) {
		out = (void*)malloc(1);
		memcpy(out, (const void*)false, 1);
	} else {
		out = (void*)malloc(strlen(_default)+1);
		strcpy((char*)out, _default);
	}

	free(_name);
	return out;
}

bool Config::asBool(void *toBool) {
	char tmp;
	memcpy(&tmp, toBool, 1);
	if(tmp)
		return true;
	else
		return false;
}

void Config::addOption(const char *name, const char *_default, const char *comment) {
	FILE *config;

	config = fopen("server.cfg", "a");

	fprintf(config, "# %s\n", comment);
	if(*_default == true)
		fprintf(config, "%s=true\n", name, _default);
	else if(*_default == false)
		fprintf(config, "%s=false\n", name, _default);
	else
		fprintf(config, "%s=%s\n", name, _default);

	fputs("\n", config);

	fclose(config);
}