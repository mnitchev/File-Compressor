#include "CommandPrompt.h"

void CommandPrompt::work() {
	char line[1000];
	string temp;
	cout << "$ ";
	while (!done) {
		cin.getline(line, 1000);
		temp = line;
		this->executeOP(temp);
		cout << "$ ";
	}
}

void CommandPrompt::executeOP(string input){
	if (input == string("-c")) {
		currentMode = COMPRESS;
	}
	if (input == string("-d")) {
		currentMode = DECOMPRESS;
	}
	string opcode;
	string parameters;
	bool flag = false;
	for (int i = 0; i < input.size(); i++) {
		if (!flag) {
			if (input[i] == ' ')
				flag = true;
			else
				opcode += input[i];
		}
		else {
			parameters += input[i];
		}
	}
	if (opcode == string("-i")) {
		if (currentMode == COMPRESS) {
			cs.compressFile(parameters);
		}
		if (currentMode == DECOMPRESS) {
			ds.decompressFile(parameters);
		}
		return;
	}
	if (opcode == string("-o")) {
		if (currentMode == COMPRESS) {
			cs.setOutputFile(parameters);
		}
		if (currentMode == DECOMPRESS) {
			ds.setOutputFile(parameters);
		}
		return;
	}
	if (opcode == string("-s")) {
		cs.compressString(parameters);
		return;
	}
	if (opcode == string("quit")) {
		done = true;
	}

}
