/*
 * AdaptiveHuffmanTree.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: till
 */

#ifndef ADAPTIVEHUFFMANTREE_HPP_
#define ADAPTIVEHUFFMANTREE_HPP_

#include <vector>
#include <map>

#include "CodeObject.hpp"
#include "Bitstream.hpp"

class Node;

class AdaptiveHuffmanTree{
private:
	Node* _root;
	std::vector<Node*> _gallager;
	std::map<unsigned char,Node*> _mapping;
public:
	AdaptiveHuffmanTree();
	~AdaptiveHuffmanTree();

	void deleteTree(Node* root);

	CodeObject getCode(unsigned char ch);

	bool updateNode(Node* node);
	void updateCode(Node* node);

	void printCodes();

	unsigned char getNextChr(Bitstream& stream);

	Node* findLeaf(Node* root,Bitstream& stream);
};




#endif /* ADAPTIVEHUFFMANTREE_HPP_ */
