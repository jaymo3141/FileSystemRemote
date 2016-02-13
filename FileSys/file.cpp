#include "file.h"



char& Block::operator[](int i)
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

void LDisk::read_block(int i, Block *p){

	//Read block i into block at pointer p
	*p = disk[i];


}

void LDisk::write_block(int i, Block *p){

	//Write block at pointer p into block i
	disk[i] = *p;


}