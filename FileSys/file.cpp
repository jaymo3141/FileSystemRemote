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

	if (numBytes == 0)
		return 0;

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

	if (numBytes == 0)
		return 0;

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
	int k{ 0 };
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 63; j+=16, k++){

			descriptorList[k].length = BytesToInt(descriptorBlocks[i][j], descriptorBlocks[i][j + 1], descriptorBlocks[i][j + 2], descriptorBlocks[i][j + 3]);
			descriptorList[k].block1 = BytesToInt(descriptorBlocks[i][j + 4], descriptorBlocks[i][j + 5], descriptorBlocks[i][j + 6], descriptorBlocks[i][j + 7]);
			descriptorList[k].block2 = BytesToInt(descriptorBlocks[i][j + 8], descriptorBlocks[i][j + 9], descriptorBlocks[i][j + 10], descriptorBlocks[i][j + 11]);
			descriptorList[k].block3 = BytesToInt(descriptorBlocks[i][j + 12], descriptorBlocks[i][j + 13], descriptorBlocks[i][j + 14], descriptorBlocks[i][j + 15]);

		}
	}



}

void DescriptorBank::WriteDescriptorListToBlocks()
{
	int k{ 0 };
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 63; j += 16, k++){

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
	descriptorList[index].block2 = block2;
}

