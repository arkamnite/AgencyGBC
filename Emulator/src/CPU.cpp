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
	acc << std::hex << std::uppercase << accumulator;

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


void CPU::decode_execute(uint16_t opcode)
{
	uint8_t val = (uint8_t)memory[programCounter + 1];
	uint16_t val16 = memory[programCounter + 1];
	registerPair spPair; // Create registerPair with value of the stack pointer.
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
		load(&BC);
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
	
	/* =================================== ROW 0x001x ==============================*/
	
	case 0x0012: // LD (DE),A
		load(&DE);
		programCounter += 1;
		cycles += 8;
		break;
	case 0x0013: // INC DE
		inc(&DE);
		programCounter += 1;
		cycles += 8;
		break;
	case 0x0019: // ADD HL, DE
		add(&HL, &DE);
		programCounter += 1;
		cycles += 8;
		break;
	case 0x001B: // DEC DE
		dec(&DE);
		programCounter += 1;
		cycles += 8;
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
		break;
	case 0x0048: // LD C, B
		break;
	case 0x0049: // LD C, C
		break;
	case 0x004A: // LD C, D
		break;
	case 0x004B: // LD C, E
		break;
	case 0x004C: // LD C, H
		break;
	case 0x004D: // LD C, L
		break;
	case 0x004E:
		break;
	case 0x004F:
		load(&BC.low, accumulator);
		programCounter++;
		cycles += 4;
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
