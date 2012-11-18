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
			/*if(counter == 35756)
				std::cout << "get Code:" << counter << std::endl;*/
			CodeObject codeObject = tree.getCode(buffer[i]);
			//std::cout << "Print character:" << counter++ << " of " << numberCharacters << std::endl;
			rest= codeObject.printCode(rest);
			fflush(stdout);
		}
	}
	//std::cout << "Print rest" << std::endl;
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


