#ifndef FILE_H
#define FILE_H


#include <exception>
#include <stdexcept>
#include <iostream>

typedef char byte;

class Block {
private:

	byte mem[64];

public:

	virtual byte& operator[](int i); 

};
 
class BitMap : public Block
{
private:
	const unsigned char MASK[8] = {

		0x01,	//Bit 0
		0x02,	//Bit 1
		0x04,	//Bit 2
		0x08,	//Bit 3
		0x10,	//Bit 4
		0x20,	//Bit 5
		0x40,	//Bit 6
		0x80,	//Bit 7
		
	};


public:

	//Bit manipulators
	void setBit(int i, bool val);
	bool getBit(int i);


	BitMap();
	

};

class DescriptorBank : public Block
{
public:

	FileDescriptor& getFileDescriptor(int i);





};

struct FileDescriptor
{
	byte* length;			//File length
	byte* fileSegment[3];	//Location of each file segment
};



class LDisk{
private:

	Block disk[64];


public:
};





#endif