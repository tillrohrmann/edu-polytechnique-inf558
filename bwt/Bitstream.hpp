/*
 * Bitstream.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: till
 */

#ifndef BITSTREAM_HPP_
#define BITSTREAM_HPP_

#include <cstdio>

/**
 * This class provides a bit level access to a file. It fills the buffer and
 * delivers one bit after another to the user. When the buffer gets empty it
 * refills it by reading another data block of the file.
 */
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

	// read 1 bit
	unsigned char getNextBit();
	// read 8 bit
	unsigned char getNextChr();
	// check whether there are other bits
	bool hasNextBit();
};




#endif /* BITSTREAM_HPP_ */
