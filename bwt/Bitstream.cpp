/*
 * Bitstream.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: till
 */

#include "Bitstream.hpp"

#include <iostream>

Bitstream::Bitstream(FILE* file, int bufferSize): _file(file),_bufferSize(bufferSize){
	_counter = 0;
	_buffer = new unsigned char[bufferSize];
	_max =0;
	_fileEmpty = false;
}

Bitstream::~Bitstream(){
	fclose(_file);
	if(_buffer != nullptr){
		delete [] _buffer;
	}
}

bool Bitstream::hasNextBit(){
	if(_counter >= _max){
		reloadBuffer();
	}

	return _counter < _max;
}

void Bitstream::reloadBuffer(){
	if(!_fileEmpty){
		_counter = 0;
		_max = fread(_buffer,1,_bufferSize,_file);

		if(_max <=0){
			_fileEmpty = true;
		}else{
			_max *= 8;
		}
	}
}

unsigned char Bitstream::getNextBit(){
	if(_counter >= _max){
		reloadBuffer();
	}

	if(_counter < _max){
		unsigned char result =  (_buffer[_counter/8] >> (7-_counter%8))&1;
		_counter++;
		return result;
	}else{
		return 255;
	}
}

unsigned char Bitstream::getNextChr(){
	unsigned char result = 0;

	if(_counter >= _max){
		reloadBuffer();
	}

	if(_counter < _max){
		if(_counter+8 <= _max){
			if(_counter%8==0){
				result = _buffer[_counter/8];
			}else{
				result = _buffer[_counter/8] << (_counter %8);
				result |= _buffer[_counter/8+1] >> (8-(_counter%8));
			}
			_counter+=8;
		}else{
			result = _buffer[_counter/8] << (_counter %8);
			int diff = _counter %8;
			reloadBuffer();

			if(_counter < _max){
				result |= _buffer[0] >> (8-diff);
			}else{
				std::cerr << "Could not read next character." << std::endl;
				exit(1);
			}
			_counter = diff;
		}
	}else{
		std::cerr << "Could not read next character." << std::endl;
		exit(1);
	}

	return result;
}


