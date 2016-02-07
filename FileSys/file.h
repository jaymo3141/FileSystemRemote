#ifndef FILE_H
#define FILE_H


#include <exception>
#include <stdexcept>
#include <iostream>

class Block {
private:

	char byte[64];

public:

	char& operator[](int i); 

};
 
class BitMap : public Block
{


};

class LDisk{
private:

	Block disk[64];


public:
};





#endif