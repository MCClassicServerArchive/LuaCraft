#ifndef _CONFIG_H
#define _CONFIG_H

class Config {

public:

	void *getOption(const char *name, bool addIfMissing, const char *_default, const char *comment);
	bool asBool(void *toBool);

private:

	void addOption(const char *name, const char *_default, const char *comment);

};

#endif