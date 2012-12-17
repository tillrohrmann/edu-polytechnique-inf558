/*
 * CodeObject.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: till
 */


#include "CodeObject.hpp"

#include <cstring>
#include <cstdio>
#include <iostream>

CodeObject::CodeObject(){
	_data = nullptr;
	_numbits = 0;
}

CodeObject::CodeObject(unsigned char* data, int numbits){
	_data = data;
	_numbits = numbits;
}

CodeObject::CodeObject(unsigned char data, int numbits){
	_data = new unsigned char[1];
	*_data = data;
	_numbits = numbits;
}

CodeObject::CodeObject(const CodeObject& ref){
	_data = new unsigned char[ref.numbytes()];
	memcpy(_data,ref._data,ref.numbytes());
	_numbits = ref._numbits;
}

// constructor for rvalues
CodeObject::CodeObject(CodeObject&& ref){
	_data = ref._data;
	ref._data = nullptr;
	_numbits = ref._numbits;
	ref._numbits = 0;
}

CodeObject::~CodeObject(){
	if(_data != nullptr){
		delete [] _data;
	}

	_data = nullptr;
	_numbits = 0;
}


const CodeObject& CodeObject::operator=(const CodeObject& ref){
	if(_data != nullptr){
		delete [] _data;
	}

	_data = new unsigned char[ref.numbytes()];
	memcpy(_data,ref._data,ref.numbytes());
	_numbits = ref._numbits;

	return *this;
}

/**
 * Assignment operator for rvalues
 */
CodeObject& CodeObject::operator=(CodeObject&&ref){
	if(_data != nullptr){
		delete [] _data;
	}

	_data = ref._data;
	ref._data = nullptr;
	_numbits = ref._numbits;
	ref._numbits = 0;

	return *this;
}

void CodeObject::printRest() const{
	fwrite(_data,1,numbytes(),stdout);
}

/**
 * This function appends the code rest to the current code object. Then it
 * prints all full bytes of the code to stdout and returns the rest of the code.
 */
CodeObject CodeObject::printCode(const CodeObject& rest) const{

	if(_numbits==0 && rest._numbits ==0){
		return CodeObject();
	}else{
		int numberBytes = (_numbits + rest._numbits+7)/8;
		int numberFullBytes = (_numbits+rest._numbits)/8;
		unsigned char* buffer = new unsigned char[numberBytes];

		if(rest._data != nullptr)
			memcpy(buffer,rest._data,rest.numbytes());

		if(rest._numbits %8 ==0){
			for(int i = rest.numbytes();i<numberBytes;i++){
				buffer[i] = _data[i-rest.numbytes()];
			}
		}else if(_data!= nullptr){
			int diff = rest._numbits%8;
			buffer[rest.numbytes()-1] |= _data[0] >> diff;

			for(int i =rest.numbytes();i<numberBytes;i++){
				buffer[i] = _data[i-rest.numbytes()] << (8-diff);
				if(i-rest.numbytes() < numbytes()-1){
					buffer[i] |= _data[i-rest.numbytes()+1] >> diff;
				}
			}
		}

		if(numberFullBytes >0){
			fwrite(buffer,1,numberFullBytes,stdout);
			fflush(stdout);
		}
		unsigned restCode = buffer[numberFullBytes];
		delete [] buffer;

		if(numberFullBytes - numberBytes == 0){
			return CodeObject();
		}else{
			return CodeObject(restCode,(_numbits + rest._numbits)%8);
		}
	}

}

/**
 * This function appends a 1 to the current code and returns it as a new
 * CodeObject.
 */
CodeObject CodeObject::appendOne(){
	unsigned char * newdata = nullptr;
	if(_numbits%8==0){
		newdata=new unsigned char[numbytes()+1];
	}else{
		newdata = new unsigned char[numbytes()];
	}

	if(_data != nullptr)
		memcpy(newdata,_data,numbytes());

	if(_numbits%8 ==0){
		newdata[numbytes()] = 1 << 7;
	}else{
		newdata[numbytes()-1] |= 1 << (7 - _numbits%8);
	}

	return CodeObject(newdata,_numbits+1);
}

/**
 * This function appends a 0 to the current code and returns it as a new
 * CodeObject.
 */
CodeObject CodeObject::appendZero(){
	unsigned char * newdata = nullptr;
	if(_numbits%8==0){
		newdata=new unsigned char[numbytes()+1];
	}else{
		newdata = new unsigned char[numbytes()];
	}

	if(_data != nullptr)
		memcpy(newdata,_data,numbytes());


	if(_numbits%8 ==0){
		newdata[numbytes()] = 0;
	}else{
		newdata[numbytes()-1] &= ~(1 << (7-(_numbits%8)));
	}

	return CodeObject(newdata,_numbits+1);
}

/**
 * This function appends the 8 bits of ch to the current code and returns the
 * result as a new CodeObject
 */
CodeObject CodeObject::append(unsigned char ch){
	unsigned char* newdata = nullptr;
	newdata = new unsigned char[numbytes()+1];

	if(_data != nullptr){
		memcpy(newdata,_data,numbytes());
	}

	if(_numbits %8!=0){
		newdata[numbytes()-1] |= ch >> (_numbits%8);
		newdata[numbytes()] = ch << (8-_numbits%8);
	}else{
		newdata[numbytes()] = ch;
	}

	return CodeObject(newdata,_numbits+8);
}

/**
 * This function converts the current code to a string and returns it.
 */
std::string CodeObject::toStr() const{
	std::string result ="";

	int fullBytes = _numbits/8;

	for(int i=0; i< fullBytes;i++){
		for(int j =7; j>=0;j--){
			if((_data[i]>>j)&1){
				result += "1";
			}else{
				result +="0";
			}
		}
	}

	for(int j = 7; j>= (8-(_numbits%8));j--){
		if((_data[fullBytes]>>j)&1){
			result += "1";
		}else{
			result += "0";
		}
	}

	return result;
}

