#include "file.h"


OpenFileTable::OpenFileTable()	
{
	for (int i = 0; i < 4; i++)
	{
		oft[i].descriptorIndex = -1;
		oft[i].fileLength = -1;
		oft[i].position = -1;
	}
}

int OpenFileTable::readFile(int index, std::list<byte>& data, int numBytes)
{

	int bytesRead{ 0 };

	//read numBytes from the buf into v
	for (int i = 0; i < numBytes; i++)
	{
		if (oft[index].position < oft[index].fileLength)
		{
			data.push_back(oft[index].buf[ (oft[index].position) % 64 ]);
			oft[index].position++;
			bytesRead++;
		}

		else
			break;
	}
	
	return bytesRead;
}

int OpenFileTable::writeFile(int index,std::list<byte>& data, int numBytes)
{
	int bytesWritten{ 0 };

	//wrtie numBytes from the data into buf
	for (int i = 0; i < numBytes; i++)
	{
		if (oft[index].position < oft[index].fileLength)
		{
			oft[index].buf[(oft[index].position) % 64] = data.front();
			data.pop_front();
			oft[index].position++;
			bytesWritten++;
		}

		else
			break;
	}

	return bytesWritten;
}

OFTEntry OpenFileTable::getOFTEntryAt(int index)
{
	return oft[index];
}

void OpenFileTable::setOFTEntryAt(int index, const Block& block, int position, int descriptorIndex, int fileLength)
{
	oft[index].buf = block;
	oft[index].position = position;
	oft[index].descriptorIndex = descriptorIndex;
	oft[index].fileLength = fileLength;
}

void OpenFileTable::lseek(int index, int position)
{
	oft[index].position = position;
}

FileDescriptorHandel::FileDescriptorHandel() :
	length{ -1 },
	block1{ -1 },
	block2{ -1 },
	block3{ -1 }

{

}

void DescriptorBank::BuildDescriptorList()
{
	for (int i = 0, k = 0; i < 6; i++){
		for (int j = 0, k = 0; j < 63; j+=16, k++){

			descriptorList[k].length = BytesToInt(descriptorBlocks[i][j], descriptorBlocks[i][j + 1], descriptorBlocks[i][j + 2], descriptorBlocks[i][j + 3]);
			descriptorList[k].block1 = BytesToInt(descriptorBlocks[i][j + 4], descriptorBlocks[i][j + 5], descriptorBlocks[i][j + 6], descriptorBlocks[i][j + 7]);
			descriptorList[k].block2 = BytesToInt(descriptorBlocks[i][j + 8], descriptorBlocks[i][j + 9], descriptorBlocks[i][j + 10], descriptorBlocks[i][j + 11]);
			descriptorList[k].block3 = BytesToInt(descriptorBlocks[i][j + 12], descriptorBlocks[i][j + 13], descriptorBlocks[i][j + 14], descriptorBlocks[i][j + 15]);

		}
	}



}

void DescriptorBank::WriteDescriptorListToBlocks()
{

	for (int i = 0, k = 0; i < 6; i++){
		for (int j = 0, k = 0; j < 63; j += 16, k++){

			IntToBytes(descriptorList[k].length,descriptorBlocks[i][j], descriptorBlocks[i][j + 1], descriptorBlocks[i][j + 2], descriptorBlocks[i][j + 3]);
			IntToBytes(descriptorList[k].block1, descriptorBlocks[i][j + 4], descriptorBlocks[i][j + 5], descriptorBlocks[i][j + 6], descriptorBlocks[i][j + 7]);
			IntToBytes(descriptorList[k].block2, descriptorBlocks[i][j + 8], descriptorBlocks[i][j + 9], descriptorBlocks[i][j + 10], descriptorBlocks[i][j + 11]);
			IntToBytes(descriptorList[k].block3, descriptorBlocks[i][j + 12], descriptorBlocks[i][j + 13], descriptorBlocks[i][j + 14], descriptorBlocks[i][j + 15]);

		}
	}

}

void DescriptorBank::SetDescriptorHandelAt(int index, int length, int block1, int block2, int block3)
{
	descriptorList[index].length = length;
	descriptorList[index].block1 = block1;
	descriptorList[index].block2 = block2;
	descriptorList[index].block3 = block3;

}

int DescriptorBank::GetFreeDescriptorIndex() const
{
	for (int i = 0; i < 24; i++)
	{	
		if (descriptorList[i].length == -1)
			return i;

	}

	return -1;


}


BitMap::BitMap(Block b) :
	bitMapContainer{ b },
	leastSignificantBits{ BytesToInt(b[0], b[1], b[2], b[3]) },
	mostSignificantBits{ BytesToInt(b[4], b[5], b[6], b[7]) }
{

}

