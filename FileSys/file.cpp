#include "file.h"




BitMap::BitMap(Block b) :
	bitMapContainer{ b },
	leastSignificantBits{ BytesToInt(b[0], b[1], b[2], b[3]) },
	mostSignificantBits{ BytesToInt(b[4], b[5], b[6], b[7]) }
{

}

void BitMap::setBit(int bitNum, bool val){

	//Set 0 to 1
	if (val){

		if (bitNum < 32 && bitNum >= 0){
			leastSignificantBits |= (int)pow(2, bitNum);
		}

		else if (bitNum < 64 && bitNum > 31){
			mostSignificantBits |= (int)pow(2, bitNum % 32);
		}
	}

	//Set 1 to 0
	else{

		if (bitNum < 32 && bitNum >= 0){
			leastSignificantBits &= ~(int)pow(2, bitNum);
		}

		else if (bitNum < 64 && bitNum > 31){
			mostSignificantBits &= ~(int)pow(2, bitNum % 32);
		}

	}


}

bool BitMap::getBitAt(int bitNum)
{

	if (bitNum < 32 && bitNum >= 0){
		return (bool)(leastSignificantBits & (int)pow(2, bitNum));
	}

	else if (bitNum < 64 && bitNum > 31){
		return (bool)(mostSignificantBits & (int)pow(2, bitNum % 32));
	}


}

int BitMap::getNextOpenBit()
{
	for (int i = 0; i < 64; i++){
		if (!getBitAt(i))
			return i;
	}

	return -1;

}

void BitMap::printMap(){

	std::bitset<32> a(leastSignificantBits);
	std::bitset<32> b(mostSignificantBits);

	std::cout << b << " " << a << std::endl;
}

Block::Block()
{
	for (int i = 0; i < 64; i++){
		mem[i] = 0;
	}
}

byte& Block::operator[](int i)
{
	try{

		if (i > 63 || i < 0)
			throw std::out_of_range("Index out of range");

		return mem[i];
	}
	catch (std::out_of_range o)
	{
		std::cout << o.what();
		
	}
	
}

void LDisk::read_block(int i, Block& p){

	//Read block i into block at pointer p
	p = disk[i];


}

void LDisk::write_block(int i, Block& p){

	//Write block at pointer p into block i
	disk[i] = p;


}

OFTEntry::OFTEntry() :
	buf{},
	position{ 0 },
	descriptorIndex{ 0 },
	fileLength{ 0 }
{}


unsigned int BytesToInt(unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4)
{
	unsigned int a{ b1 };
	unsigned int b{ b2*(unsigned int)pow(2, 8) };
	unsigned int c{ b3*(unsigned int)pow(2, 16) };
	unsigned int d{ b4*(unsigned int)pow(2, 24) };

	return (a + b + c + d);

}

/*Test Drivers*/

void BIT_MAP_TEST(){

	//BIT MAP TEST DRIVER

	/*Test Cases
	1. Set bit i to 1 or 0 : CHECK
	2. Get next free bit : CHECK
	3. Create bitmap from block : CHECK
	*/

	Block b1;

	b1[0] = 1;
	b1[1] = 1;
	b1[2] = 1;
	b1[3] = 1;

	std::cout << "This should be 16843009: " << BytesToInt(b1[0], b1[1], b1[2], b1[3]) << std::endl;

	b1[4] = 1;
	b1[5] = 1;
	b1[6] = 1;
	b1[7] = 1;


	BitMap map(b1);

	map.printMap();


	std::cout << map.getNextOpenBit() << std::endl;

	map.setBit(0, true);
	map.setBit(1, true);
	map.setBit(2, true);
	map.setBit(3, true);
	map.setBit(11, true);
	map.setBit(12, true);

	std::cout << map.getNextOpenBit() << std::endl;

	map.setBit(15, true);
	map.setBit(18, true);
	map.setBit(22, true);
	map.setBit(23, true);
	map.setBit(21, true);
	map.setBit(34, true);
	map.setBit(36, true);
	map.setBit(38, true);
	map.setBit(52, true);

	map.printMap();

	std::cout << map.getNextOpenBit() << std::endl;


	map.setBit(15, false);
	map.setBit(18, false);
	map.setBit(22, false);
	map.setBit(23, false);
	map.setBit(21, false);
	map.setBit(34, false);
	map.setBit(36, false);
	map.setBit(38, false);
	map.setBit(52, false);

	map.printMap();

	std::cout << map.getNextOpenBit() << std::endl;


}