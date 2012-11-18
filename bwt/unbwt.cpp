/*
 * unbwt.cpp
 *
 *  Created on: Nov 17, 2012
 *      Author: till
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>

#include "common.hpp"

void unbwt(int firstline,unsigned char* buffer, int buflen,unsigned char* output){
	int cSum[257];
	int *predecessors = new int[buflen];
	int *transformation = new int[buflen];
	int index =0;

	for(int i=0; i < 257;i++){
		cSum[i] = 0;
	}


	for(int i =0; i < firstline; i++){
		predecessors[i] = cSum[buffer[i]+1]++;
	}

	for(int i =firstline+1; i < buflen;i++){
		predecessors[i] = cSum[buffer[i]+1]++;
	}

	predecessors[firstline] = cSum[buffer[firstline]+1]++;

	for(int i =1; i< 256;i++){
		cSum[i]+=cSum[i-1];
	}

	for(int i=0; i < buflen;i++){

		index = cSum[buffer[i]] + predecessors[i];
		transformation[i] = index;
	}

	index = firstline;

	for(int i=0; i< buflen;i++){
		output[buflen-1-i] = buffer[index];
		index = transformation[index];
	}

	delete [] predecessors;
	delete [] transformation;
}

void transformFile(const std::string & filename){
	FILE* file = fopen(filename.c_str(),"rb");
	int blocksize=0;
	unsigned char* buffer = nullptr;
	unsigned char* output = nullptr;
	int actualSize;
	int firstLine;

	if(file == nullptr){
		std::cerr << "Could not open file:" << filename<< std::endl;
		exit(1);
	}


	fread(&blocksize,sizeof(int),1,file);

	buffer = new unsigned char[blocksize];
	output = new unsigned char[blocksize];

	fread(&firstLine,sizeof(int),1,file);
	while((actualSize = readDataBlock(file,buffer,blocksize))>0){

		unbwt(firstLine,buffer,actualSize,output);

		fread(&firstLine,sizeof(int),1,file);
		fwrite(output,1,actualSize,stdout);
	}

	delete [] buffer;
	delete [] output;
}


int main(int argc, char** argv){

	std::string filename = "";

	if(argc < 2){
		std::cout << "Usage: <input-file>" << std::endl;
		exit(1);
	}else{
		filename = argv[1];
	}


	transformFile(filename);
	return 0;
}

