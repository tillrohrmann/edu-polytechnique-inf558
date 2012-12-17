/*
 * ahm.cpp
 *
 *  Created on: Nov 17, 2012
 *      Author: till
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <cstdio>
#include "AdaptiveHuffmanTree.hpp"

const int BLOCK_SIZE=20000;
const int OUTPUT_BUFFER_SIZE=10;

/**
 * This function performs the adaptive Huffman encoding on the file specified by filename
 */
void ahm(const std::string& filename){
	unsigned char buffer[BLOCK_SIZE];
	unsigned char outputBuffer[OUTPUT_BUFFER_SIZE];
	int bytesRead=0;
	FILE* file = fopen(filename.c_str(),"rb");
	AdaptiveHuffmanTree tree;
	CodeObject rest;

	if(file==nullptr){
		std::cerr<<"Could not open file:" << filename << std::endl;
		exit(1);
	}

	fseek(file,0,SEEK_END);
	int numberCharacters = ftell(file);
	fseek(file,0,SEEK_SET);

	fwrite(&numberCharacters,sizeof(int),1,stdout);
	int counter =0;

	while((bytesRead = fread(buffer,1,BLOCK_SIZE,file))>0){
		for(int i =0; i < bytesRead; i++){
			// encode current character
			CodeObject codeObject = tree.getCode(buffer[i]);
			// print rest.lengh/8 bytes of the code and keep the rest
			rest= codeObject.printCode(rest);
			fflush(stdout);
		}
	}
	//print eventually the rest
	rest.printRest();


	fclose(file);
}

int main(int argc, char** argv){
	std::string filename="";

	if(argc< 2){
		std::cout << "Usage: <input-file>" << std::endl;
		exit(1);
	}
	filename = argv[1];

	ahm(filename);

	return 0;
}


