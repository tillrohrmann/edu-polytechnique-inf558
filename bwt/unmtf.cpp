/*
 * unmtf.cpp
 *
 *  Created on: Nov 17, 2012
 *      Author: till
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>

const int BLOCK_SIZE=20000;

/**
 * This function performs the inverse move to front transformation on the file specified by filename
 */
void unmtf(const std::string& filename){

	FILE *file = fopen(filename.c_str(),"rb");
	unsigned char code[256];
	unsigned char buffer[BLOCK_SIZE];
	int bytesRead;
	unsigned char oldCode;

	if(file == nullptr){
		std::cerr << "Could not open file:" << filename<< std::endl;
		exit(1);
	}

	// initialize code table
	for(int i =0; i< 256; i++){
		code[i] = i;
	}

	// read bytes until eof
	while((bytesRead = fread(buffer,1,BLOCK_SIZE,file))>0){
		for(int i =0; i< bytesRead; i++){

			// buffer[i] contains index of character in code
			fwrite(code+buffer[i],1,1,stdout);

			oldCode = code[buffer[i]];

			// move oldCode to the front
			for(int j = buffer[i]; j> 0;j--){
				code[j] = code[j-1];
			}

			code[0] = oldCode;
		}
	}

	fclose(file);
}


int main(int argc, char** argv){
	std::string filename = "";

	if(argc< 2){
		std::cout <<"Usage: <input-file>" << std::endl;
		exit(1);
	}

	filename = argv[1];

	unmtf(filename);

	return 0;
}


