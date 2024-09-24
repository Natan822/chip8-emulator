#include "Chip8.h"
#include <fstream>

bool screenUpdate = true;

Chip8::Chip8() 
	: randGen(std::chrono::system_clock::now().time_since_epoch().count()) 
{
	pc = START_ADDRESS;

	for (int i = 0; i < FONTSET_SIZE; i++)
	{
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	randByte = std::uniform_int_distribution<int>(0, 255u);

	// Tables
	tables[0x0] = &Chip8::table0;
	tables[0x1] = &Chip8::OP_1nnn;
	tables[0x2] = &Chip8::OP_2nnn;
	tables[0x3] = &Chip8::OP_3xnn;
	tables[0x4] = &Chip8::OP_4xnn;
	tables[0x5] = &Chip8::OP_5XY0;
	tables[0x6] = &Chip8::OP_6XNN;
	tables[0x7] = &Chip8::OP_7XNN;
	tables[0x8] = &Chip8::table8;
	tables[0x9] = &Chip8::OP_9XY0;
	tables[0xA] = &Chip8::OP_ANNN;
	tables[0xB] = &Chip8::OP_BNNN;
	tables[0xC] = &Chip8::OP_CXNN;
	tables[0xD] = &Chip8::OP_DXYN;
	tables[0xE] = &Chip8::tableE;
	tables[0xF] = &Chip8::tableF;

	for (size_t i = 0; i <= 0xE; i++)
	{
		tables0[i] = &Chip8::OP_NULL;
		tables8[i] = &Chip8::OP_NULL;
		tablesE[i] = &Chip8::OP_NULL;
	}

	// Tables 0
	tables0[0] = &Chip8::OP_00E0;
	tables0[0xE] = &Chip8::OP_00EE;

	// Tables 8
	tables8[0x0] = &Chip8::OP_8XY0;
	tables8[0x1] = &Chip8::OP_8XY1;
	tables8[0x2] = &Chip8::OP_8XY2;
	tables8[0x3] = &Chip8::OP_8XY3;
	tables8[0x4] = &Chip8::OP_8XY4;
	tables8[0x5] = &Chip8::OP_8XY5;
	tables8[0x6] = &Chip8::OP_8XY6;
	tables8[0x7] = &Chip8::OP_8XY7;
	tables8[0xE] = &Chip8::OP_8XYE;

	// Tables E
	tablesE[0x1] = &Chip8::OP_EXA1;
	tablesE[0xE] = &Chip8::OP_EX9E;

	for (size_t i = 0; i <= 0x65; i++)
	{
		tablesF[i] = &Chip8::OP_NULL;
	}

	// Tables F
	tablesF[0x07] = &Chip8::OP_FX07;
	tablesF[0x0A] = &Chip8::OP_FX0A;
	tablesF[0x15] = &Chip8::OP_FX15;
	tablesF[0x18] = &Chip8::OP_FX18;
	tablesF[0x1E] = &Chip8::OP_FX1E;
	tablesF[0x29] = &Chip8::OP_FX29;
	tablesF[0x33] = &Chip8::OP_FX33;
	tablesF[0x55] = &Chip8::OP_FX55;
	tablesF[0x65] = &Chip8::OP_FX65;
}

Chip8::~Chip8() {
}

void Chip8::table0() {
	(this->*tables0[opcode & 0xFu])();
}

void Chip8::table8() {
	(this->*tables8[opcode & 0xFu])();
}

void Chip8::tableE() {
	(this->*tablesE[opcode & 0xFu])();
}

void Chip8::tableF() {
	(this->*tablesF[opcode & 0xFFu])();
}

void Chip8::LoadROM(std::string filePath) {
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);

	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file: " + filePath);
	}

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

void Chip8::Cycle() {
	Chip8::fetch();
	Chip8::execute();

	if (soundTimer > 0)
	{
		soundTimer--;
	}

	if (delayTimer > 0)
	{
		delayTimer--;
	}
}

void Chip8::fetch() {
	opcode = (memory[pc] << 8) + memory[pc + 1];
	pc += 2;
}

void Chip8::execute() {
	(this->*tables[(opcode & 0xF000) >> 12u])();
}

// Instructions
void Chip8::OP_NULL() {}

void Chip8::OP_00E0() {
	memset(video, 0, sizeof(video));
}

void Chip8::OP_00EE() {
	sp--;
	pc = stack[sp];
}

void Chip8::OP_1nnn() {
	uint16_t address = opcode & 0x0FFFu;
	pc = address;
}

