#ifndef FILE_H
#define FILE_H


#include <exception>
#include <stdexcept>
#include <iostream>

typedef char byte;

Block cache[7];


//Block containing 64 bytes
class Block {
private:

	byte mem[64];

public:

	byte& operator[](int i);

};


//Logical Disk
class LDisk{
private:

	Block disk[64];
	


public: 
	void read_block(int i, Block *p);
	void write_block(int i, Block *p);


};

//Open File Table
typedef struct OFTEntry{
	Block buf;
	int position;
	int descriptorIndex;
	int fileLength;
}OFTEntry;

OFTEntry OpenFileTable[4];







//class Directory{
//private:
//	//Pointer to the file descriptor in the LDisk
//	Block* directoryFileDescriptor;
//
//
//public:
//
//	//Print the directory
//	void print();
//
//	//Return descriptor index
//	int file(std::string fileName);
//
//	void addFile(std::string fileName);
//
//
//
//};
// 


class BitMap
{
private:

	//Mask used to manipulate bits
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

	
	

	//Pointer to the first block in LDisk
	Block map;

	bool getBit(int i);


public:

	//Bit manipulators
	void setBit(int i, bool val);
	int getFreeBit();
	
	Block getMap();
	

	BitMap();
	

};

//
//class DescriptorBank
//{
//public:
//
//	FileDescriptor& getFileDescriptor(int i);
//
//
//
//
//
//};
//
//struct FileDescriptor
//{
//	byte* length;			//File length
//	byte* fileSegment[3];	//Location of each file segment
//};
//







#endif