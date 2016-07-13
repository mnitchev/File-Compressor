#ifndef COMPRESSOR_H_
#define COMPRESSOR_H_

#include "HuffmanTree.h"
#include <fstream>

/*
* Класът Compressor приема низ или път към файл
* и компресира данните и ги записва в друг файл.
* В конструкторът се указва къде да бъде запазен новият файл.
*/
typedef pair<char, string> cpair;
typedef unsigned char uchar;
class Compressor {
	list<cpair> charMap;
	string filePath;
public:
	Compressor(string filePath);
	~Compressor() {}

	void compressString(string input);
	void compressFile(string inputFilePath);

	void setOutputFile(string);

private:
	string mapString(string input);
	string findCharacter(char in);
	string convertTree(HuffTree& tree);
	void writeToFile(string outputString, int numberOfBits, string tree);
	void recursiveSearch(HuffNode* root, string code);
	string toBitString(string);
};


#endif