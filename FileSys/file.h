#ifndef FILE_H
#define FILE_H


#include <exception>
#include <stdexcept>
#include <iostream>

typedef char byte;


class LDisk{
private:

	Block disk[64];
	BitMap bitMap;


public: 


};



class Block {
private:

	byte mem[64];

public:

	byte& operator[](int i); 

};

class Directory{
private:
	//Pointer to the file descriptor in the LDisk
	Block* directoryFileDescriptor;


public:

	//Print the directory
	void print();

	//Return descriptor index
	int file(std::string fileName);

	void addFile(std::string fileName);



};
 
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
	Block* bitBlock;


public:

	//Bit manipulators
	void setBit(int i, bool val);
	bool getBit(int i);

	//Next four free bits in the bit map
	char freeBits[4];

	BitMap();
	

};

class DescriptorBank
{
public:

	FileDescriptor& getFileDescriptor(int i);





};

struct FileDescriptor
{
	byte* length;			//File length
	byte* fileSegment[3];	//Location of each file segment
};








#endif