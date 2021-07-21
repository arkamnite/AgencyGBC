#include "CPU.h"

CPU::CPU()
{
}

void CPU::reset()
{
	accumulator = 0;
	cycles = 0;
	programCounter = 0;
	addressBuffer = 0;
	stackPointer = 0;
	flags.operator&=(0b00000000);
	BC.setPair(0x0000);
	DE.setPair(0x0000);
	HL.setPair(0x0000);
}

std::string regToHex(registerPair* reg)
{
	std::stringstream regString;
	regString << std::hex << std::uppercase << reg->getPair().to_ullong();
	return regString.str();
}

std::vector<std::string> CPU::getRegisterValues()
{
	std::vector<std::string> regvals = {};

	std::stringstream acc;
	unsigned long acclong = accumulator;
	acc << std::hex << std::uppercase << acclong;

	regvals.push_back("Accumulator: " + acc.str());
	regvals.push_back("BC: " + regToHex(&BC));
	regvals.push_back("DE: " + regToHex(&DE));
	regvals.push_back("HL: " + regToHex(&HL));
	regvals.push_back("PC: " + std::to_string(programCounter));

	std::stringstream opS;
	opS << std::hex << std::uppercase << memory[programCounter - 1];

	regvals.push_back("Last Opcode: " + opS.str());
	regvals.push_back("Cycles: " + std::to_string(cycles));
	regvals.push_back("\n");
	return regvals;
}

std::vector<uint8_t> CPU::getRegisters()
{
	std::vector<uint8_t> values;
	values.push_back(accumulator);
	values.push_back(BC.high.to_ulong());
	values.push_back(BC.low.to_ulong());
	values.push_back(DE.high.to_ulong());
	values.push_back(DE.low.to_ulong());
	values.push_back(HL.high.to_ulong());
	values.push_back(HL.low.to_ulong());
	return values;
}

void CPU::setSP(uint16_t val)
{
	stackPointer = val;
}

uint16_t CPU::getSP()
{
	return stackPointer;
}

uint16_t CPU::readMemory(uint16_t pos)
{
	if (pos >= MEMORY_MAX_RANGE)
		return 0;
	else
		return memory[pos];
}

void CPU::cycle()
{
	uint16_t op = memory[programCounter];
	decode_execute(op);
}

void CPU::cycleLog()
{

	cycle();

	std::vector<std::string> v = getRegisterValues();
	for (const auto& i : v)
		std::cout << i << '\n';
}

void CPU::loadOpcode(uint16_t opcode)
{
	memory[programCounter] = opcode;
	programCounter++;
}

void CPU::resetPC()
{
	programCounter = 0;
}

void CPU::resetMemory()
{
	memset(memory, 0x00, MEMORY_MAX_RANGE);
}


