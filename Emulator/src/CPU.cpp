#include "CPU.h"

CPU::CPU()
{
}

void CPU::reset()
{
}

void CPU::cycle()
{
}

void CPU::decode_execute(uint16_t opcode)
{
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

void CPU::load(std::bitset<8>* destination, std::bitset<8>* source)
{
}

void CPU::load(registerPair* regPair, uint8_t val)
{
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
