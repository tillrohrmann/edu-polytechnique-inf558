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

bool AdaptiveHuffmanTree::updateNode(Node* node){
	if(node == nullptr){
		return false;
	}
	if(node->_parent == nullptr){
		node->_weight++;
		return false;
	}else{
		Node* pre = _gallager[node->_index-1];

		if(pre->_weight > node->_weight){
			node->_weight++;

			return updateNode(node->_parent);
		}else{
			int index = node->_index-2;

			while(index >=0 && _gallager[index]->_weight <= node->_weight)
				index--;

			pre = _gallager[index+1];

			bool nodeLeft = node->_parent->_left == node;
			bool preLeft = pre->_parent->_left == pre;

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

			updateCode(pre);

			_gallager[node->_index] = pre;
			_gallager[pre->_index] = node;

			int iswap = pre->_index;
			pre->_index = node->_index;
			node->_index = iswap;

			node->_weight++;

			if(!updateNode(node->_parent)){
				updateCode(node);
			}

			return true;
		}
	}
}

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

CodeObject AdaptiveHuffmanTree::getCode(unsigned char ch){
	std::map<unsigned char,Node*>::iterator it;
	if((it=_mapping.find(ch)) != _mapping.end()){
		Node* node = it->second;

		CodeObject result = node->_code;

		updateNode(node);
		if(DEBUG){
			std::cout << std::endl << std::endl << "Old Chr:" << ch << " int: " << (unsigned int)ch << " Code:" << result.toStr() << std::endl << std::endl;
			printCodes();
		}


		return result;
	}else{
		Node* oldSignNode = _gallager[_gallager.size()-1];

		oldSignNode->_isNS = false;

		Node* newSignNode = new Node(oldSignNode,nullptr,nullptr,'?',1,_gallager.size()+1,true);
		Node* newChNode = new Node(oldSignNode,nullptr,nullptr,ch,1,_gallager.size());

		_gallager.push_back(newChNode);
		_gallager.push_back(newSignNode);

		oldSignNode->_left = newChNode;
		oldSignNode->_right = newSignNode;

		CodeObject result = oldSignNode->_code.append(ch);

		updateNode(oldSignNode);
		updateCode(newChNode);
		updateCode(newSignNode);

		if(DEBUG){
			std::cout << std::endl << std::endl <<  "New Chr:" << ch << " int: " << (unsigned int)ch << " Code:" << result.toStr() << std::endl << std::endl;
			printCodes();
		}

		_mapping[ch] = newChNode;

		return result;
	}
}

void AdaptiveHuffmanTree::printCodes(){
	for(std::vector<Node*>::iterator it = _gallager.begin(); it != _gallager.end();it++){
		std::cout << "Chr:" << (*it)->_character << " code:" << (*it)->_code.toStr() << " weight:" << (*it)->_weight<<std::endl;
	}
}

unsigned char AdaptiveHuffmanTree::getNextChr(Bitstream& stream){
	Node * leaf = findLeaf(_root,stream);
	unsigned char result;
	if(leaf == nullptr){
		std::cerr << "Error" << std::endl;
		exit(1);
	}

	if(leaf->_isNS){
		result = stream.getNextChr();

//		if(DEBUG){
//			std::string code = leaf->_code.append(result).toStr();
//
//			std::cout << "Chr:" << result << " Code:" << code << std::endl;
//			printCodes();
//		}
		getCode(result);

		return result;
	}else{
		result = leaf->_character;
//		if(DEBUG){
//			std::string code = leaf->_code.toStr();
//
//			std::cout << "Chr:" << result << " Code:" << code << std::endl;
//			printCodes();
//		}
		getCode(result);
		return result;
	}
}

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