void CPU::decode_execute(uint16_t opcode)
{
	uint8_t val = (uint8_t)memory[programCounter + 1]; // 8-bit value found at the next location in memory.
	uint16_t val16 = memory[programCounter + 1]; // 16-bit value found at the next location in memory.
	registerPair spPair; // Create registerPair with value of the stack pointer.
	uint16_t s8 = readMemory(programCounter + 1);
	s8 >>= 8;
	spPair.setPair(stackPointer);
	switch (opcode) {

	/* =================================== ROW 0x000x ==============================*/

	case 0x0000: // NOP
		programCounter++;
		cycles += 4;
		break;
	case 0x0001: // LD BC,d16
		load(&BC, programCounter + 1);
		programCounter += 3;
		cycles += 12;
		break;
	case 0x0002: // LD (BC),A
		//load(&BC);
		memory[BC.toInt()] = accumulator;
		programCounter += 1;
		cycles += 8;
		break;
	case 0x0003: // INC BC
		inc(&BC);
		programCounter += 1;
		cycles += 8;
		break;
	case 0x0004: // INC B
		inc(&BC.high);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0005: // DEC B
		dec(&BC.low);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0006: // LD B,d8
		load(&BC.high, val);
		programCounter += 2;
		cycles += 4;
		break;
	case 0x0007: // TODO: RLCA
		rotateCarry(&accumulator, false);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0008: // TODO: LD (a16), SP
		write16bitsBE(stackPointer, val16);
		programCounter += 3;
		cycles += 20;
		break;
	case 0x0009: // ADD HL, BC
		add(&HL, &BC);
		programCounter += 1;
		cycles += 8;
		break;
	case 0x000B: // DEC BC
		dec(&BC);
		programCounter += 1;
		cycles += 8;
		break;
	case 0x000F: // RRCA TODO: CHECK
		rotateCarry(&accumulator, true);
		programCounter += 1;
		cycles += 4;
		break;
	
	/* =================================== ROW 0x001x ==============================*/
	
	case 0x0011: // LD DE, d16 TODO: Find out what the hell is going on here
		DE.setPair(read16bits(programCounter + 1));
		programCounter += 3;
		cycles += 12;
	case 0x0012: // LD (DE),A 
		//load(&DE);
		memory[DE.toInt()] = accumulator;
		programCounter += 1;
		cycles += 8;
		break;
	case 0x0013: // INC DE
		inc(&DE);
		programCounter += 1;
		cycles += 8;
		break;
	case 0x0017: // RLA
		rotateThroughCarry(&accumulator, false);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0018: // JR, s8
		//std::cout << "Begin PC jump: " << programCounter << " step: " << +fromTC((uint8_t)readMemory(programCounter + 1))  << std::endl;
		programCounter = programCounter + fromTC(val);
		//std::cout << "Completed PC jump: " << programCounter << std::endl;
		cycles += 12;
		break;
	case 0x0019: // ADD HL, DE
		add(&HL, &DE);
		programCounter += 1;
		cycles += 8;
		break;
	case 0x001A: // LD A, (DE)
		accumulator = (uint8_t)readMemory((uint16_t)DE.toInt());
		programCounter += 1;
		cycles += 8;
		break;
	case 0x001B: // DEC DE
		dec(&DE);
		programCounter += 1;
		cycles += 8;
		break;
	case 0x001C: // INC E
		inc(&DE.low);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x001D: // DEC E
		dec(&DE.low);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x001E: // LD E, d8
		DE.setLow(val);
		programCounter += 2;
		cycles += 8;
		break;
	case 0x001F: // RRA
		rotateThroughCarry(&accumulator, true);
		programCounter += 1;
		cycles += 4;
		break;

	/* =================================== ROW 0x002x ==============================*/
	case 0x0029: // ADD HL, HL
		add(&HL, &HL);
		programCounter += 1;
		cycles += 8;
		break;
	case 0x002B: // DEC HL
		dec(&HL);
		programCounter += 1;
		cycles += 8;
		break;

	/* =================================== ROW 0x003x ==============================*/
	
	case 0x003e: // LD A, d8
		loadAccumulator(val);
		programCounter += 2;
		cycles += 8;
		break;
	case 0x0039: // ADD HL, SP TODO: Check this works
		add(&HL, &spPair);
		programCounter += 1;
		cycles += 8;
		break;
	case 0x003B: // DEC SP
		stackPointer--; // Will cause overflow lmao
		programCounter += 1;
		cycles += 8;
		break;
	
	/* =================================== ROW 0x004x ==============================*/
	
	/* Register to register load. 1 byte, 4 cycles. */
	case 0x0040: // LD B, B
		load(&BC.high, &BC.high);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0041: // LD B, C
		load(&BC.high, &BC.low);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0042: // LD B, D
		load(&BC.high, &DE.high);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0043: // LD B, E
		load(&BC.high, &DE.low);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0044: // LD B, H
		load(&BC.high, &HL.high);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0045: // LD B, L
		load(&BC.high, &HL.low);
		programCounter += 1;
		cycles += 4;
		break;
	/* Load a value from one register to a register PAIR. 1 byte, 8 cycles. */
	case 0x0046: // LD B, HL
		break;
	case 0x0047: // LD B, A
		load(&BC.high, accumulator);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0048: // LD C, B
		load(&BC.low, &BC.high);
		break;
	case 0x0049: // LD C, C
		load(&BC.low, &BC.low);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x004A: // LD C, D
		load(&BC.low, &DE.high);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x004B: // LD C, E
		load(&BC.low, &DE.low);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x004C: // LD C, H
		load(&BC.low, &HL.high);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x004D: // LD C, L
		load(&BC.low, &HL.low);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x004E: // LD C, (HL) TODO: How does this work? Which 8 bits are we storing?
		load(&BC.low, (uint8_t)read16bits(HL.toInt()));
		programCounter += 1;
		cycles += 8;
		break;
	case 0x004F: // LD C, A
		load(&BC.low, accumulator);
		programCounter++;
		cycles += 4;
		break;

	/* =================================== ROW 0x005x ==============================*/
	case 0x0050: // LD D, B
		break;
	case 0x0051: // LD D, C
		break;
	case 0x0052: // LD D, D
		break;
	case 0x0053: // LD D, E
		break;
	case 0x0054: // LD D, H
		break;
	case 0x0055: // LD D, L
		break;
	case 0x0056: // LD D, (HL)
		break;
	case 0x0057: // LD D, A
		break;
	case 0x0058: // LD E, B
		break;
	case 0x0059: // LD E, C
		break;
	case 0x005A: // LD E, D
		break;
	case 0x005B: // LD E, E
		break;
	case 0x005C: // LD E, H
		break;
	case 0x005D: // LD E, L
		break;
	case 0x005E: // LD E, (HL)
		break;
	case 0x005F: // LD E, A
		break;
	
	/* =================================== ROW 0x006x ==============================*/
	case 0x0060: // LD H, B
		break;
	case 0x0061: // LD H, C
		break;
	case 0x0062: // LD H, D
		break;
	case 0x0063: // LD H, E
		break;
	case 0x0064: // LD H, H
		break;
	case 0x0065: // LD H, L
		break;
	case 0x0066: // LD H, (HL)
		break;
	case 0x0067: // LD H, A
		break;
	case 0x0068: // LD L, B
		break;	 
	case 0x0069: // LD L, C
		break;	 
	case 0x006A: // LD L, D
		break;	 
	case 0x006B: // LD L, E
		break;	 
	case 0x006C: // LD L, H
		break;	 
	case 0x006D: // LD L, L
		break;	 
	case 0x006E: // LD L, (HL)
		break;	 
	case 0x006F: // LD L, A
		break;

	/* =================================== ROW 0x007x ==============================*/
	case 0x0070: // LD H, B
		break;	 
	case 0x0071: // LD H, C
		break;	 
	case 0x0072: // LD H, D
		break;	 
	case 0x0073: // LD H, E
		break;	 
	case 0x0074: // LD H, H
		break;	 
	case 0x0075: // LD H, L
		break;	 
	case 0x0076: // LD H, (HL)
		break;	 
	case 0x0077: // LD H, A
		break;	 
	case 0x0078: // LD L, B
		break;	 
	case 0x0079: // LD L, C
		break;	 
	case 0x007A: // LD L, D
		break;	 
	case 0x007B: // LD L, E
		break;	 
	case 0x007C: // LD L, H
		break;	 
	case 0x007D: // LD L, L
		break;	 
	case 0x007E: // LD L, (HL)
		break;	 
	case 0x007F: // LD L, A
		break;

		/* =================================== ROW 0x008x ==============================*/
	case 0x0080: // ADD A, B
		break;
	case 0x0081: // ADD A, C
		break;
	case 0x0082: // ADD A, D
		break;
	case 0x0083: // ADD A, E
		break;
	case 0x0084: // ADD A, L
		break;
	case 0x0085: // ADD A, L
		break;
	case 0x0086: // ADD A, HL
		break;
	case 0x0087: // ADD A, A
		break;
	case 0x0088: // ADC A, B
		break;
	case 0x0089: // ADC A, C
		break;
	case 0x008A: // ADC A, D
		break;
	case 0x008B: // ADC A, E
		break;
	case 0x008C: // ADC A, H
		break;
	case 0x008D: // ADC A, L
		break;
	case 0x008E: // ADC A, (HL)
		break;
	case 0x008F: // ADC A, A
		break;

		/* =================================== ROW 0x009x ==============================*/
	case 0x0090: // SUB B
		break;
	case 0x0091: // SUB C
		break;
	case 0x0092: // SUB D
		break;
	case 0x0093: // SUB E
		break;
	case 0x0094: // SUB H
		break;
	case 0x0095: // SUB L
		break;
	case 0x0096: // SUB (HL)
		break;
	case 0x0097: // SUB A
		break;
	case 0x0098: // SUBC A, B
		break;
	case 0x0099: // SUBC A, C
		break;
	case 0x009A: // SUBC A, D
		break;
	case 0x009B: // SUBC A, E
		break;
	case 0x009C: // SUBC A, H
		break;
	case 0x009D: // SUBC A, L
		break;
	case 0x009E: // SUBC A, (HL)
		break;
	case 0x009F: // SUBC A, A
		break;

		/* =================================== ROW 0x00Ax ==============================*/
	case 0x00A0: // AND B
		break;
	case 0x00A1: // AND C
		break;
	case 0x00A2: // AND D
		break;
	case 0x00A3: // AND E
		break;
	case 0x00A4: // AND H
		break;
	case 0x00A5: // AND L
		break;
	case 0x00A6: // AND (HL)
		break;
	case 0x00A7: // AND A
		break;
	case 0x00A8: // XOR B
		break;	 
	case 0x00A9: // XOR C
		break;	 
	case 0x00AA: // XOR D
		break;	 
	case 0x00AB: // XOR E
		break;	 
	case 0x00AC: // XOR H
		break;	 
	case 0x00AD: // XOR L
		break;	 
	case 0x00AE: // XOR (HL)
		break;
	case 0x00AF: // XOR A
		break;

		/* =================================== ROW 0x00Bx ==============================*/
	case 0x00B0:
		break;
	case 0x00B1:
		break;
	case 0x00B2:
		break;
	case 0x00B3:
		break;
	case 0x00B4:
		break;
	case 0x00B5:
		break;
	case 0x00B6:
		break;
	case 0x00B7:
		break;
	case 0x00B8:
		break;
	case 0x00B9:
		break;
	case 0x00BA:
		break;
	case 0x00BB:
		break;
	case 0x00BC:
		break;
	case 0x00BD:
		break;
	case 0x00BE:
		break;
	case 0x00BF:
		break;

		/* =================================== ROW 0x00Cx ==============================*/
	case 0x00C0:
		break;
	case 0x00C1:
		break;
	case 0x00C2:
		break;
	case 0x00C3:
		break;
	case 0x00C4:
		break;
	case 0x00C5:
		break;
	case 0x00C6:
		break;
	case 0x00C7:
		break;
	case 0x00C8:
		break;
	case 0x00C9:
		break;
	case 0x00CA:
		break;
	case 0x00CB:
		break;
	case 0x00CC:
		break;
	case 0x00CD:
		break;
	case 0x00CE:
		break;
	case 0x00CF:
		break;

		/* =================================== ROW 0x00Dx ==============================*/
	case 0x00D0:
		break;
	case 0x00D1:
		break;
	case 0x00D2:
		break;
	case 0x00D3:
		break;
	case 0x00D4:
		break;
	case 0x00D5:
		break;
	case 0x00D6:
		break;
	case 0x00D7:
		break;
	case 0x00D8:
		break;
	case 0x00D9:
		break;
	case 0x00DA:
		break;
	case 0x00DB:
		break;
	case 0x00DC:
		break;
	case 0x00DD:
		break;
	case 0x00DE:
		break;
	case 0x00DF:
		break;

		/* =================================== ROW 0x00Ex ==============================*/
	case 0x00E0:
		break;
	case 0x00E1:
		break;
	case 0x00E2:
		break;
	case 0x00E3:
		break;
	case 0x00E4:
		break;
	case 0x00E5:
		break;
	case 0x00E6:
		break;
	case 0x00E7:
		break;
	case 0x00E8:
		break;
	case 0x00E9:
		break;
	case 0x00EA:
		break;
	case 0x00EB:
		break;
	case 0x00EC:
		break;
	case 0x00ED:
		break;
	case 0x00EE:
		break;
	case 0x00EF:
		break;

		/* =================================== ROW 0x00Fx ==============================*/
	case 0x00F0:
		break;
	case 0x00F1:
		break;
	case 0x00F2:
		break;
	case 0x00F3:
		break;
	case 0x00F4:
		break;
	case 0x00F5:
		break;
	case 0x00F6:
		break;
	case 0x00F7:
		break;
	case 0x00F8:
		break;
	case 0x00F9:
		break;
	case 0x00FA:
		break;
	case 0x00FB:
		break;
	case 0x00FC:
		break;
	case 0x00FD:
		break;
	case 0x00FE:
		break;
	case 0x00FF:
		break;
	default:
		programCounter++;
		cycles += 4;
		break;
	}
}

