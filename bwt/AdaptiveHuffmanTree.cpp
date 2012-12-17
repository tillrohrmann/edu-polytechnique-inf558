/*
 * AdaptiveHuffmanTree.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: till
 */

#include "AdaptiveHuffmanTree.hpp"
#include "Node.hpp"

#include <iostream>

const bool DEBUG=false;

AdaptiveHuffmanTree::AdaptiveHuffmanTree(){
	_root = new Node();
	_root->_weight =1;
	_root->_isNS = true;
	_gallager.push_back(_root);
}

AdaptiveHuffmanTree::~AdaptiveHuffmanTree(){
	deleteTree(_root);
	_root = nullptr;
}

void AdaptiveHuffmanTree::deleteTree(Node* root){
	if(root->_left != nullptr){
		deleteTree(root->_left);
		delete root->_left;
	}

	if(root->_right != nullptr){
		deleteTree(root->_right);
		delete root->_right;
	}
}

/**
 * This function updates the Huffman tree by incrementing the weight of node
 * and restoring the gallager property
 *
 * @return bool true iff updateCode has been called for this node and all its children
 * 				false iff updateCode has not been called
 */
bool AdaptiveHuffmanTree::updateNode(Node* node){
	if(node == nullptr){
		return false;
	}
	//root node
	if(node->_parent == nullptr){
		node->_weight++;
		return false;
	}else{
		Node* pre = _gallager[node->_index-1];

		// if true, gallager order stays the same by incrementing the weight of node
		if(pre->_weight > node->_weight){
			node->_weight++;

			return updateNode(node->_parent);
		}else{
			int index = node->_index-2;

			// search first node which has a higher weight than node->_weight
			while(index >=0 && _gallager[index]->_weight <= node->_weight)
				index--;

			// the node with which node will be swapped to restore gallager order
			pre = _gallager[index+1];

			// calculate position where the nodes are attached to their parents
			bool nodeLeft = node->_parent->_left == node;
			bool preLeft = pre->_parent->_left == pre;

			//exchange pre and node
			if(nodeLeft){
				node->_parent->_left = pre;
			}else
				node->_parent->_right = pre;

			if(preLeft){
				pre->_parent->_left = node;
			}else{
				pre->_parent->_right = node;
			}

			Node* swap = pre->_parent;
			pre->_parent = node->_parent;
			node->_parent = swap;

			// update code of the swapped node
			updateCode(pre);

			// swap gallager mapping
			_gallager[node->_index] = pre;
			_gallager[pre->_index] = node;

			int iswap = pre->_index;
			pre->_index = node->_index;
			node->_index = iswap;

			node->_weight++;

			//update the parent node as well
			if(!updateNode(node->_parent)){
				updateCode(node);
			}

			return true;
		}
	}
}

/**
 * This function calculates the new code of a node if it has
 * been moved. It takes the parent code and appends either a 0
 * or a 1 depending on the side on which it was appended
 */
void AdaptiveHuffmanTree::updateCode(Node* node){
	if(node == nullptr){
		return;
	}
	if(node->_parent != nullptr){
		if(node->_parent->_left == node){
			node->_code = node->_parent->_code.appendZero();
		}else{
			node->_code = node->_parent->_code.appendOne();
		}
	}else{
		node->_code = CodeObject();
	}

	updateCode(node->_left);
	updateCode(node->_right);
}

/**
 * This function retrieves the current binary code for the character ch and increases
 * the weight of this character. Consequently the adaptive Huffman tree will also be
 * adapted
 */
CodeObject AdaptiveHuffmanTree::getCode(unsigned char ch){
	std::map<unsigned char,Node*>::iterator it;
	// if there exists a code for the character ch
	if((it=_mapping.find(ch)) != _mapping.end()){
		Node* node = it->second;

		CodeObject result = node->_code;

		// update weight of respective node
		updateNode(node);
		if(DEBUG){
			std::cout << std::endl << std::endl << "Old Chr:" << ch << " int: " << (unsigned int)ch << " Code:" << result.toStr() << std::endl << std::endl;
			printCodes();
		}


		return result;
	}else{
		// the character ch was read for the first time --> get the code of the new sign node
		// which is situated at the end of the gallager sequence
		Node* oldSignNode = _gallager[_gallager.size()-1];

		// old sign node is no longer a new sign node
		oldSignNode->_isNS = false;

		Node* newSignNode = new Node(oldSignNode,nullptr,nullptr,'?',1,_gallager.size()+1,true);
		Node* newChNode = new Node(oldSignNode,nullptr,nullptr,ch,1,_gallager.size());

		_gallager.push_back(newChNode);
		_gallager.push_back(newSignNode);

		oldSignNode->_left = newChNode;
		oldSignNode->_right = newSignNode;

		// create binary code by append the 8 bit ASCII representation of the character
		// to the binary code of the new sign node
		CodeObject result = oldSignNode->_code.append(ch);

		//update the Huffman tree
		updateNode(oldSignNode);
		updateCode(newChNode);
		updateCode(newSignNode);

		if(DEBUG){
			std::cout << std::endl << std::endl <<  "New Chr:" << ch << " int: " << (unsigned int)ch << " Code:" << result.toStr() << std::endl << std::endl;
			printCodes();
		}

		// set the new mapping
		_mapping[ch] = newChNode;

		return result;
	}
}

void AdaptiveHuffmanTree::printCodes(){
	for(std::vector<Node*>::iterator it = _gallager.begin(); it != _gallager.end();it++){
		std::cout << "Chr:" << (*it)->_character << " code:" << (*it)->_code.toStr() << " weight:" << (*it)->_weight<<std::endl;
	}
}

/**
 * This function decodes the next char from the bitstream
 */
unsigned char AdaptiveHuffmanTree::getNextChr(Bitstream& stream){
	// find leaf corresponding to the current sequence on the stream
	Node * leaf = findLeaf(_root,stream);
	unsigned char result;
	if(leaf == nullptr){
		std::cerr << "Error" << std::endl;
		exit(1);
	}

	// if the leaf is a new sign node --> oen has to read the next 8 bits to retrieve the next character
	if(leaf->_isNS){
		// read complete next 8 bits
		result = stream.getNextChr();

		if(DEBUG){
			std::string code = leaf->_code.append(result).toStr();

			std::cout << "Chr:" << result << " Code:" << code << std::endl;
			printCodes();
		}
		// update Huffman tree
		getCode(result);

		return result;
	}else{
		result = leaf->_character;
		if(DEBUG){
			std::string code = leaf->_code.toStr();

			std::cout << "Chr:" << result << " Code:" << code << std::endl;
			printCodes();
		}
		// update Huffman tree
		getCode(result);
		return result;
	}
}

/**
 * This functions reads the bitstream stream until it finds a leaf and returns
 * the leaf node
 */
Node* AdaptiveHuffmanTree::findLeaf(Node* root,Bitstream& stream){
	if(root->_left ==nullptr && root->_right == nullptr){
		return root;
	}else{
		unsigned char nextBit = stream.getNextBit();

		if(nextBit)
			return findLeaf(root->_right,stream);
		else
			return findLeaf(root->_left,stream);
	}
}


