#pragma once

#include <iostream>
#include <stdio.h>
#include <cstdint>

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

private:

};