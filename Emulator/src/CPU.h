#pragma once

#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <bitset>
#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <iomanip>

#define MEMORY_MAX_RANGE 65536


class registerPair {
public:
	std::bitset<8> high;
	std::bitset<8> low;

	registerPair()
	{
		high = std::bitset<8>(0);
		low = std::bitset<8>(0);
	}

	void setHigh(uint8_t value)
	{
		auto val = std::bitset<8>(value);
		for (int i = 0; i < 8; i++)
		{
			high.set(i, val[i]);
		}
	}

	void setLow(uint8_t value)
	{
		auto val = std::bitset<8>(value);
		for (int i = 0; i < 8; i++)
		{
			low.set(i, val[i]);
		}
	}

	std::bitset<16> getPair()
	{
		std::bitset<16> pair(0);
		for (int i = 0; i < 8; i++)
		{
			pair.set(i, low[i]);
			pair.set(i + 8, high[i]);
		}
		return pair;
	}

	std::string toString()
	{
		return this->getPair().to_string();
	}

	int toInt()
	{
		auto value = this->getPair().to_ulong();
		return (int)value;
	}

	void setPair(uint16_t value)
	{
		std::bitset<16> val(value);
		for (int i = 0; i < 8; i++)
		{
			low.set(i, val[i]);
			high.set(i, val[i + 8]);
		}
	}
};

/* The Sharp CPU is emulated here. */
class CPU {
public:

	CPU();

	void reset();

	void cycle();

	void cycleLog();

	// This will load the contents of a ROM into the memory.
	void loadROM(std::string filename);

	void loadOpcode(uint16_t opcode);

	void resetPC();

	void resetMemory();

	std::vector<std::string> getRegisterValues();

	// Get the integer values stored in the registers.
	std::vector<uint8_t> getRegisters();

	// Used to directly modify the stack pointer.
	void setSP(uint16_t val);

	// Read the value of the stack pointer directly.
	uint16_t getSP();

	// Used to read a value directly from memory.
	uint16_t readMemory(uint16_t pos);

private:

	// 64 KiB of memory. TODO: Change this to dynamic allocation
	uint16_t memory[MEMORY_MAX_RANGE] = { 0 };

	// 16-bit address buffer
	uint16_t addressBuffer = 0;

	/* CPU registers and flags */
	
	// 8-bit accumulator (A)
	uint8_t accumulator = 0;

	/* 8-bit status flags;
	* Bit 7: (Z) Zero
	* Bit 6: (N) Subtraction (BCD)
	* Bit 5: (H) Half-carry (BCD)
	* Bit 4: (C) Carry
	* */
	std::bitset<8> flags = std::bitset<8>(0);
		
	/* LCD control register
	* Bit 7 - LCD Display Enable (0=Off, 1=On)
	* Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
	* Bit 5 - Window Display Enable (0=Off, 1=On)
	* Bit 4 - BG & Window Tile Data Select (0=8800-97FF, 1=8000-8FFF)
	* Bit 3 - BG Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
	* Bit 2 - OBJ (Sprite) Size (0=8x8, 1=8x16)
	* Bit 1 - OBJ (Sprite) Display Enable (0=Off, 1=On)
	* Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)
	* */
	std::bitset<8> lcdreg = std::bitset<8>(0);

	// 16- bit stack pointer
	uint16_t stackPointer = 0;
		
	// 16-bit program counter
	uint16_t programCounter = 0;

	// Current number of cycles; when this reaches a certain threshold, reset back to zero.
	int cycles = 0;
	
	/* REGISTER READ/WRITE */
	// BC Register Pair
	registerPair BC; 
	// DE Register Pair
	registerPair DE; 
	// HL Register Pair
	registerPair HL;

private:

	/* =================================================================== UTILITY =============================================================== */

	void openFile(std::string filename);

	// This function will use a massive switch case to work out the correct address to set in the address buffer.
	// It will then execute the correct opcode, and increment the program counter accordingly.
	void decode_execute(uint16_t opcode);