uint16_t CPU::read16bits(const uint16_t position)
{
	// Initialise both values to 0 in case the memory address is out of range.
	uint16_t low = 0;
	uint16_t high = 0;

	// Perform bounds check on the memory- do this only once.
	if ((position + 1) < (MEMORY_MAX_RANGE - 1))
	{
		low = memory[position];
		high = memory[position + 1];
	}

	// Shift high-bits left by 8 bits.
	high <<= 8;
	return uint16_t(high + low);
}

void CPU::write16bitsBE(uint16_t value, const uint16_t position)
{
	// Perform bounds checking on the memory
	if ((position+1) < (MEMORY_MAX_RANGE - 1))
	{
		uint8_t arr[2] = { 0 };
		splitValue(value, arr);
		memory[position] = arr[1];
		memory[position + 1] = arr[0];
	}
}

uint16_t CPU::readBEMemory(const uint16_t position)
{
	uint16_t val = 0;
	if ((position + 1) < (MEMORY_MAX_RANGE - 1))
	{
		val = memory[position];
		val <<= 8;
		val += memory[position + 1];
	}
	return val;
}

void CPU::splitValue(uint16_t value, uint8_t* arr)
{
	arr[0] = *((uint8_t*)&(value));
	arr[1] = *((uint8_t*)&(value)+1);
}

