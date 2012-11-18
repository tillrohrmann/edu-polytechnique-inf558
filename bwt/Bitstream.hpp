/*
 * Bitstream.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: till
 */

#ifndef BITSTREAM_HPP_
#define BITSTREAM_HPP_

#include <cstdio>

class Bitstream{
private:
	unsigned char* _buffer;
	FILE* _file;
	int _counter;
	int _max;
	int _bufferSize;
	bool _fileEmpty;

	void reloadBuffer();
public:
	Bitstream(FILE* file, int bufferSize);
	~Bitstream();

	unsigned char getNextBit();
	unsigned char getNextChr();
	bool hasNextBit();
};




#endif /* BITSTREAM_HPP_ */
