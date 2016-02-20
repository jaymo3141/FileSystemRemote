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

int OpenFileTable::readFile(int index, std::deque<byte>& data, int numBytes)
{

	int bytesRead{ 0 };

	//read numBytes from the buf into v
	for (int i = 0; i < numBytes; i++)
	{
		
		data.push_back(oft[index].buf[ (oft[index].position) % 64 ]);
		oft[index].position++;
		bytesRead++;
		

		
	}
	
	return bytesRead;
}

int OpenFileTable::writeFile(int index,std::deque<byte>& data, int numBytes)
{
	int bytesWritten{ 0 };

	//wrtie numBytes from the data into buf
	for (int i = 0; i < numBytes; i++)
	{
		
		//increase the file length if writting past end of file
		if (oft[index].position == oft[index].fileLength)
			oft[index].fileLength++;
			

			oft[index].buf[(oft[index].position) % 64] = data.front();
			data.pop_front();
			oft[index].position++;
			bytesWritten++;
		

		
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

void OpenFileTable::setPositionAt(int index, int position)
{
	oft[index].position = position;
}

void OpenFileTable::setFileLengthAt(int index, int fileLength)
{
	oft[index].fileLength = fileLength;
}

void OpenFileTable::setDescriptorIndexAt(int index, int descriptorIndex)
{
	oft[index].descriptorIndex = descriptorIndex;
}

void OpenFileTable::setBlockAt(int index, const Block& block)
{
	oft[index].buf = block;
}

int OpenFileTable::getFileLengthAt(int index)
{
	return oft[index].fileLength;
}

int OpenFileTable::getDescriptorIndexAt(int index)
{
	return oft[index].descriptorIndex;
}

Block& OpenFileTable::getBlockRefAt(int index)
{
	return oft[index].buf;
}

int OpenFileTable::getLowestFreeIndex()
{

	for (int i = 0; i < 4; i++)
	{
		if (oft[i].descriptorIndex == -1)
			return i;
	}

	return -1;

}

int OpenFileTable::getPositionAt(int index)
{
	return oft[index].position;
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

void DescriptorBank::setLengthAt(int index, int length)
{
	descriptorList[index].length = length;

}


void DescriptorBank::setBLock1At(int index, int block1)
{
	descriptorList[index].block1 = block1;
}

void DescriptorBank::setBLock2At(int index, int block2)
{
	descriptorList[index].block1 = block2;
}

void DescriptorBank::setBLock3At(int index, int block3)
{
	descriptorList[index].block1 = block3;
}


BitMap::BitMap() :
	leastSignificantBits{ 127 },
	mostSignificantBits{ 0 }
{

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
		mem[i] = 255;
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




int SystemSimulation::getFileDescriptorIndex(char* name)
{
	std::deque<byte> cache;
	OFTEntry dir = oft.getOFTEntryAt(0);
	FileDescriptorHandel directoryDescriptor = descriptorBank.GetDescriptorHandelAt(0);

	oft.setPositionAt(0, 0);

	oft.readFile(0, cache, 64);

	for (int i = 0, imod = 0; i < dir.fileLength; i++){

		imod = i % 64;

		if (cache[imod] == *name &&
			cache[imod + 1] == *(name + 1) &&
			cache[imod + 2] == *(name + 2) &&
			cache[imod + 3] == *(name + 3))
		{
			return BytesToInt(cache[imod + 4], cache[imod + 5], cache[imod + 6], cache[imod + 7]);
		}

		switch (i)
		{
		case 64:
			mainDisk.write_block(directoryDescriptor.block1, oft.getBlockRefAt(0));
			mainDisk.read_block(directoryDescriptor.block2, oft.getBlockRefAt(0));
			cache.clear();
			oft.readFile(0, cache, 64);
			break;
		case 128:
			mainDisk.write_block(directoryDescriptor.block2, oft.getBlockRefAt(0));
			mainDisk.read_block(directoryDescriptor.block3, oft.getBlockRefAt(0));
			cache.clear();
			oft.readFile(0, cache, 64);
			break;
		default:
			break;

		}



	}


}

void SystemSimulation::addFileToDirectory(char* name, int descriptorIndex)
{

	std::deque<byte> cache;
		
	cache.push_back(*name);
	cache.push_back(*(name + 1));
	cache.push_back(*(name + 2));
	cache.push_back(*(name + 3));

	byte b1{0}, b2{0}, b3{0}, b4{0};

	IntToBytes(descriptorIndex, b1, b2, b3, b4);

	cache.push_back(b1);
	cache.push_back(b2);
	cache.push_back(b3);
	cache.push_back(b4);

	write(0, cache, 8);

		
}

void SystemSimulation::printDirectory()
{


	std::deque<byte> cache;
	int dir = oft.getFileLengthAt(0);
	
	lseek(0, 0);

	for (int i = 0; i < dir; i += 8){

		read(0, cache, 8);

		std::cout << "File Name: " << cache[0] << cache[1] << cache[2] << cache[3] << std::endl
			<< "Descriptor: " << BytesToInt(cache[4], cache[5], cache[6], cache[7]) << std::endl;

		cache.clear();
	}
		
				
		
	

		/*switch (i)
		{
		case 64:
			mainDisk.write_block(directoryDescriptor.block1, oft.getBlockRefAt(0));
			mainDisk.read_block(directoryDescriptor.block2, oft.getBlockRefAt(0));
			cache.clear();
			oft.readFile(0, cache, 64);
			break;
		case 128:
			mainDisk.write_block(directoryDescriptor.block2, oft.getBlockRefAt(0));
			mainDisk.read_block(directoryDescriptor.block3, oft.getBlockRefAt(0));
			cache.clear();
			oft.readFile(0, cache, 64);
			break;
		default:
			break;

		}*/



	

}

int SystemSimulation::lseek(int index, int position)
{

	if (position > oft.getFileLengthAt(index))
	{	
		ss << "error";
		return -1;
	}


	FileDescriptorHandel fileDescriptor = descriptorBank.GetDescriptorHandelAt(oft.getDescriptorIndexAt(index));


	

	if (oft.getPositionAt(index) < 64) 
	{
		mainDisk.write_block(fileDescriptor.block1, oft.getBlockRefAt(index));
	}

	else if (oft.getPositionAt(index) > 63 && oft.getPositionAt(index) < 128)
	{
		mainDisk.write_block(fileDescriptor.block2, oft.getBlockRefAt(index));

	}

	else if (oft.getPositionAt(index) < 128)
	{
		mainDisk.write_block(fileDescriptor.block3, oft.getBlockRefAt(index));

	}


	if (position < 64)
	{
		mainDisk.write_block(fileDescriptor.block1, oft.getBlockRefAt(index));
	}

	else if (position > 63 && position < 128)
	{
		mainDisk.write_block(fileDescriptor.block2, oft.getBlockRefAt(index));

	}

	else if (position < 128)
	{
		mainDisk.write_block(fileDescriptor.block3, oft.getBlockRefAt(index));

	}


	oft.setPositionAt(index, position);

	return oft.getPositionAt(index);

}

int SystemSimulation::read(int index, std::deque<byte>& mem, int numBytes)
{
	int pos{ oft.getPositionAt(index) };
	int bytesRead{ 0 };
	int actualBytesToRead{0};

	//Check for reading past eof
	if (pos + numBytes > oft.getFileLengthAt(index))
	{
		actualBytesToRead = oft.getFileLengthAt(index) - pos;
	}
	else
		actualBytesToRead = numBytes;
	

	if (pos < 64 && pos + actualBytesToRead > 64)
	{
		bytesRead += oft.readFile(index, mem, 64 - pos);
		lseek(index, 64);
		if (pos + actualBytesToRead - 64 > 64)
		{
			bytesRead += oft.readFile(index, mem, 64);
			lseek(index, 128);
			bytesRead += oft.readFile(index, mem, pos + actualBytesToRead - 128);
		}
		else
			bytesRead += oft.readFile(index, mem, (pos + actualBytesToRead) - 64);
	}
	else if (pos < 128 && pos + actualBytesToRead > 128)
	{
		bytesRead += oft.readFile(index, mem, 128 - pos);
		lseek(index, 128);
		bytesRead += oft.readFile(index, mem, (pos + actualBytesToRead) - 128);
	}

	else
		return oft.readFile(index, mem, actualBytesToRead);
	

	return actualBytesToRead;

}

int SystemSimulation::write(int index, std::deque<byte>& mem, int numBytes)
{

	int pos{ oft.getPositionAt(index) };
	int bytesWritten{ 0 };
	int actualBytesToWrite{ 0 };
	bool newBlock2{ false };
	bool newBlock3{ false };
	int openBit;

	int fileDescriptorIndex = oft.getDescriptorIndexAt(index);
	FileDescriptorHandel fd = descriptorBank.GetDescriptorHandelAt(fileDescriptorIndex);

	

	//Check for reading past eof
	if (pos + numBytes > 192)
	{
		actualBytesToWrite = 192 - pos;
	}
	else
		actualBytesToWrite = numBytes;


	if (pos <= 64 && pos + actualBytesToWrite > 64)
	{

		//allocate another block if expanding the file
		if (pos + numBytes > oft.getFileLengthAt(index))
		{
			allocateSecondBlock(fileDescriptorIndex);
		}

		bytesWritten += oft.writeFile(index, mem, 64 - pos);
		lseek(index, 64);

		if (pos + actualBytesToWrite - 64 > 64)
		{
			//allocate another block if expanding the file
			if (pos + numBytes > oft.getFileLengthAt(index))
				allocateThirdBlock(fileDescriptorIndex);

			bytesWritten += oft.writeFile(index, mem, 64);
			lseek(index, 128);
			bytesWritten += oft.writeFile(index, mem, pos + actualBytesToWrite - 128);
		}
		else
			bytesWritten += oft.writeFile(index, mem, (pos + actualBytesToWrite) - 64);
	}
	else if (pos <= 128 && pos + actualBytesToWrite > 128)
	{
		//allocate another block if expanding the file
		if (pos + numBytes > oft.getFileLengthAt(index))
			allocateThirdBlock(fileDescriptorIndex);

		bytesWritten += oft.writeFile(index, mem, 128 - pos);
		lseek(index, 128);
		bytesWritten += oft.writeFile(index, mem, (pos + actualBytesToWrite) - 128);
	}

	else
	{	
		bytesWritten = oft.writeFile(index, mem, actualBytesToWrite);
		descriptorBank.setLengthAt(oft.getDescriptorIndexAt(index), oft.getFileLengthAt(index) + actualBytesToWrite);
		return bytesWritten;
	}

	descriptorBank.setLengthAt(oft.getDescriptorIndexAt(index), oft.getFileLengthAt(index) + bytesWritten);
	return bytesWritten;



}

void SystemSimulation::close(int index)
{
	
	FileDescriptorHandel fd = descriptorBank.GetDescriptorHandelAt(oft.getDescriptorIndexAt(index));
	int blockAddress; 
	int pos = oft.getPositionAt(index);
	Block emptyBlock(255);

	if (pos < 64)
		blockAddress = fd.block1;
	else if (pos > 64 && pos < 128)
		blockAddress = fd.block2;
	else
		blockAddress = fd.block3;

	mainDisk.write_block(blockAddress, oft.getBlockRefAt(index));

	oft.setOFTEntryAt(index, emptyBlock, -1, -1, -1);

}

void SystemSimulation::save(std::string fileName)
{

	std::ofstream fout;

	fout.open(fileName);

	//close all files
	for (int i = 3; i > 0; i--)
	{
		if (oft.getFileLengthAt(i) != -1)
			close(i);
	}

	//copy bitmap back to the LDisk
	bitMap.saveMapToContainer();
	mainDisk.write_block(0, bitMap.getBitMapBlock());

	//copy descriptor bank back to ldisk
	descriptorBank.WriteDescriptorListToBlocks();
	
	for (int i = 0; i < 6; i++)
	{
		mainDisk.write_block(i + 1, descriptorBank.GetDescriptorBlockAt(i));
	}

	Block temp;

	for (int i = 0; i < 64; i++)
	{
		mainDisk.read_block(i, temp);
		for (int j = 0; j < 64; j++)
		{

			fout << temp[j];

		}
	}



}


void SystemSimulation::allocateSecondBlock(int descriptorIndex)
{
	int openBit = bitMap.getNextOpenBit();
	descriptorBank.setBLock2At(descriptorIndex, openBit);
	bitMap.setBit(openBit, true);
}

void SystemSimulation::allocateThirdBlock(int descriptorIndex)
{
	int openBit = bitMap.getNextOpenBit();
	descriptorBank.setBLock3At(descriptorIndex, openBit);
	bitMap.setBit(openBit, true);
}



SystemSimulation::SystemSimulation()
{
	//Sets up the directory
	oft.setDescriptorIndexAt(0, 0);
	oft.setFileLengthAt(0, 0);
	oft.setPositionAt(0, 0);

	descriptorBank.setLengthAt(0, 0);

	int openBit = bitMap.getNextOpenBit();
	bitMap.setBit(openBit, true);

	descriptorBank.setBLock1At(0, openBit);


}


int BytesToInt(byte b1, byte b2, byte b3, byte b4)
{
	unsigned int a{ b4 };
	unsigned int b{ b3*(unsigned int)pow(2, 8) };
	unsigned int c{ b2*(unsigned int)pow(2, 16) };
	unsigned int d{ b1*(unsigned int)pow(2, 24) };

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

	oft.setOFTEntryAt(0, b1, 0, 0, 10);
	oft.setOFTEntryAt(1, b2, 0, 2, 0);
	oft.setOFTEntryAt(2, b3, 0, 4, 20);
	oft.setOFTEntryAt(3, b4, 0, 5, 0);

	std::deque<byte> testList;
	

	oft.readFile(0, testList, 10);

	std::cout << "Following numbers should be 127" << std::endl;

	for (int i = 0; i < testList.size(); i++)
	{
		std::cout << (int)testList[i] << std::endl;
	}

	std::cout << std::endl;

	oft.writeFile(1, testList, 10);

	testList.clear();

	oft.setPositionAt(1, 0);

	oft.readFile(1, testList, 10);

	std::cout << "Following numbers should be 127" << std::endl;

	for (int i = 0; i < testList.size(); i++)
	{
		std::cout << (int)testList[i] << std::endl << std::endl;
	}

	std::cout << "Block 0 \n" << "File Length (10): " << oft.getFileLengthAt(0) << "\nDescriptor Index " << oft.getDescriptorIndexAt(0) << "\n";

	std::cout << "Block 1 \n" << "File Length (10): " << oft.getFileLengthAt(1) << "\nDescriptor Index " << oft.getDescriptorIndexAt(1) << "\n";


	std::deque<byte> test2;

	oft.readFile(2, test2, 20);

	std::cout << "Following numbers should be 15" << std::endl;

	for (int i = 0; i < test2.size(); i++)
	{
		std::cout << (int)test2[i] << std::endl << std::endl;
	}

	oft.writeFile(3, test2, 20);

	Block testBlock = oft.getBlockRefAt(3);

	std::cout << "Following numbers should be 15" << std::endl;

	for (int i = 0; i < 15; i++)
	{
		std::cout << (int)testBlock[i] << std::endl << std::endl;
	}


	std::cout << "EXITING OPEN_FILE_TABLE_TEST" << std::endl << std::endl;


}

void SYSTEM_SIMULATION_TEST()
{

	std::cout << "ENTERING SYSTEM_SIMULATION_TEST" << std::endl;


	SystemSimulation sys;

	//sys.addFileToDirectory("abc", 1);
	//sys.addFileToDirectory("abcd", 2);
	//sys.addFileToDirectory("efgh", 3);

	//sys.printDirectory();

	//std::cout << std::endl << std::endl;

	//fill up the directory

	for (int i = 0; i < 10; i++)
	{
		sys.addFileToDirectory("test", i);
	}
	
	sys.printDirectory();


	std::cout << "EXITING SYSTEM_SIMULATION_TEST" << std::endl;

}

