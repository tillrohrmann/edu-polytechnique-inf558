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

/**
 * This function calculates the inverse burrows wheeler transformation on the data
 * given in buffer. The result is saved in output.
 */
void unbwt(int firstline,unsigned char* buffer, int buflen,unsigned char* output){
	// saves the cumulative sum
	int cSum[257];
	// saves the number of identical characters which appear before the ith element of the buffer
	int *predecessors = new int[buflen];
	// saves the mapping of the resulting permutation
	int *transformation = new int[buflen];
	int index =0;

	for(int i=0; i < 257;i++){
		cSum[i] = 0;
	}

	// calculate the number of characters buffer[i] before the ith element
	for(int i =0; i < firstline; i++){
		predecessors[i] = cSum[buffer[i]+1]++;
	}

	for(int i =firstline+1; i < buflen;i++){
		predecessors[i] = cSum[buffer[i]+1]++;
	}

	/*
	 * since there is no real end of string character, the first line is replaced by
	 * the last line if one had a end of string character. Therefore one has to map the
	 * character of the first line to the last of its kind in the sorted string.
	 */
	predecessors[firstline] = cSum[buffer[firstline]+1]++;

	// calculate cumulative sum
	for(int i =1; i< 256;i++){
		cSum[i]+=cSum[i-1];
	}

	// calculate the permutation mapping
	for(int i=0; i < buflen;i++){

		index = cSum[buffer[i]] + predecessors[i];
		transformation[i] = index;
	}

	index = firstline;

	// retrieve the original string by following the permutation
	for(int i=0; i< buflen;i++){
		output[buflen-1-i] = buffer[index];
		index = transformation[index];
	}

	delete [] predecessors;
	delete [] transformation;
}

/**
 * This function takes a file and reads chunks of blocksize. On these chunks
 * it calls the inverse burrows wheeler transformation.
 */
void transformFile(const std::string & filename){
	FILE* file = fopen(filename.c_str(),"rb");
	int blocksize=0;
	unsigned char* buffer = nullptr;
	unsigned char* output = nullptr;
	int actualSize;
	int firstLine;
	int elemsRead;

	if(file == nullptr){
		std::cerr << "Could not open file:" << filename<< std::endl;
		exit(1);
	}

	// read blocksize of encoded file (always the first int in the file)
	elemsRead = fread(&blocksize,sizeof(int),1,file);

	if(elemsRead != 1){
		std::cerr << "Could not read the blocksize of the encoded block"  << std::endl;
		exit(1);
	}

	buffer = new unsigned char[blocksize];
	output = new unsigned char[blocksize];

	// read first line information (always at the beginning of a block)
	elemsRead = fread(&firstLine,sizeof(int),1,file);

	if(elemsRead != 1){
		std::cerr << "Could not read the firstline information of an encoded block" << std::endl;
		exit(1);
	}
	while((actualSize = readDataBlock(file,buffer,blocksize))>0){

		// calculate inverse burrows wheeler transformation
		unbwt(firstLine,buffer,actualSize,output);

		elemsRead = fread(&firstLine,sizeof(int),1,file);
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

