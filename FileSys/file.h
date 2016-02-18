#ifndef FILE_H
#define FILE_H


#include <exception>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <bitset>
#include <vector>

typedef unsigned char byte;

//Block containing 64 bytes
class Block {
private:

	byte mem[64];

public:

	byte& operator[](int i);
	Block();
	Block(byte allBytes);
	Block(const Block& B);
	Block& operator=(const Block& right);

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


class OpenFileTable
{
private:
	OFTEntry oft[4];

	
public:

	int readFile(int index, std::vector<byte>& v, int numBytes);
	int writeFile(int index, const std::vector<byte>& v, int numBytes);
	void lseek(int index, int position);

	void printDirectory();
	int getFileDescriptorIndex(char* name);
	void addFileToDirectory(char* name, int descriptorIndex);


	OpenFileTable();




};

class DirectoryHandeler
{



};



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

	FileDescriptorHandel();

};


class DescriptorBank{

private:
	FileDescriptorHandel descriptorList[24];
	Block descriptorBlocks[6];

	

public:
	
	void SetDesctriptorBlock(const int blockNum, const Block& b){ descriptorBlocks[blockNum] = b; }
	Block GetDescriptorBlockAt(int num) const { return descriptorBlocks[num]; }

	void BuildDescriptorList();
	void WriteDescriptorListToBlocks();

	void SetDescriptorHandelAt(int index, int length, int block1 =-1, int block2 = -1, int block3 = -1);
	FileDescriptorHandel GetDescriptorHandelAt(int num) const { return descriptorList[num]; }

	int GetFreeDescriptorIndex() const; //returns -1 if no free index found

	



};

int BytesToInt(byte b1, byte b2, byte b3, byte b4);

void IntToBytes(int num, byte& b1, byte& b2, byte& b3, byte& b4);

void PrintDescriptorHandel(FileDescriptorHandel f);



/*Test Drivers*/
void BIT_MAP_TEST();
void BYTES_TO_INT_TEST();
void INT_TO_BYTES_TEST();
void DESCRIPTOR_BANK_TEST();






#endif