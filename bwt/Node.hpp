/*
 * Node.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: till
 */

#ifndef NODE_HPP_
#define NODE_HPP_

#include "CodeObject.hpp"

/**
 * This class represents a node of the adaptive Huffman tree
 */
class Node{
public:
	Node* _parent;
	Node* _left;
	Node* _right;
	char _character;
	int _weight;
	// index of gallager sequence
	int _index;
	// current binary code of the node
	CodeObject _code;
	// is this node a new sign node?
	bool _isNS;

	Node();
	Node(Node* parent, Node* left, Node* right, char character, int weight, int index,bool isNS =false);
};


#endif /* NODE_HPP_ */
