#ifndef HUFFMANTREE_H_
#define HUFFMANTREE_H_

#include <list>
#include <utility>
#include <string>
#include <queue>
#include <functional>
#include <iostream>


using namespace std;


typedef pair<char, int> upair;
typedef list<upair>::iterator uiterator;

/*
* Клас за честотната таблица на низ.
*/
class AlphabetMap{
	list<upair> map;
public:
	AlphabetMap(){}
	AlphabetMap(string inputString){ this->mapString(inputString); }
	~AlphabetMap(){}

	void mapString(string inputString);
	uiterator begin(){ return map.begin(); }
	uiterator end(){ return map.end(); }

	int size(){ return map.size(); }

private:
	void push(char character);
};

/*
* Клас за връх на дърво на Хъфман.
*/
struct HuffNode{
	int frequency;
	HuffNode* left;
	HuffNode* right;
	char character;

	HuffNode(int, char);
	HuffNode(int, HuffNode*, HuffNode*);
	~HuffNode() {}

	bool operator<(HuffNode const& B) const;
	bool isLeaf() { return left == NULL && right == NULL; }
	void destroy() {
		if (left != NULL && !left->isLeaf()) {
			left->destroy();
			delete left;
		}
		if (right != NULL && !right->isLeaf()) {
			right->destroy();
			delete right;
		}
	}
};

/*
* Компаратор
*/
class Comparison {
public:
	bool operator() (const HuffNode* A, const HuffNode* B) const {
			if (A->frequency == B->frequency)
				return B->character != NULL && A->character == NULL;
			return A->frequency > B->frequency;
	}
};

class HuffTree{
	HuffNode* treeRoot;
public:
	HuffTree(string inputString) { this->buildTree(this->buildMap(inputString)); }
	~HuffTree() { 
		treeRoot->destroy(); 
	}

	HuffNode* root() { return treeRoot; }
private:
	AlphabetMap buildMap(string inputString);
	void buildTree(AlphabetMap);
};

#endif