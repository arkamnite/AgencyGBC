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

void CPU::cycle()
{
	uint16_t op = memory[programCounter];
	decode_execute(op);
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

std::vector<std::string> CPU::getRegisterValues()
{
	std::vector<std::string> regvals = {};

	std::stringstream acc;
	acc << std::setfill('0') << std::setw(9) << std::hex << accumulator;

	regvals.push_back("Accumulator: " + acc.str());
	regvals.push_back("BC: " + BC.getPair().to_string());
	regvals.push_back("DE: " + DE.getPair().to_string());
	regvals.push_back("HL: " + HL.getPair().to_string());
	regvals.push_back("Cycles: " + std::to_string(cycles));
	regvals.push_back("\n");
	return regvals;
}

void CPU::decode_execute(uint16_t opcode)
{
	uint8_t val = (uint8_t)memory[programCounter + 1];
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

	/* =================================== ROW 0x002x ==============================*/

	/* =================================== ROW 0x003x ==============================*/
	
	case 0x003e:
		loadAccumulator(val);
		programCounter += 2;
		cycles += 8;
		break;
	
	/* =================================== ROW 0x004x ==============================*/
	
	/* Register to register load. 1 byte, 4 cycles. */
	case 0x0040:
		load(&BC.high, &BC.high);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0041:
		load(&BC.high, &BC.low);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0042:
		load(&BC.high, &DE.high);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0043:
		load(&BC.high, &DE.low);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0044:
		load(&BC.high, &HL.high);
		programCounter += 1;
		cycles += 4;
		break;
	case 0x0045:
		load(&BC.high, &HL.low);
		programCounter += 1;
		cycles += 4;
		break;
	/* Load a value from one register to a register PAIR. 1 byte, 8 cycles. */
	case 0x0046:
		break;
	case 0x0047:
		break;
	case 0x0048:
		break;
	case 0x0049:
		break;
	case 0x004A:
		break;
	case 0x004B:
		break;
	case 0x004C:
		break;
	case 0x004D:
		break;
	case 0x004E:
		break;
	case 0x004F:
		load(&BC.low, accumulator);
		programCounter++;
		cycles += 4;
		break;
	}
}

uint16_t CPU::read16bits(int position)
{
	// TODO: Implement bounds check.
	uint16_t low = memory[position];
	uint16_t high = memory[++position];
	high <<= 8;
	return uint16_t(high + low);
}

/* =================================================================== OPCODES =============================================================== */

void CPU::load(uint16_t addr, uint8_t val)
{

}

void CPU::load(std::bitset<8>* reg, uint8_t val)
{
	reg->operator&=(0b00000000);
	reg->operator|=(val);
}

void CPU::loadAccumulator(uint8_t val)
{
	accumulator = val;
}

void CPU::load(std::bitset<8>* destination, std::bitset<8>* source)
{
	destination->operator&=(0);
	for (int i = 0; i < 8; i++)
		destination->set(i, source->operator[](i));

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

void CPU::dec(registerPair* reg)
{
}

void CPU::AND(uint8_t operand)
{
}

void CPU::OR(uint8_t operand)
{
}
