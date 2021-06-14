#pragma once

#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <bitset>
#include <string>
#include <memory>


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

	// This will load the contents of a ROM into the memory.
	void loadROM(std::string filename);

private:

	// 64 KiB of memory.
	uint16_t memory[65536];

	// 16-bit address buffer
	uint16_t addressBuffer;

	/* CPU registers and flags */
	
	// 8-bit accumulator (A)
	uint8_t accumulator;

	/* 8-bit status flags;
	* Bit 7: (Z) Zero
	* Bit 6: (N) Subtraction (BCD)
	* Bit 5: (H) Half-carry (BCD)
	* Bit 4: (C) Carry
	* */
	std::bitset<8> flags;
		
	// 16- bit stack pointer
	uint16_t stackPointer;

	// 16-bit program counter
	uint16_t programCounter;
	
	/* REGISTER READ/WRITE */
	// BC Register Pair
	registerPair BC; 
	// DE Register Pair
	registerPair DE; 
	// HL Register Pair
	registerPair HL;

protected:

	/* =================================================================== UTILITY =============================================================== */

	void openFile(std::string filename);

	// This function will use a massive switch case to work out the correct address to set in the address buffer.
	// It will then execute the correct opcode, and increment the program counter accordingly.
	void decode_execute(uint16_t opcode);

	/* =================================================================== OPCODES =============================================================== */
	
	/* -------------------------------------------------------------- 8-BIT OPCODES -------------------------------------------------------------- */
	
	// 8-bit loading opcodes. Specifies a destination address as well as a value to store.
	void load8bit(uint16_t addr, uint8_t val);

	// 8-bit arithmetic add. These all store the values in A.
	void add8bit(uint8_t value);

	// 8-bit arithmetic add. These store the values in A, with a value from one of the existing registers.
	void add8bit(registerPair* source);

	// 8-bit arithmetic add with carry. These all store the values in A.
	void addc8bit(uint8_t value);

	// 8-bit arithmetic add with carry. These store the values in A, with a value from one of the existing registers.
	void addc8bit(registerPair* source);

	// 8-bit arithmetic subtract. These all store the values in A.
	void sub8bit(uint8_t value);

	// 8-bit arithmetic subtract. These store the values in A, with a value from one of the existing registers.
	void sub8bit(registerPair* source);

	// 8-bit arithmetic subtract with carry. These all store the values in A.
	void subc8bit(uint8_t value);

	// 8-bit arithmetic subtract with carry. These store the values in A, with a value from one of the existing registers.
	void subc8bit(registerPair* source);

	// 8-bit arithmetic increment. These take a pointer to the register to increment.
	void inc8bit(registerPair* reg);
	
	// 8-bit arithmetic decrement. These take a pointer to the register to decrement.
	void dec8bit(registerPair* reg);



	/* -------------------------------------------------------------- 16-BIT OPCODES -------------------------------------------------------------- */
};