/*
 * Node.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: till
 */

#ifndef NODE_HPP_
#define NODE_HPP_

#include "CodeObject.hpp"

class Node{
public:
	Node* _parent;
	Node* _left;
	Node* _right;
	char _character;
	int _weight;
	int _index;
	CodeObject _code;
	bool _isNS;

	Node();
	Node(Node* parent, Node* left, Node* right, char character, int weight, int index,bool isNS =false);
};


#endif /* NODE_HPP_ */
