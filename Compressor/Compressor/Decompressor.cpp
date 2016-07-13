#include "Decompressor.h"


/*
* Всеки символ в опашката се записва в temp и се чете с
* бит по бит с помощта на маска, като същевременно се обхожда
* Хъфмановото дърво. Когато се достигне някое не-NULL листо
* символът който съдържа се записва в резултата.
*/
string Decompressor::decompressString(queue<uchar> inputString){
	string result;
	uchar temp, mask;
	int currentNode = 0, currentNumberOfBits = 0;
	while (!inputString.empty()) {
		temp = inputString.front();
		mask = 128;
		for (int i = 0; i < 8 && currentNumberOfBits != numberOfBits; i++) {
			if ((temp & mask) == 0)
				currentNode = (currentNode * 2) + 1;
			else
				currentNode = (currentNode * 2) + 2;

			if (huffTree.c_str()[currentNode] != NULL) {
				result += huffTree.c_str()[currentNode];
				currentNode = 0;
			}

			currentNumberOfBits++;
			mask >>= 1;
		}
		inputString.pop();
	}
	return result;
}


/*
* Първо се извлича информацията от файла в реда в който Compressor я записва.
* Компресираната ифнормация се записва в опашка от uchar и се подава на метода
* decompressString.
*/
void Decompressor::decompressFile(string inputFilePath) {
	ifstream readerStream;
	readerStream.open(inputFilePath, ios_base::binary | ios_base::in);
	readerStream.seekg(0, ios_base::beg);
	readerStream.read(reinterpret_cast<char*>(&this->numberOfBits), sizeof(int));
	char character;
	readerStream.read(reinterpret_cast<char*>(&character), sizeof(char));
	while (character != '\a') {
		huffTree += character;
		readerStream.read(reinterpret_cast<char*>(&character), sizeof(char));
	}
	int bytesToRead = numberOfBits / 8;
	if (numberOfBits % 8 != 0)
		bytesToRead++;
	queue<uchar> compressedString;
	uchar temp;
	while (readerStream.read(reinterpret_cast<char*>(&temp), sizeof(char)) && bytesToRead > 0) {
		compressedString.push(temp);
		bytesToRead--;
	}
	readerStream.close();
	
	string result = this->decompressString(compressedString);
	writeToFile(result);
	cout << result << endl;
}

void Decompressor::writeToFile(string outputString){
	ofstream file;
	file.open(filePath, ios_base::out | ios_base::trunc);
	file << outputString;
	file.close();
}
