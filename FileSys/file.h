#ifndef FILE_H
#define FILE_H


#include <exception>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <bitset>
#include <vector>
#include <list>
#include <deque>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <ios>

typedef unsigned char byte;
typedef std::string str;
typedef std::list<byte>::iterator ITERATOR;


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

	void printContents();

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

	int readFile(int index, std::deque<byte>& v, int numBytes);
	int writeFile(int index, std::deque<byte>& v, int numBytes);
	

	OFTEntry getOFTEntryAt(int index);
	void setOFTEntryAt(int index, const Block& block, int position, int descriptorIndex, int fileLength);

	void setBlockAt(int index, const Block& block);
	void setFileLengthAt(int index, int fileLength);
	void setDescriptorIndexAt(int index, int descriptorIndex);
	void setPositionAt(int index, int position);
	
	int getFileLengthAt(int index);
	int getDescriptorIndexAt(int index);
	int getPositionAt(int index);
	Block& getBlockRefAt(int index);

	int getLowestFreeIndex(); // returns -1 if none found


	OpenFileTable();




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
	void loadMapToContainer(const Block& map);
	void setBit(int i, bool val);	
	int getNextOpenBit();			//Returns -1 if an open bit is not found

	void setLeastSignificantBits(int value);
	void setMostSignificantBits(int value);


	Block getBitMapBlock(){ return bitMapContainer; }
	void printMap();

	BitMap();
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
	void setLengthAt(int index, int length);
	void setBLock1At(int index, int block1);
	void setBLock2At(int index, int block2);
	void setBLock3At(int index, int block3);




	FileDescriptorHandel GetDescriptorHandelAt(int num) const { return descriptorList[num]; }

	int GetFreeDescriptorIndex() const; //returns -1 if no free index found

	



};



class SystemSimulation
{
private:
	LDisk mainDisk;
	DescriptorBank descriptorBank;
	OpenFileTable oft;
	BitMap bitMap;

	 




public:

	std::stringstream ss;




	void printDirectory();
	int getFileDescriptorIndex(const char* name);
	void addFileToDirectory(const char* name, int descriptorIndex);
	void removeFromDirectory(const char* name);

	void allocateSecondBlock(int descriptorIndex);
	void allocateThirdBlock(int descriptorIndex);
	void swapBlocks(int openFileIndex, int BlockX, int BlockY);


	int lseek(int index, int position);
	int read(int index, std::deque<byte>& mem, int numBytes);
	int write(int index, std::deque<byte>& mem, int numBytes);
	int close(int index);
	int save(std::string fileName);
	int init();
	int init(std::string fileName);
	int open(const char* name);
	int create(const char* name); //returns -1 if error
	int destroy(const char* name); // don't forget to dealocate directory blocks when the directory gets smaller




	SystemSimulation();
	SystemSimulation(std::string fileName);
	

};

int BytesToInt(byte b1, byte b2, byte b3, byte b4);

void IntToBytes(int num, byte& b1, byte& b2, byte& b3, byte& b4);

void PrintDescriptorHandel(FileDescriptorHandel f);





/*Test Drivers*/
void BIT_MAP_TEST();
void BYTES_TO_INT_TEST();
void INT_TO_BYTES_TEST();
void DESCRIPTOR_BANK_TEST();
void OPEN_FILE_TABLE_TEST();
void SYSTEM_SIMULATION_TEST();






#endif