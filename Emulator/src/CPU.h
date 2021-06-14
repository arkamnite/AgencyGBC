#pragma once

#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <bitset>

/* The Sharp CPU is emulated here. */

/*---------------------------------------------------------------------------------------------*/
/* MEMORY MAP (GBC) */
/*
* 32KB of work RAM
* 16KB of VRAM
* 
* 0000 - 3FFF: 16 KiB ROM bank 00 (from cartridge)
* 4000 - 7FFF: 16 KiB ROM bank 01-NN (from cartridge, switchable bank via the mapper)
* 8000 - 9FFF: 8KiB VRAM (in CGB mode, switchable bank 0/1)
* A000 - BFFF: 8KiB external RAM (from cartridge, switchable bank if any)
* C000 - CFFF: 4KiB work RAM
* D000 - DFFF: 4KiB work RAM (in CGB mode, switchable bank 1-7)
* EOOO - FDFF: Mirror of C000 ~ DFFF. Nintendo says use of this area is prohibited
* FE00 - FE9F: Sprite Attribute Table
* FEAO - FEFF: Not usable
* FF00 - FF7F: I/O registers
* FF80 - FFFE: High RAM
* FFFF - FFFF: INterrupt enable register
* 
* IO RANGES (GBC)
* 
*	START	END		APP.	PURPOSE
*	$FF00			DMG		Controller
	$FF01	$FF02	DMG		Communication
	$FF04	$FF07	DMG		Divider and Timer
	$FF10	$FF26	DMG		Sound
	$FF30	$FF3F	DMG		Waveform RAM
	$FF40	$FF4B	DMG		LCD
  [	$FF4F			CGB		VRAM Bank Select ]
	$FF50			DMG		Set to non-zero to disable boot ROM
	$FF51	$FF55	CGB		VRAM DMA
	$FF68	$FF69	CGB		BG / OBJ Palettes
	$FF70			CGB		WRAM Bank Select
* 
* 
*/

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

	// 16-bit BC bitset
	std::bitset<16> BC;

	// 16-bit DE bitset
	std::bitset<16> DE;

	// 16-bit HL bitset
	std::bitset<16> HL;

	// 16- bit stack pointer
	uint16_t stackPointer;

	// 16-bit program counter
	uint16_t programCounter;

	/* 
	* Think of creating a "register pair" template class which then has references for registers.
	*
		uint8_t regs[8];
		uint8_t &B = regs[0];
		uint8_t &C = regs[1];
		regpair<0,1> BC;
	*/

public:

	/* REGISTER READ/WRITE */
	
	// Load a value into the B register.
	void B(uint8_t val);
	// Read from the B register
	uint8_t B();
	// Load a value into the C register.
	void C(uint8_t val);
	// Read from the C register
	uint8_t C();
	// Load a value into the D register.
	void D(uint8_t val);
	// Read from the D register
	uint8_t D();
	// Load a value into the E register.
	void E(uint8_t val);
	// Read from the E register
	uint8_t E();
	// Load a value into the H register.
	void H(uint8_t val);
	// Read from the H register
	uint8_t H();
	// Load a value into the L register.
	void L(uint8_t val);
	// Read from the L register
	uint8_t L();



	/* OPCODES */

};