#ifndef  DECOMPRESSOR_H_
#define DECOMPRESSOR_H_

#include "HuffmanTree.h"
#include "Compressor.h"

class Decompressor {
	string huffTree;
	string filePath;
	int numberOfBits;
public:
	Decompressor(string filePath) { this->filePath = filePath; this->numberOfBits = 0; }

	void setOutputFile(string filePath) { this->filePath = filePath; }
	void decompressFile(string inputFilePath);

private:
	string decompressString(queue<uchar> inputString);
	void writeToFile(string outputString);
	
};
#endif // ! DECOMPRESSOR_H_