/* =================================================================== OPCODES =============================================================== */

/* TODO: Check all flags are correctly RESET or ZEROED */

void CPU::load(uint16_t addr, uint8_t val)
{

}

void CPU::load(std::bitset<8>* reg, uint8_t val)
{
	*reg = val;
}

void CPU::loadAccumulator(uint8_t val)
{
	accumulator = val;
}

void CPU::load(std::bitset<8>* destination, std::bitset<8>* source)
{
	/*destination->operator&=(0);
	for (int i = 0; i < 8; i++)
		destination->set(i, source->operator[](i));*/
	*destination = *source;

}

void CPU::load(registerPair* regPair, uint8_t val)
{
	regPair->setPair(val);
}

void CPU::load(registerPair* regPair, uint16_t val)
{
	regPair->setPair(val);
}

void CPU::load(registerPair* regPair, int pos)
{
	regPair->setPair(read16bits(pos));
}

void CPU::load(registerPair* destination)
{
	/* TODO: Check that you are setting this value correctly. */
	destination->setHigh(0);
	destination->setLow(accumulator);
}

void CPU::add(uint8_t value)
{
}

void CPU::add(registerPair* source)
{
}

void CPU::add(registerPair* destination, registerPair* source)
{
	// Reset the subtraction flag in all instances of adding two registers together.
	flags.reset(6);
	uint16_t opA = (uint16_t)destination->toInt();
	uint16_t opB = (uint16_t)source->toInt();

	// Add the two results and store them in the destination register.
	destination->setPair(opA + opB);

	// Set the carry flag if required:
	if (opA + opB > UINT16_MAX)
		flags.set(4, true);

	// TODO: Check BCD addition stuff
}

