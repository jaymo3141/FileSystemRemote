#ifndef FILE_H
#define FILE_H


#include <exception>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <bitset>

typedef unsigned char byte;

//Block containing 64 bytes
class Block {
private:

	byte mem[64];

public:

	byte& operator[](int i);
	Block();

};


//Logical Disk
class LDisk{
private:

	Block disk[64];
	


public: 
	void read_block(int i, Block& p);
	void write_block(int i, Block& p);


};

//Open File Table
struct OFTEntry
{
	Block buf;
	int position;
	int descriptorIndex;
	int fileLength;

	OFTEntry();
};


//class OpenFileTable
//{
//private:
//	OFTEntry OpenFileTable[4];
//
//
//
//};



class BitMap
{
private:

	////Mask used to manipulate bits
	//const unsigned char MASK[8] = {

	//	0x01,	//Bit 0
	//	0x02,	//Bit 1
	//	0x04,	//Bit 2
	//	0x08,	//Bit 3
	//	0x10,	//Bit 4
	//	0x20,	//Bit 5
	//	0x40,	//Bit 6
	//	0x80,	//Bit 7

	//};

	
	

	
	Block bitMapContainer;
	int leastSignificantBits;
	int mostSignificantBits;

	bool getBitAt(int bitNum);


public:

	void saveMapToContainer();
	void setBit(int i, bool val);	
	int getNextOpenBit();			//Returns -1 if an open bit is not found

	Block getBitMapBlock(){ return bitMapContainer; }
	void printMap();

	BitMap(Block b);
	

};

struct FileDescriptorHandel
{
	int length;
	int block1;
	int block2;
	int block3;

};

unsigned int BytesToInt(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4);

/*Test Drivers*/
void BIT_MAP_TEST();






#endif