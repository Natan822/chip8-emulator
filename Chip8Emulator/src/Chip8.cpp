#include "Chip8.h"
#include <fstream>

Chip8::Chip8() 
	: randGen(std::chrono::system_clock::now().time_since_epoch().count()) 
{
	pc = START_ADDRESS;

	for (int i = 0; i < FONTSET_SIZE; i++)
	{
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

void Chip8::LoadROM(const char* filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (int i = 0; i < size; i++)
		{
			memory[START_ADDRESS + i] = buffer[i];
		}

		delete[] buffer;
	}

}