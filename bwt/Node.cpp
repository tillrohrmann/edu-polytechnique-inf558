/*
 * Node.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: till
 */

#include "Node.hpp"

Node::Node(): _parent(nullptr),_left(nullptr),_right(nullptr),
_character('\0'),_weight(0),_index(0),_isNS(false){

}

Node::Node(Node* parent, Node* left, Node*right, char character, int weight,
		int index,bool isNS):_parent(parent),_left(left),_right(right),_character(character),
				_weight(weight),_index(index),_isNS(isNS){

}