	// Will return the 16-bit immediate data. The lower byte is the first byte of data, and the higher byte is the second byte of immediate data.
	uint16_t read16bits(const uint16_t position);

	// Will write a 16-bit value into memory, given a position within range. Stores in big-endian, i.e. the first byte is the lower half and the second byte is the higher half.
	void write16bitsBE(uint16_t value, const uint16_t position);

	// Will return the value stored in big-endian at this position in memory. Hence, it will construct a 16-bit value where the high-byte is at this position, and the low-byte is one position along in memory.
	uint16_t readBEMemory(const uint16_t position);

	// Will split a 16-bit value into two 8-bit values which will be stored in an array, provided by reference.
	void splitValue(uint16_t value, uint8_t* arr);

	/* =================================================================== OPCODES =============================================================== */
	
	/* -------------------------------------------------------------- 8-BIT OPCODES -------------------------------------------------------------- */
	
	// 8-bit loading opcodes. Specifies a destination address as well as a value to store.
	void load(uint16_t addr, uint8_t val);

	// Load. Specifies a single register to load into, as well as a value to store.
	void load(std::bitset<8>* reg, uint8_t val);

	// Load the accumulator with an 8-bit value.
	void loadAccumulator(uint8_t val);

	// Load. Specifies a single register to load into, as well as a single register to load a value from.
	void load(std::bitset<8>* destination, std::bitset<8>* source);

	// Load. Specifies a register pair to load a value into, as well as a value to store.
	void load(registerPair* regPair, uint8_t val);

	// Load. Specifies a register pair to load a 16 bit value into, as well as the 16 bit value.
	void load(registerPair* regPair, uint16_t val);

	// Load. Specifies a register pair to load a 16 bit immediate value ALREADY IN MEMORY, relative to the memory position provided.
	void load(registerPair* regPair, int pos);

	// Load. Specifies a register pair to store the accumulator value INTO.
	void load(registerPair* destination);

	// Arithmetic add with immediate operand. These all store the values in A.
	void add(uint8_t value);

	// Arithmetic add. These store the values in A, with a value from one of the existing registers.
	void add(registerPair* source);

	// Arithmetic add. Specifies a register pair destination, as well as a source to add to the value in the destination already.
	void add(registerPair* destination, registerPair* source);

	// Arithmetic add with carry. These all store the values in A.
	void addc(uint8_t value);

	// Arithmetic add with carry. These store the values in A, with a value from one of the existing registers.
	void addc(registerPair* source);

	// Arithmetic subtract. These all store the values in A.
	void sub(uint8_t value);

	// Arithmetic subtract. These store the values in A, with a value from one of the existing registers.
	void sub(registerPair* source);

	// Arithmetic subtract with carry. These all store the values in A.
	void subc(uint8_t value);

	// Arithmetic subtract with carry. These store the values in A, with a value from one of the existing registers.
	void subc(registerPair* source);

	// Arithmetic increment. These take a pointer to the register to increment.
	void inc(registerPair* reg);

	// Arithmetic increment on a single register.
	void inc(std::bitset<8>* reg);
	
	// Arithmetic decrement. These take a pointer to the register to decrement.
	void dec(registerPair* reg);

	// Arithmetic decrement on a single register.
	void dec(std::bitset<8>* reg);

	// AND operation, 8-bit immediate address
	void AND(uint8_t operand);

	// AND operation, 

	// OR operation
	void OR(uint8_t operand);

	// Rotate the contents of this register to the left or right, modifying the appropriate bits. Send a 1 for rightwards shift, a 0 for leftwards shift.
	void rotateCarry(uint8_t* src, bool shiftRight);

	// Rotate the contents of this register pair, and use the carry flag.
	void rotateThroughCarry(uint8_t* src, bool shiftRight);

	// Return the signed value, according to Two's Complement, of the input 8-bit number:
	int8_t fromTC(uint8_t num);

	/* -------------------------------------------------------------- 16-BIT OPCODES -------------------------------------------------------------- */
};