void Chip8::OP_2nnn() {
	uint16_t address = opcode & 0x0FFFu;

	stack[sp] = pc;
	sp++;

	pc = address;
}

void Chip8::OP_3xnn() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = opcode & 0x00FFu;

	if (registers[Vx] == value)
	{
		pc += 2;
	}
}

void Chip8::OP_4xnn() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = opcode & 0x00FFu;

	if (registers[Vx] != value)
	{
		pc += 2;
	}
}

void Chip8::OP_5XY0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] == registers[Vy])
	{
		pc += 2;
	}
}

void Chip8::OP_6XNN() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = opcode & 0x00FFu;

	registers[Vx] = value;
}

void Chip8::OP_7XNN() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = opcode & 0x00FFu;

	registers[Vx] += value;
}

void Chip8::OP_8XY0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] = registers[Vy];
}

void Chip8::OP_8XY1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] |= registers[Vy];
}

void Chip8::OP_8XY2() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] &= registers[Vy];
}

void Chip8::OP_8XY3() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] ^= registers[Vy];
}

void Chip8::OP_8XY4() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	uint16_t sum = registers[Vx] + registers[Vy];

	if (sum > 255)
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 1;
	}

	registers[Vx] = sum & 0xFFu;
}

void Chip8::OP_8XY5() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vy] > registers[Vx])
	{
		registers[0xF] = 0;
	}
	else
	{
		registers[0xF] = 1;
	}
	
	registers[Vx] -= registers[Vy];
}

void Chip8::OP_8XY6() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[0xF] = registers[Vx] & 1;
	registers[Vx] >>= 1;
}

void Chip8::OP_8XY7() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] > registers[Vy])
	{
		registers[0xF] = 0;
	}
	else
	{
		registers[0xF] = 1;
	}
	registers[Vx] = registers[Vy] - registers[Vx];
}

void Chip8::OP_8XYE() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

	registers[Vx] <<= 1;
}

void Chip8::OP_9XY0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] != registers[Vy])
	{
		pc += 2;
	}
}

void Chip8::OP_ANNN() {
	uint16_t value = opcode & 0x0FFFu;
	index = value;
}

void Chip8::OP_BNNN() {
	uint16_t value = opcode & 0x0FFFu;
	pc = value + registers[0];
}

void Chip8::OP_CXNN() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = opcode & 0x00FF;

	registers[Vx] = randByte(randGen) & value;
}

void Chip8::OP_DXYN() {
	screenUpdate = true;

	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0xFu;

	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

	registers[0xF] = 0;

	for (size_t row = 0; row < height; row++)
	{
		uint8_t spriteByte = memory[index + row];
		for (size_t bit = 0; bit < 8; bit++)
		{
			uint8_t spriteBit = spriteByte & (0x80u >> bit);
			uint32_t* screenBit = &video[xPos + (VIDEO_WIDTH * (yPos + row)) + bit];

			if (spriteBit != 0)
			{
				if (*screenBit == 0xFFFFFFFF)
				{
					registers[0xF] = 1;
				}

				*screenBit ^= 0xFFFFFFFF;
			}

		}
	}
}

void Chip8::OP_EX9E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t key = registers[Vx];

	if (keypad[key] == 1)
	{
		pc += 2;
	}
}

void Chip8::OP_EXA1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t key = registers[Vx];

	if (keypad[key] == 0)
	{
		pc += 2;
	}
}

void Chip8::OP_FX07() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	registers[Vx] = delayTimer;
}

void Chip8::OP_FX0A() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (size_t key = 0; key < 16; key++)
	{
		if (keypad[key] == 1)
		{
			registers[Vx] = key;
			break;
		}
	}
	pc -= 2;
}

void Chip8::OP_FX15() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	delayTimer = registers[Vx];
}

void Chip8::OP_FX18() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	soundTimer = registers[Vx];
}

void Chip8::OP_FX1E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	index += registers[Vx];
	if (index > 0xFFF)
	{
		registers[0xFu] = 1;
	}
}

void Chip8::OP_FX29() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = registers[Vx];

	index = FONTSET_START_ADDRESS + (value * 5);
}

void Chip8::OP_FX33() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = registers[Vx];

	for (int i = 2; i >= 0; i--)
	{
		memory[index + i] = value % 10;
		value /= 10;
	}
}

void Chip8::OP_FX55() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (size_t i = 0; i <= Vx; i++)
	{
		memory[index + i] = registers[i];
	}
}

void Chip8::OP_FX65() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (size_t i = 0; i <= Vx; i++)
	{
		registers[i] = memory[index + i];
	}
}