/*
 * common.hpp
 *
 *  Created on: Nov 17, 2012
 *      Author: till
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <stdio.h>

/**
 * Reads from the file blocksize bytes and stores them in buffer
 *
 * @param FILE* file
 * @param unsigned char* buffer
 * @param int blocksize
 *
 * @return int number of bytes read
 */
int readDataBlock(FILE* file,unsigned char* buffer, int blocksize);

/**
 * Template for quicksosrt algorithm
 */
template<typename T>
void qsort(T* data,int left, int right){
	if(right - left >= 2){
		int pivot = left;
		int l = left+1;
		int r = right;

		while(true){
			while(l < right && data[l] <=  data[pivot]){
				l++;
			}
			while(r > left && data[pivot] < data[r]){
				r++;
			}

			if(l<r){
				T swap = data[l];
				data[l] = data[r];
				data[r] = swap;
			}else{
				break;
			}
		}

		if(pivot != r){
			T swap = data[pivot];
			data[pivot] = data[r];
			data[r] = swap;
		}

		qsort(data,left,r-1);
		qsort(data,r+1,right);
	}
}




#endif /* COMMON_HPP_ */
