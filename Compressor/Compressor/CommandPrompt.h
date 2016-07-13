#ifndef COMMANDPROMPT_H_
#define COMMANDPROMPT_H_

#include "Decompressor.h"

enum MODE { COMPRESS = 0, DECOMPRESS = 1 };

class CommandPrompt {
	Compressor cs;
	Decompressor ds;
	bool done;
	MODE currentMode;
public:
	CommandPrompt() : cs(string("C:\\OneDrive\\Uni\\C++\\out.hff")), ds(string("C:\\OneDrive\\Uni\\C++\\test\\out.txt")), currentMode(COMPRESS) {}
	void work();
private:
	void executeOP(string);

};

#endif