void CPU::addc(uint8_t value)
{
}

void CPU::addc(registerPair* source)
{
}

void CPU::sub(uint8_t value)
{
}

void CPU::sub(registerPair* source)
{
}

void CPU::subc(uint8_t value)
{
}

void CPU::subc(registerPair* source)
{
}

void CPU::inc(registerPair* reg)
{
	uint16_t value = reg->toInt();
	reg->setPair(++value);

	// Reset the SUB flag
	flags.reset(6);

}

void CPU::inc(std::bitset<8>* reg)
{
	int val = (int) reg->to_ulong();
	*reg = ++val;
}

void CPU::dec(registerPair* reg)
{
	uint16_t value = reg->toInt();
	reg->setPair(--value);
}

void CPU::dec(std::bitset<8>* reg)
{
	int val = (int)reg->to_ulong();
	*reg = --val;
}

void CPU::AND(uint8_t operand)
{
}

void CPU::OR(uint8_t operand)
{
}

void CPU::rotateCarry(uint8_t* src, bool shiftRight)
{
	// Create a local copy of the register.
	uint8_t val = *src;
	
	// Store the value of bit 7 and bit 0
	uint8_t bit7 = val & 0b10000000;
	uint8_t bit0 = val & 0b00000001;
	
	// Shift all bits leftwards or rightwards, depending on the boolean.
	shiftRight ? *src >>= 1 : *src <<= 1 ;

	// Toggle bit 0 if we have shifted left, and bit 7 if we have shifted right.
	shiftRight ? *src = (*src | bit0) : *src = (*src | bit7);

	// Toggle the carry bit as required
	if (shiftRight)
	{
		flags.set(4, (bit0 == 0b0000001));
	}
	else
	{
		flags.set(4, (bit7 == 0b1000000));
	}
	
	// Reset the other flags as required
	flags.reset(7); // Zero flag
	flags.reset(6); // SUB
	flags.reset(5); // Half carry

}

void CPU::rotateThroughCarry(uint8_t* src, bool shiftRight)
{
	// Rotate all bits, then set 0 to the contents of the carry flag.
	shiftRight ? *src >>= 1 : *src <<= 1;

	// Toggle the lowest or highest bit depending on the carry flag.
	if (flags.test(4) && !shiftRight)
		*src = *src | 0b00000001;
	else if (flags.test(4) && shiftRight)
		*src = *src | 0b10000000;

	// Reset the other flags as required
	flags.reset(7); // Zero flag
	flags.reset(6); // SUB
	flags.reset(5); // Half carry
}

int8_t CPU::fromTC(uint8_t num)
{
	if ((num & 0b10000000) != 0b10000000) // If the number is negative in TC
	{
		return int8_t(num);
	}
	else // The number has a leading 1, i.e. it is negative. 
	{
		// Using bitset to speed up these operations
		std::bitset<8> toBeFlipped(num);
		for (int i = 0; i < 8; i++)
		{
			toBeFlipped.flip(i);
		}

		return (int8_t)(-1 * (toBeFlipped.to_ulong() + 1));
	}
}
