/*
 * CodeObject.hpp
 *
 *  Created on: Nov 18, 2012
 *      Author: till
 */

#ifndef CODEOBJECT_HPP_
#define CODEOBJECT_HPP_

#include <string>


class CodeObject{
private:
	unsigned char* _data;
	int _numbits;

public:
	CodeObject();
	CodeObject(unsigned char* data, int numbits);
	CodeObject(unsigned char data, int numbits);
	CodeObject(const CodeObject& ref);
	CodeObject(CodeObject&& ref);
	~CodeObject();

	const CodeObject& operator=(const CodeObject& b);
	CodeObject& operator=(CodeObject&& b);

	CodeObject printCode(const CodeObject& rest) const;

	CodeObject appendOne();
	CodeObject appendZero();
	CodeObject append(unsigned char ch);


	void printRest() const;

	int numbits() const{
		return _numbits;
	}

	unsigned char* data(){
		return _data;
	}

	int numbytes() const{
		return (_numbits+7)/8;
	}

	std::string toStr() const;
};



#endif /* CODEOBJECT_HPP_ */
