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

/**
 * This class represents the adaptive Huffman tree
 */
class AdaptiveHuffmanTree{
private:
	Node* _root;
	// saves gallager order of nodes
	std::vector<Node*> _gallager;
	// saves a mapping between unsigned char to its node in the adaptive Huffman tree for quick access
	std::map<unsigned char,Node*> _mapping;
public:
	AdaptiveHuffmanTree();
	~AdaptiveHuffmanTree();

	void deleteTree(Node* root);

	// retrieves binary code associated to character ch
	CodeObject getCode(unsigned char ch);

	// update node function for tree adjustments
	bool updateNode(Node* node);
	// update code function for tree adjustments and thus also adjustments to the code
	void updateCode(Node* node);

	void printCodes();

	// decodes the next character from the stream
	unsigned char getNextChr(Bitstream& stream);

	// traverse the tree until a leaf node with a corresponding char is found (decoding)
	Node* findLeaf(Node* root,Bitstream& stream);
};




#endif /* ADAPTIVEHUFFMANTREE_HPP_ */
