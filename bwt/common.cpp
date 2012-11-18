/*
 * common.cpp
 *
 *  Created on: Nov 17, 2012
 *      Author: till
 */

#include "common.hpp"

int readDataBlock(FILE* file,unsigned char* buffer, int blocksize){
	if(feof(file)){
		return -1;
	}else{
		int bytesRead = fread(buffer,1,blocksize,file);
		return bytesRead;
	}
}



