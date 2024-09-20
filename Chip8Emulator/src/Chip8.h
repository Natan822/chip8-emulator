#pragma once

#include <chrono>
#include <cstdint>
#include <random>

const unsigned int START_ADDRESS = 0x200;

const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int FONTSET_SIZE = 80;

const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;

class Chip8
{

	typedef void (Chip8::*Chip8Table)();

public:
	uint8_t registers[16]{};
	uint8_t memory[4096]{};
	uint16_t index{}; // Index Register
	uint16_t pc{}; // Program Counter
	uint16_t stack[16]{};
	uint8_t sp{}; // Stack Pointer
	uint8_t delayTimer{};
	uint8_t soundTimer{};
	uint8_t keypad[16]{};
	uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT]{};
	uint16_t opcode;

	std::default_random_engine randGen;
	std::uniform_int_distribution<int> randByte;

	uint8_t fontset[FONTSET_SIZE] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	Chip8Table tables[0xF + 1]{};
	Chip8Table tables0[0xE + 1]{};
	Chip8Table tables8[0xE + 1]{};
	Chip8Table tablesE[0xE + 1]{};
	Chip8Table tablesF[0x65 + 1]{};

	Chip8();
	~Chip8();

	void LoadROM(const char* filename);
	void Cycle();

private:
	void fetch();
	void execute();
	
	void table0();
	void table8();
	void tableE();
	void tableF();

	// Instructions
	void OP_NULL(); // Nothing
	void OP_00E0(); // CLS
	void OP_00EE(); // RET
	void OP_1nnn(); // JMP
	void OP_2nnn(); // CALL
	void OP_3xnn(); // Skip next instruction if register VX == NN
	void OP_4xnn(); // Skip next instruction if register VX != NN
	void OP_5XY0(); // Skip next instruction if register VX == register VY
	void OP_6XNN(); // LD VX, NN
	void OP_7XNN(); // ADD VX, NN
	void OP_8XY0(); // LD VX, VY
	void OP_8XY1(); // OR VX. VY
	void OP_8XY2(); // AND VX, VY
	void OP_8XY3(); // XOR VX, VY
	void OP_8XY4(); // ADD VX, VY (in case of overflow, VF = 1, otherwise VF = 0)
	void OP_8XY5(); // SUB, VX, VY (in case of underflow, VF = 0, otherwise VF = 1)
	void OP_8XY6(); // SHR VX, VY (shift VX right 1. Ignore VY. VF = least significant bit of VX)
	void OP_8XY7(); // SUBN VX, VY (VX = VY - VX)(in case of underflow(VX > VY), VF = 0)
	void OP_8XYE(); // SHL VX, VY (shift VX left 1. Ignore VY. VF = most significant bit of VX)
	void OP_9XY0(); // Skip next instruction if register VX != VY
	void OP_ANNN(); // Index = NNN
	void OP_BNNN(); // PC = NNN + V0
	void OP_CXNN(); // VX = (random(0, 255) & NN)
	void OP_DXYN(); // Display N sprite at (VX, VY) xor'ing with what's already. If there's collision then VF = 1 , otherwise VF = 0
	void OP_EX9E(); // Skip next instruction if key indicated by VX is pressed
	void OP_EXA1(); // Skip next instruction if key indicated by VX is not pressed
	void OP_FX07(); // LD VX, DT
	void OP_FX0A(); // Wait for a key press and store the value of the key into VX
	void OP_FX15(); // LD DT, VX
	void OP_FX18(); // LD ST, VX
	void OP_FX1E(); // ADD index, VX. if index > 0x0FFF then VF = 1, otherwise VF = 0
	void OP_FX29(); // I = address of the CHIP-8 8x5 font sprite representing the value in VX.
	void OP_FX33(); // Convert VX to BCD and stores the hundreds digits at index, tens digit at index + 1 and ones digit index + 2
	void OP_FX55(); // Stores registers values from V0 to VX inclusive at memory address indicated by index
	void OP_FX65(); // Copy values from index to index + X incluse to registers from V0 to VX inclusive
};