#include "HuffmanTree.h"

/*
* Оператор за сравнение на върхове в дървото.
*/
bool HuffNode::operator<(HuffNode const& B) const {
	if (this->frequency == B.frequency)
		return this->character != NULL && B.character == NULL;
	return this->frequency < B.frequency;
}

/*
* Записва ново срещане на character.
* Ако не съществува такъв символ в списъка създава нов с 1 срещане.
*/
void AlphabetMap::push(char character){
	if (map.empty()){
		map.push_back(upair(character, 1));
	}
	else{
		auto iter = map.begin();
		while (iter != map.end() && iter->first != character){
			iter++;
		}
		if (iter == map.end()){
			map.push_back(upair(character, 1));
		}
		else{
			iter->second++;
		}
	}
}


void AlphabetMap::mapString(string inputString){
	for (int i = 0; i < inputString.size(); i++){
		this->push(inputString.c_str()[i]);
	}
}

/*
* Двата вида конструктора са за листо и за нормален връх.
*/
HuffNode::HuffNode(int freq, char charactr) : frequency(freq), left(NULL), right(NULL), character(charactr){}
HuffNode::HuffNode(int freq, HuffNode* _left, HuffNode* _right) : frequency(freq), left(_left), right(_right), character(NULL){}


/*
* Създава честотна таблица по string.
*/
AlphabetMap HuffTree::buildMap(string inputString){
	AlphabetMap result(inputString);
	return result;
}

/*
* Изгражда дървото по честотна таблица.
*/
void HuffTree::buildTree(AlphabetMap almap){
	/*
	* Дървото се построява с помощтта на priority_queue с компаратор.
	* Взимат се първите два върха в опашката и се събират в един, който
	* се добавя в опашката. По този начин винаги се взимат най-малките върхове.
	* Това се прави докато не остане един връх - корена на дървото.
 	*/
	auto iter = almap.begin();
	priority_queue<HuffNode*, std::vector<HuffNode*>,Comparison> trees;
	for (int i = 0; i < almap.size(); i++){
		trees.push(new HuffNode(iter->second, iter->first));
		iter++;
	}
	HuffNode* tempLeft;
	HuffNode* tempRight;
	HuffNode* temp;
	while (trees.size() > 1){
		tempLeft = trees.top();
		trees.pop();
		tempRight = trees.top();
		trees.pop();
		temp = new HuffNode((tempLeft->frequency + tempRight->frequency), tempLeft, tempRight);
		trees.push(temp);
	}
	if (!trees.empty()) {
		treeRoot = trees.top();
		trees.pop();
	}
}