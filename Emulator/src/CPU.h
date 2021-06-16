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
	void load(uint16_t addr, uint8_t val);

	// Load. Specifies a single register to load into, as well as a value to store.
	void load(std::bitset<8>* reg, uint8_t val);

	// Load. Specifies a register pair to load a value into, as well as a value to store.
	void load(registerPair* regPair, uint8_t val);

	// Arithmetic add with immediate operand. These all store the values in A.
	void add(uint8_t value);

	// Arithmetic add. These store the values in A, with a value from one of the existing registers.
	void add(registerPair* source);

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
	
	// Arithmetic decrement. These take a pointer to the register to decrement.
	void dec(registerPair* reg);

	// AND operation, 8-bit immediate address
	void AND(uint8_t operand);

	// OR operation
	void OR(uint8_t operand);



	/* -------------------------------------------------------------- 16-BIT OPCODES -------------------------------------------------------------- */
};