/*
 * mtf.cpp
 *
 *  Created on: Nov 17, 2012
 *      Author: till
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>

const int BLOCK_SIZE=20000;

void mtf(const std::string& filename){
	FILE*file = fopen(filename.c_str(),"rb");
	unsigned char buffer[BLOCK_SIZE];
	int bytesRead =0;
	unsigned char code[256];
	unsigned char chCode;

	if(file == nullptr){
		std::cerr << "Could not open file:" << filename<< std::endl;
		exit(1);
	}


	for(int i=0; i< 256;i++){
		code[i] = i;
	}

	while((bytesRead = fread(buffer,1,BLOCK_SIZE,file)) > 0){
		for(int i =0; i< bytesRead;i++){
			chCode= 0;

			while(chCode <256 && code[chCode] != buffer[i]){
				chCode++;
			}

			fwrite(&chCode,1,1,stdout);

			for(int j=chCode; j >0;j--){
				code[j] = code[j-1];
			}
			code[0] = buffer[i];
		}
	}

	fclose(file);
}

int main(int argc, char** argv){
	std::string filename ="";

	if(argc < 2){
		std::cout <<"Usage: <input-file>"<< std::endl;
		exit(1);
	}

	filename = argv[1];

	mtf(filename);

	return 0;
}
