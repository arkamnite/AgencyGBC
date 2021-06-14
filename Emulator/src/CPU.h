#pragma once

#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <bitset>
#include <string>
#include <memory>

/* The Sharp CPU is emulated here. */

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

class CPU {
public:

	CPU();

	void reset();

	void cycle();

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

	/* OPCODES */

		
};




/*
class Regpair {
private:
	// LO-bitset
	std::unique_ptr<std::bitset<8>> low;

	// HI-bitset
	std::unique_ptr<std::bitset<8>> high;

public:
	Regpair(std::bitset<8> hireg, std::bitset<8> lowreg)
	{
		low = std::make_unique<std::bitset<8>>(lowreg);
		high = std::make_unique<std::bitset<8>>(hireg);
	}

	void hset(std::bitset<8> val)
	{
		high->std::bitset<8>::operator|=(val);
	}

	void lset(std::bitset<8> val)
	{
		low->std::bitset<8>::operator|=(val);
	}

	std::bitset<16> value()
	{
		std::bitset<16> val(0);
		std::bitset<16> l(low.get());
		std::bitset<16> h(high.get());
		val &= h;
		val <<= 8;
		val &= l;
		return val;
	}

};
*/
