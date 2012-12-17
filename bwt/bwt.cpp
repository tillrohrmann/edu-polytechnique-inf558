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


/**
 * This functions compares two rotations of the same string.
 * The one string is starting at index a and the other is starting at index b.
 * If one index reaches the value buflen, then this string is bigger than the other
 * string, because of a virtual end of string character which has the highest value.
 *
 * @return int -1 if str_a < str_b, 0 if str_a == str_b, 1 if str_a > str_b
 */
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

/**
 * This function sorts the different rotations of the burrows wheeler transformation.
 * For the purpose of efficiency one uses the same buffer just with different starting
 * indices to represent the different rotations. These indices are stored in indices.
 * Moreover one inserts a virtual end of string character which has the highest value.
 * If the comparison operation encounters the virtual end of string character (which is unique)
 * then the string for which the character was detected is bigger than the other.
 *
 * @return int index of the unsorted firstline
 */
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


/**
 * This function performs the burrows wheeler transformation on the data in
 * buffer and saves the transformed data into output. It returns the index of the first
 * line
 *
 * @return int index of the first line
 */
int bwt(unsigned char* buffer, int buflen,unsigned char* output){
	int* indices = new int[buflen];
	int firstline =0;

	//initialize the starting points of the different rotations
	for(int i=0; i< buflen; i++){
		indices[i] = i;
	}

	// sort the different rotations
	firstline = qsortBWT(indices,buffer,buflen,0,buflen-1);

	// get last column of the sorted table of rotations
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

/**
 * This function calls the burrows wheeler transformation on a block
 * of the size blocksize of the file specified by filename
 *
 * @param string filename
 * @param int blocksize blocksize in bytes
 */
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


	// write first blocksize of burrows wheeler transformation
	fwrite(&blocksize,sizeof(int),1,stdout);

	// as long as there is more data left to be encoded continue
	while((actualSize=readDataBlock(file,buffer,blocksize)) >0){
		firstline = bwt(buffer,actualSize,output);

		// write index of the first line (necessary for the reconstruction of the data)
		fwrite(&firstline,sizeof(int),1,stdout);
		// write transformed data
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


