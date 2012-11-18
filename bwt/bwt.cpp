/*
 * bwt.cpp
 *
 *  Created on: Nov 17, 2012
 *      Author: till
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <cstdio>
#include <unistd.h>

#include "common.hpp"

const int MAX_BLOCKSIZE = 20000000;

int compareSuffices(unsigned char* buffer, int buflen,int a, int b){
	while(a < buflen && b < buflen && buffer[a] == buffer[b]){
		b++;
		a++;
	}

	if(a==buflen){
		return 1;
	}else if(b == buflen){
		return -1;
	}else{
		return buffer[a]-buffer[b];
	}
}

int qsortBWT(int* indices,unsigned char* buffer, int buflen,int left,int right){
	if(right-left <1){
		return left;
	}else{
		int pivot =left;
		int l = left+1;
		int r = right;

		while(true){
			while(l < right && compareSuffices(buffer,buflen,indices[l],indices[pivot]) <= 0){
				l++;
			}

			while(r > left && compareSuffices(buffer,buflen,indices[pivot],indices[r]) < 0){
				r--;
			}

			if(l < r){
				int swap = indices[l];
				indices[l] = indices[r];
				indices[r] = swap;
			}else{
				break;
			}
		}

		if(r != pivot){
			int swap = indices[pivot];
			indices[pivot] = indices[r];
			indices[r] = swap;
		}

		qsortBWT(indices,buffer,buflen,left,r-1);
		qsortBWT(indices,buffer,buflen,r+1,right);

		return r;
	}
}

int bwt(unsigned char* buffer, int buflen,unsigned char* output){
	int* indices = new int[buflen];
	int firstline =0;

	for(int i=0; i< buflen; i++){
		indices[i] = i;
	}

	firstline = qsortBWT(indices,buffer,buflen,0,buflen-1);

	for(int i=0; i< buflen;i++){
		if(indices[i] == 0){
			output[i] = buffer[buflen-1];
		}else{
			output[i] = buffer[indices[i]-1];
		}
	}

	delete [] indices;
	return firstline;
}

void transformFile(const std::string& filename, int blocksize){
	int actualSize=0;
	unsigned char* buffer = new unsigned char[blocksize+1];
	unsigned char* output = new unsigned char[blocksize+1];
	int firstline =0;

	FILE* file = fopen(filename.c_str(),"rb");

	if(file == nullptr){
		std::cerr << "Could not open file:" << filename<< std::endl;
		exit(1);
	}


	fwrite(&blocksize,sizeof(int),1,stdout);

	while((actualSize=readDataBlock(file,buffer,blocksize)) >0){
		firstline = bwt(buffer,actualSize,output);

		fwrite(&firstline,sizeof(int),1,stdout);
		fwrite(output,1,actualSize,stdout);
	}

	fclose(file);
	delete [] buffer;
	delete [] output;
}

int main(int argc, char** argv){
	std::string filename = "";
	int blocksize = 0;

	if(argc < 2){
		std::cout << "Usage: <input file> <block-size>" << std::endl;
		exit(1);
	}else{
		filename = argv[1];

		if(argc > 2){
			blocksize = atoi(argv[2]);
		}else{
			blocksize = MAX_BLOCKSIZE;
		}
	}

	transformFile(filename, blocksize);

	return 0;
}


