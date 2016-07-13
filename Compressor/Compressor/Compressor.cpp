#include "Compressor.h"

Compressor::Compressor(string filePath){
	this->filePath = filePath;
}

/*
* Този метод компресира даден низ и го записва във файл.
* Пътят към файла се задава във конструктора.
*/
void Compressor::compressString(string input){
	/*
	* Първо извикваме методът mapString(string) за да
	* генерираме кодиращата таблица.
	*/
	string tempTree = this->mapString(input);

	/*
	* Променливата compressedString е изходния кодиран низ.
	* Маската ни помага да записваме кода на текущия символ в кодирания низ.
	*
	* stringPos - позиция във входящия низ.
	* bytePos - позиция в текущия байт на compressedString.
	* encoding - кодът на текущия символ от входящия низ.
	*/
	string compressedString;
	uchar mask = 128, temp = 0;
	int stringPos = 0, bytePos = 0, numberOfBits = 0;
	string encoding;

	/*
	* Обхождаме входния низ. Намираме символа в списъка charMap и присвояваме
	* кодът му в encoding. Записваме единиците с помощта на маската и обновяваме
	* bytePos, при нулите само обновяваме bytePos тъй при достъпване на нов байт в
	* compressedString той се записва като 0.
	* При всяко записване прибавяме единица към numberOfBits за да знаем колко бита
	* е изходния низ накрая.
	*/
	for (int i = 0; i < input.size(); i++) {
		char c = input.c_str()[i];
		encoding = findCharacter(input.c_str()[i]);
		for (int enPos = 0; enPos < encoding.size(); enPos++){
			if (bytePos == 8) {
				mask = 128;
				bytePos = 0;
				compressedString += temp;
				temp = 0;
			}
			if (encoding.c_str()[enPos] == '1') {
				mask >>= bytePos++;
				temp |= mask;
				mask = 128;
				numberOfBits++;
			}
			else {
				bytePos++;
				numberOfBits++;
			}
		}
	}

	compressedString += temp;
	int originalSize = input.size() * 8;
	string bitString = toBitString(compressedString);
	cout << "Achieved " << ((numberOfBits * 100) / originalSize) << "% compression compared to original size!" << endl
		<< "Compressed string in bits : " << bitString << endl
		<< "Compressed string as bytes : ";
	for (int i = 0; i < compressedString.size(); i++)
		cout << (int)compressedString.c_str()[i] << " ";
	cout << endl;
	this->writeToFile(compressedString, numberOfBits, tempTree);
}


/*
* Извлича информацията от текстов файл и я предава на compressString.
*/
void Compressor::compressFile(string inputFilePath){
	ifstream readStream;
	string input;
	readStream.open(inputFilePath, ios_base::in);
	char character;
	while (readStream.get(character)) {
		input += character;
	}
	readStream.close();
	this->compressString(input);
}

void Compressor::setOutputFile(string filePath){
	this->filePath = filePath;
}

/*
* Помощна рекурсивна функция за намиране на кода на всеки символ в дървото на хъфман.
*/
void Compressor::recursiveSearch(HuffNode* root, string code) {
	if (root->character != NULL){
		charMap.push_back(cpair(root->character, code));
		return;
	}
	recursiveSearch(root->left, (code + '0'));
	recursiveSearch(root->right, (code + '1'));
}

string Compressor::toBitString(string inputString){
	string output;
	uchar mask = 128, temp;
	for (int i = 0; i < inputString.size(); i++) {
		temp = inputString.c_str()[i];
		while (mask >= 1) {
			if ((temp & mask) == 0)
				output += '0';
			else
				output += '1';
			mask >>= 1;
		}
		mask = 128;
	}
	return output;
}

/*
* Намира дървото на хъфман и конструира кодираща таблица по него.
*/
string Compressor::mapString(string input){
	HuffTree tree(input);
	recursiveSearch(tree.root(), string());
	return convertTree(tree);
}

/*
* Намира даден символ в списъка на кодиране и връща кода.
* ако символа не се намери връща празен string.
*/
string Compressor::findCharacter(char in){
	auto iter = charMap.begin();
	while (iter != charMap.end() && iter->first != in) {
		iter++;
	}
	if (iter == charMap.end())
		return string();
	return iter->second;
}

/*
* Конвертира дърво в последователност от символи в string.
* Дървото се записва като пълно двоично дърво като празните места
* се записват с '\0'.
*/
string Compressor::convertTree(HuffTree& tree){
	/*
	* Резултата се записва в reuslt.
	* Върховете на дървото които предстои да бъдат записани се пазят
	* в опашка.
	* Броим листата които са достигнати с leavesReached.
	*/
	string result;
	queue<HuffNode*> nodeQueue;
	int leavesReached = 0;

	/*
	* Ако се достигне NULL връх се записват още два в опашката,
	* иначе се записват левия и десния връх.
	*/
	nodeQueue.push(tree.root());
	while (leavesReached < charMap.size()) {
		if (nodeQueue.front() == NULL) {
			result += (char)0;
			nodeQueue.push(NULL);
			nodeQueue.push(NULL);
			nodeQueue.pop();
		}
		else {
			if (nodeQueue.front()->character != NULL) {
				result += nodeQueue.front()->character;
				leavesReached++;
			}
			else
				result += (char)0;
			nodeQueue.push(nodeQueue.front()->left);
			nodeQueue.push(nodeQueue.front()->right);
			nodeQueue.pop();
		}
	}
	return result;
}


/*
* Записва кодирания низ във файл.
*/
void Compressor::writeToFile(string outputString, int numberOfBits, string tree){
	ofstream writeStream;
	writeStream.open(filePath.c_str(), ios_base::out | ios_base::binary | ios_base::trunc);

	/*
	* В началото на файла се записва колко бита е кодираната информация.
	* След това се записва дървото на хъфман и кодираната информация 
	* с разделителен знак '\a' между тях.
	*/
	writeStream.write(reinterpret_cast<char*>(&numberOfBits), sizeof(int));
	for (int i = 0; i < tree.size(); i++)
	writeStream.write((&tree.c_str()[i]), sizeof(char));
	writeStream.write((char*)"\a", sizeof(char));
	for (int i = 0; i < outputString.size(); i++)
		writeStream.write((&outputString.c_str()[i]), sizeof(char));
	writeStream.close();
}

