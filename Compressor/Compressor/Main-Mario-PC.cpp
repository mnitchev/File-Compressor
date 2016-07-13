#include "Compressor.h"
#include "Decompressor.h"

int main() {

	Compressor myComp("out.hff");
	//myComp.compressString(string("input"));
	myComp.compressFile(string("test.txt"));

	Decompressor myDecomp("out.hff");
	myDecomp.decompressFile("out.hff");

	cin.get();
	return 0;
}