void BitMap::saveMapToContainer()
{

	IntToBytes(mostSignificantBits, bitMapContainer[0], bitMapContainer[1], bitMapContainer[2], bitMapContainer[3]);
	IntToBytes(leastSignificantBits, bitMapContainer[4], bitMapContainer[5], bitMapContainer[6], bitMapContainer[7]);


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

Block::Block(const Block& B)
{
	for (int i = 0; i < 64; i++)
	{
		mem[i] = B.mem[i];
	}

}

Block::Block(byte allBytes)
{
	for (int i = 0; i < 64; i++)
	{
		mem[i] = allBytes;
	}
}

Block& Block::operator=(const Block& right)
{
	for (int i = 0; i < 64; i++)
	{
		mem[i] = right.mem[i];
	}

	return *this;
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


int BytesToInt(byte b1, byte b2, byte b3, byte b4)
{
	unsigned int a{ b1 };
	unsigned int b{ b2*(unsigned int)pow(2, 8) };
	unsigned int c{ b3*(unsigned int)pow(2, 16) };
	unsigned int d{ b4*(unsigned int)pow(2, 24) };

	return (a + b + c + d);

}

void IntToBytes(int num, byte& b1, byte& b2, byte& b3, byte& b4)
{
	b1 = (num >> 24);
	b2 = (num >> 16) & 255;
	b3 = (num >> 8) & 255;
	b4 = num & 255;

}

void PrintDescriptorHandel(FileDescriptorHandel f)
{

	std::cout << "Length: " << f.length << std::endl
		<< "Block1: " << f.block1 << std::endl
		<< "Block2: " << f.block2 << std::endl
		<< "Block3: " << f.block3 << std::endl << std::endl;

}

void printDirectory()
{}

int SystemSimulation::getFileDescriptorIndex(char* name)
{
	std::list<byte> cache;
	std::list<byte>::iterator it = cache.begin();

	oft.readFile(0, cache, 8);

	if (*(it++) == *name &&
		*(it++) == *(name + 1) &&
		*(it++) == *(name + 2) &&
		*(it++) == *(name + 3))
	{
		return BytesToInt(*(it++), *(it++), *(it++), *(it));
	}
	



}

void addFileToDirectory(char* name, int descriptorIndex)
{}

/*Test Drivers*/

void BIT_MAP_TEST(){

	//BIT MAP TEST DRIVER

	/*Test Cases
	1. Set bit i to 1 or 0 : CHECK
	2. Get next free bit : CHECK
	3. Create bitmap from block : CHECK
	*/

	std::cout << "ENTERING BIT_MAP_TEST" << std::endl;

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

	map.saveMapToContainer();

	Block b2 = map.getBitMapBlock();

	for (int i = 0; i < 8; i++){
		std::bitset<8> a = b2[i];

		std::cout << a << std::endl;
	}


	std::cout << "EXITING BIT_MAP_TEST" << std::endl; 



}

void BYTES_TO_INT_TEST()
{
	/*TEST CASES*/

	// 1. -1
	// 2. any positive number
	// 3. any other negative number



	std::cout << "ENTERING BYTES_TO_INT_TEST" << std::endl;

	byte zero = 0;

	byte b1{ 0xff };
	byte b2{ 0xff };
	byte b3{ 0xff };
	byte b4{ 0xff };

	std::cout <<"This should be -1: " << BytesToInt(b1, b2, b3, b4) << std::endl;


	std::cout << "EXITING BYTES_TO_INT_TEST" << std::endl << std::endl;



}

void INT_TO_BYTES_TEST()
{



}

void DESCRIPTOR_BANK_TEST()
{

	/*TEST CASES*/

	// 1. Set a descriptor and print it
	// 2. Print all the descriptors
	// 3. Write descriptors back to the block

	std::cout << "ENTERING DESCRIPTOR_BANK_TEST" << std::endl << std::endl;

	Block b1(0xff);
	Block b2(0xff);
	Block b3(0xff);
	Block b4(0xff);
	Block b5(0xff);
	Block b6(0xff);

	DescriptorBank db;

	db.SetDesctriptorBlock(0, b1);
	db.SetDesctriptorBlock(1, b2);
	db.SetDesctriptorBlock(2, b3);
	db.SetDesctriptorBlock(3, b4);
	db.SetDesctriptorBlock(4, b5);
	db.SetDesctriptorBlock(5, b6);


	db.BuildDescriptorList();

	FileDescriptorHandel testDescriptor = db.GetDescriptorHandelAt(1);

	PrintDescriptorHandel(testDescriptor);

	db.SetDescriptorHandelAt(0, 2, 15, 31);
	db.SetDescriptorHandelAt(1, 3, 7, 8, 3);
	db.SetDescriptorHandelAt(2, 2, 2, 4 );
	db.SetDescriptorHandelAt(3, 0);

	FileDescriptorHandel testDescriptor2 = db.GetDescriptorHandelAt(0);
	FileDescriptorHandel testDescriptor3 = db.GetDescriptorHandelAt(1);
	FileDescriptorHandel testDescriptor4 = db.GetDescriptorHandelAt(2);
	FileDescriptorHandel testDescriptor5 = db.GetDescriptorHandelAt(3);


	PrintDescriptorHandel(testDescriptor2);
	PrintDescriptorHandel(testDescriptor3);
	PrintDescriptorHandel(testDescriptor4);
	PrintDescriptorHandel(testDescriptor5);


	std::cout << "Free Descriptor index: " << db.GetFreeDescriptorIndex() << std::endl;

	db.WriteDescriptorListToBlocks();

	Block b7 = db.GetDescriptorBlockAt(0);

	for (int i = 0; i < 48; i++){
		std::cout << (int)b7[i] << std::endl;
	}
			
	std::cout << "EXITING DESCRIPTOR_BANK_TEST" << std::endl << std::endl;


}

void OPEN_FILE_TABLE_TEST()
{


	std::cout << "ENTERING OPEN_FILE_TABLE_TEST" << std::endl << std::endl;

	Block b1(127);
	Block b2(65);
	Block b3(15);
	Block b4(255);

	OpenFileTable oft;

	oft.setOFTEntryAt(0, b1, 0, 0, 64);
	oft.setOFTEntryAt(1, b2, 62, 2, 64);
	oft.setOFTEntryAt(2, b3, 126, 4, 64);
	oft.setOFTEntryAt(3, b4, 33, 5, 64);

	std::list<byte> testList;
	

	oft.readFile(0, testList, 10);

	std::cout << "Following numbers should be 127" << std::endl;

	for (auto it = testList.begin(); it != testList.end(); it++)
	{
		std::cout << (int)*it << std::endl;
	}

	testList.clear();



	std::cout << "EXITING OPEN_FILE_TABLE_TEST" << std::endl << std::endl;


}