void DescriptorBank::setBLock3At(int index, int block3)
{
	descriptorList[index].block3 = block3;
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

void BitMap::setLeastSignificantBits(int value)
{
	leastSignificantBits = value;
}

void BitMap::setMostSignificantBits(int value)
{
	mostSignificantBits = value;
}

void BitMap::loadMapToContainer(const Block& map)
{

	bitMapContainer = map;

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

void Block::printContents()
{
	for (int i = 0; i < 64; i++)
		std::cout << (int)mem[i] << std::endl;
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




int SystemSimulation::getFileDescriptorIndex(const char* name)
{
	std::deque<byte> cache;
	OFTEntry dir = oft.getOFTEntryAt(0);
	FileDescriptorHandel directoryDescriptor = descriptorBank.GetDescriptorHandelAt(0);

	lseek(0, 0);


	for (int i = 0; i < dir.fileLength/8; i++){

		read(0, cache, 8);


		if (cache[0] == *name &&
			cache[1] == *(name + 1) &&
			cache[2] == *(name + 2) &&
			cache[3] == *(name + 3))
		{
			lseek(0, dir.fileLength);
			return BytesToInt(cache[4], cache[5], cache[6], cache[7]);
		}

		cache.clear();


	}

	lseek(0, dir.fileLength);
	return -1;

}

void SystemSimulation::addFileToDirectory(const char* name, int descriptorIndex)
{

	//check for name duplicates
	if (getFileDescriptorIndex(name) != -1)
	{	
		ss << "error\n";
		return;

	}
	

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

		ss << cache[0] << cache[1] << cache[2] << cache[3] << std::endl;


		cache.clear();
	}
	
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

	else if (oft.getPositionAt(index) > 128)
	{
		mainDisk.write_block(fileDescriptor.block3, oft.getBlockRefAt(index));

	}


	if (position < 64)
	{
		mainDisk.read_block(fileDescriptor.block1, oft.getBlockRefAt(index));
	}

	else if (position > 63 && position < 128)
	{
		mainDisk.read_block(fileDescriptor.block2, oft.getBlockRefAt(index));

	}

	else if (position > 128)
	{
		mainDisk.read_block(fileDescriptor.block3, oft.getBlockRefAt(index));

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
	

	if (pos < 64 && pos + actualBytesToRead >= 64)
	{
		bytesRead += oft.readFile(index, mem, 64 - pos);
		swapBlocks(index, 1, 2);
		if (pos + actualBytesToRead - 64 >= 64)
		{
			bytesRead += oft.readFile(index, mem, 64);
			swapBlocks(index, 2, 3);
			bytesRead += oft.readFile(index, mem, pos + actualBytesToRead - 128);
		}
		else
			bytesRead += oft.readFile(index, mem, (pos + actualBytesToRead) - 64);
	}
	else if (pos < 128 && pos + actualBytesToRead >= 128)
	{
		bytesRead += oft.readFile(index, mem, 128 - pos);
		swapBlocks(index, 2, 3);
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


	if (pos < 64 && pos + actualBytesToWrite >= 64)
	{

		//allocate another block if expanding the file
		if (pos + numBytes >= oft.getFileLengthAt(index) && fd.block2 == -1)
		{
			allocateSecondBlock(fileDescriptorIndex);
		}

		bytesWritten += oft.writeFile(index, mem, 64 - pos);
		swapBlocks(index, 1, 2);

		if (pos + actualBytesToWrite - 64 >= 64)
		{
			//allocate another block if expanding the file
			if (pos + numBytes >= oft.getFileLengthAt(index) && fd.block3 == -1)
				allocateThirdBlock(fileDescriptorIndex);

			bytesWritten += oft.writeFile(index, mem, 64);
			swapBlocks(index, 2, 3);
			bytesWritten += oft.writeFile(index, mem, pos + actualBytesToWrite - 128);
		}
		else
			bytesWritten += oft.writeFile(index, mem, (pos + actualBytesToWrite) - 64);
	}
	else if (pos < 128 && pos + actualBytesToWrite >= 128)
	{
		//allocate another block if expanding the file
		if (pos + numBytes >= oft.getFileLengthAt(index) && fd.block3 == -1)
			allocateThirdBlock(fileDescriptorIndex);

		bytesWritten += oft.writeFile(index, mem, 128 - pos);
		swapBlocks(index, 2, 3);
		bytesWritten += oft.writeFile(index, mem, (pos + actualBytesToWrite) - 128);
	}

	else
	{	
		bytesWritten = oft.writeFile(index, mem, actualBytesToWrite);
		descriptorBank.setLengthAt(oft.getDescriptorIndexAt(index), fd.length + actualBytesToWrite);
		return bytesWritten;
	}

		descriptorBank.setLengthAt(oft.getDescriptorIndexAt(index), oft.getFileLengthAt(index));

	return bytesWritten;



}

int SystemSimulation::close(int index)
{



	//Check
	//1. index is less than 4
	//2. close an empty index
	if (index > 3)
	{
		ss << "error\n";
		return -1;
	}

	if (oft.getFileLengthAt(index) == -1)
	{
		ss << "error\n";
			return -1;
	}
	

	
	FileDescriptorHandel fd = descriptorBank.GetDescriptorHandelAt(oft.getDescriptorIndexAt(index));
	int blockAddress; 
	int pos = oft.getPositionAt(index);
	Block emptyBlock(255);

	if (pos < 64)
		blockAddress = fd.block1;
	else if (pos >= 64 && pos < 128)
		blockAddress = fd.block2;
	else
		blockAddress = fd.block3;

	mainDisk.write_block(blockAddress, oft.getBlockRefAt(index));

	oft.setOFTEntryAt(index, emptyBlock, -1, -1, -1);

	return 0;

}

int SystemSimulation::save(std::string fileName)
{

	std::ofstream fout;

	fout.open(fileName);

	//close all files
	for (int i = 3; i >= 0; i--)
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

	return 0;

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

void SystemSimulation::swapBlocks(int openFileIndex, int BlockX, int BlockY)
{
	FileDescriptorHandel fileDescriptor = descriptorBank.GetDescriptorHandelAt(oft.getDescriptorIndexAt(openFileIndex));

	switch (BlockX)
	{
	case 1:
		switch (BlockY)
		{
		case 2:
			mainDisk.write_block(fileDescriptor.block1, oft.getBlockRefAt(openFileIndex));
			mainDisk.read_block(fileDescriptor.block2, oft.getBlockRefAt(openFileIndex));
			break;
		case 3:
			mainDisk.write_block(fileDescriptor.block1, oft.getBlockRefAt(openFileIndex));
			mainDisk.read_block(fileDescriptor.block3, oft.getBlockRefAt(openFileIndex));
			break;
		}
		break;

	case 2:
		switch (BlockY)
		{
		case 1:
			mainDisk.write_block(fileDescriptor.block2, oft.getBlockRefAt(openFileIndex));
			mainDisk.read_block(fileDescriptor.block1, oft.getBlockRefAt(openFileIndex));
			break;
		
		case 3:
			mainDisk.write_block(fileDescriptor.block2, oft.getBlockRefAt(openFileIndex));
			mainDisk.read_block(fileDescriptor.block3, oft.getBlockRefAt(openFileIndex));
			break;
		}
		break;

	case 3:
		switch (BlockY)
		{
		case 1:
			mainDisk.write_block(fileDescriptor.block3, oft.getBlockRefAt(openFileIndex));
			mainDisk.read_block(fileDescriptor.block1, oft.getBlockRefAt(openFileIndex));
			break;
		case 2:
			mainDisk.write_block(fileDescriptor.block3, oft.getBlockRefAt(openFileIndex));
			mainDisk.read_block(fileDescriptor.block1, oft.getBlockRefAt(openFileIndex));
			break;
		
		}
		break;
	}




}

void SystemSimulation::removeFromDirectory(const char* name)
{

	//Check
	//1. File doesn't exist
	if (getFileDescriptorIndex(name) == -1)
	{
		ss << "error\n";
		return;
	}

	std::deque<byte> cache;
	OFTEntry dir = oft.getOFTEntryAt(0);
	FileDescriptorHandel directoryDescriptor = descriptorBank.GetDescriptorHandelAt(0);

	lseek(0, 0);

	read(0, cache, dir.fileLength);
	

	for (int i = 0; i < dir.fileLength; i+=8){



		if (cache[i] == *name &&
			cache[i + 1] == *(name + 1) &&
			cache[i + 2] == *(name + 2) &&
			cache[i + 3] == *(name + 3))
		{
			for (int k = i; k < dir.fileLength - 8; k+=8)
			{
				cache[k] = cache[k + 8];
				cache[k + 1] = cache[k + 9];
				cache[k + 2] = cache[k + 10];
				cache[k + 3] = cache[k + 11];
				cache[k + 4] = cache[k + 12];
				cache[k + 5] = cache[k + 13];
				cache[k + 6] = cache[k + 14];
				cache[k + 7] = cache[k + 15];				
			}
			break;
		}

		
	}

	oft.setFileLengthAt(0, dir.fileLength - 8);
	descriptorBank.setLengthAt(0, dir.fileLength - 8);

	lseek(0, 0);

	write(0, cache, std::max(dir.fileLength - 8, 0));

	lseek(0, std::max(dir.fileLength - 8, 0));



}

int SystemSimulation::open(const char* name)
{


	Block buf;

	int descriptorIndex = getFileDescriptorIndex(name);

	//Check 
	//1. file is already open
	//2. file exists in the directory
	//3. too many files open
	if (descriptorIndex == -1)
	{
		//file hasn't been created
		ss << "error\n";
		return -1;
	}

	if (descriptorIndex == oft.getDescriptorIndexAt(1) ||
		descriptorIndex == oft.getDescriptorIndexAt(2) ||
		descriptorIndex == oft.getDescriptorIndexAt(3))
	{
		ss << "error\n";
		return -1;
	}

	int openEntry = oft.getLowestFreeIndex();
	
	if (openEntry == -1)
	{
		//too many files open
		ss << "error\n";
		return -1;
	}


	FileDescriptorHandel fd = descriptorBank.GetDescriptorHandelAt(descriptorIndex);


	mainDisk.read_block(fd.block1, buf);

	oft.setOFTEntryAt(openEntry, buf, 0, descriptorIndex, fd.length);

	return openEntry;

	
}

int SystemSimulation::create(const char* name)
{
	//Check
	//1. file already exists
	//2. disk is full
	if (getFileDescriptorIndex(name) != -1)
	{
		ss << "error\n";
		return -1;

	}

	int freeDescriptorIndex = descriptorBank.GetFreeDescriptorIndex();

	if (freeDescriptorIndex == -1)
	{
		//no descriptors left, disk is full
		ss << "error\n";
		return -1;
	}
	
	addFileToDirectory(name, freeDescriptorIndex);

	int openBit = bitMap.getNextOpenBit();
	bitMap.setBit(openBit, true);

	descriptorBank.SetDescriptorHandelAt(freeDescriptorIndex, 0, openBit);

	return 0;

}

int SystemSimulation::destroy(const char* name)
{
	//Check
	//1. file doesn't exist
	if (getFileDescriptorIndex(name) == -1)
	{
		ss << "error\n";
		return -1;

	}

	int descriptorIndex = getFileDescriptorIndex(name);
	FileDescriptorHandel fd = descriptorBank.GetDescriptorHandelAt(descriptorIndex);

	//close the file before destroying it
	for (int i = 1; i < 4; i++){
		if (descriptorIndex == oft.getDescriptorIndexAt(i))
		{
			close(i);
		}
	}

	removeFromDirectory(name);
	
	//free bitmap entries
	if (fd.block1 != -1)
		bitMap.setBit(fd.block1, false);

	if (fd.block2 != -1)
		bitMap.setBit(fd.block2, false);

	if (fd.block3 != -1)
		bitMap.setBit(fd.block3, false);

	//free the descriptor entry
	descriptorBank.SetDescriptorHandelAt(descriptorIndex, -1);

	return 0;

}

int SystemSimulation::init()
{
	Block emptyBlock(255);

	oft.setOFTEntryAt(0, emptyBlock, -1, -1, -1);
	oft.setOFTEntryAt(1, emptyBlock, -1, -1, -1);
	oft.setOFTEntryAt(2, emptyBlock, -1, -1, -1);
	oft.setOFTEntryAt(3, emptyBlock, -1, -1, -1);

	bitMap.setLeastSignificantBits(127);
	bitMap.setMostSignificantBits(0);

	for (int i = 0; i < 24; i++){
		descriptorBank.SetDescriptorHandelAt(i, -1);
	}

	for (int i = 0; i < 64; i++)
	{
		mainDisk.write_block(i, emptyBlock);
	}

	//Sets up the directory
	oft.setDescriptorIndexAt(0, 0);
	oft.setFileLengthAt(0, 0);
	oft.setPositionAt(0, 0);

	descriptorBank.setLengthAt(0, 0);

	int openBit = bitMap.getNextOpenBit();
	bitMap.setBit(openBit, true);

	descriptorBank.setBLock1At(0, openBit);

	return 0;
}

int SystemSimulation::init(std::string fileName)
{

	/*std::ifstream fin;

	fin.open(fileName, std::ios::binary);

	byte input;
	std::vector<byte> fileContents;

	for (int i = 0; i < 4096; i++)
	{
		fin >> input;
		fileContents.push_back(input);

	}*/


	// read entire file into string
	std::ifstream is(fileName, std::ifstream::binary);
	
	// get length of file:
	is.seekg(0, is.end);
	int length = is.tellg();
	is.seekg(0, is.beg);

	std::string fileContents;
	fileContents.resize(length, ' '); // reserve space
	char* begin = &*fileContents.begin();

	is.read(begin, length);
	is.close();		
	

	Block inputBlock;

	//load into disk
	for (int i = 0, k = 0; i < 4096; i++)
	{

		inputBlock[i % 64] = fileContents[i];

		if (i % 64 == 63)
		{
			mainDisk.write_block(k++, inputBlock);
		}

	}

	//load descriptor list
	Block readBlock;

	for (int i = 0; i < 7; i++){

		mainDisk.read_block(i + 1, readBlock);
		descriptorBank.SetDesctriptorBlock(i, readBlock);

	}

	descriptorBank.BuildDescriptorList();

	//load bitmap
	mainDisk.read_block(0, readBlock);

	bitMap.loadMapToContainer(readBlock);


	//open directory
	FileDescriptorHandel fd = descriptorBank.GetDescriptorHandelAt(0);

	mainDisk.read_block(fd.block1, readBlock);

	oft.setOFTEntryAt(0, readBlock, fd.length, 0, fd.length);

	


	return 0;

}



SystemSimulation::SystemSimulation() 
	/*oft(),
	bitMap()*/
{
	////Sets up the directory
	//oft.setDescriptorIndexAt(0, 0);
	//oft.setFileLengthAt(0, 0);
	//oft.setPositionAt(0, 0);

	//descriptorBank.setLengthAt(0, 0);

	//int openBit = bitMap.getNextOpenBit();
	//bitMap.setBit(openBit, true);

	//descriptorBank.setBLock1At(0, openBit);


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

	

	std::cout << std::endl << std::endl;

	//fill up the directory

	std::stringstream str;

	for (int i = 0; i < 10; i++)
	{
		//seriously wtf?

		char name1[] = "abcx";
		name1[3] = i + 48;

		sys.create(name1);

	}

	for (int i = 0; i < 10; i++)
	{
		//seriously wtf?

		char name2[] = "abcx";
		name2[2] = i + 48;

		sys.create(name2);

	}
	
	sys.printDirectory();

	

	for (int i = 0; i < 10; i++)
	{
		//seriously wtf?

		char name3[] = "abcx";
		name3[2] = i + 48;

		sys.destroy(name3);

	}

	std::cout << std::endl << std::endl;

	sys.printDirectory();

	sys.open("abc1");
	sys.open("abc2");
	sys.open("abc3");

	sys.close(1);
	sys.close(2);
	sys.close(3);





	std::cout << "EXITING SYSTEM_SIMULATION_TEST" << std::endl;

}

