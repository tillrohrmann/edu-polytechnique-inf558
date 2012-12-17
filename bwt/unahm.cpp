/*
 * unahm.cpp
 *
 *  Created on: Nov 17, 2012
 *      Author: till
 */

#include <iostream>
#include <string>
#include "AdaptiveHuffmanTree.hpp"

const int BUFFERSIZE=10;

const bool DEBUG = false;

/**
 * This functions performs the adaptive Huffman decoding on the file specified by
 * filename.
 */
void unahm(const std::string& filename){
	FILE* file = fopen(filename.c_str(),"rb");
	Bitstream stream(file,BUFFERSIZE);
	AdaptiveHuffmanTree tree;
	int numChars =0;

	if(file == nullptr){
		std::cerr << "Could not open file:" << filename << std::endl;
		exit(1);
	}

	// get number of characters of the result file
	fread(&numChars,sizeof(int),1,file);

	if(DEBUG){
		std::cout << "Num chars:" << numChars << std::endl;
	}

	for(int i =0; i< numChars;i++){
		//decode next char
		char ch = tree.getNextChr(stream);
		fwrite(&ch,1,1,stdout);
		fflush(stdout);
	}

}

int main(int argc, char** argv){
	std::string filename = "";

	if(argc < 2){
		std::cout << "Usage: <input-file>" << std::endl;
		exit(1);
	}

	filename = argv[1];

	unahm(filename);
	return 0;
}


