#include "CPU.h"

CPU::CPU()
{
}

void CPU::B(uint8_t val)
{
	std::bitset<16> bValue(val);
	std::cout << "B  value: " << bValue.to_string() << std::endl;
	bValue <<= 8;
	std::cout << "B  value: " << bValue.to_string() << std::endl;
	std::cout << "BC value: " << BC.to_string() << std::endl;
	BC ^= bValue;
	std::cout << "BC value: " << BC.to_string() << std::endl;
}

uint8_t CPU::B()
{
	return uint8_t();
}

void CPU::C(uint8_t val)
{
}

uint8_t CPU::C()
{
	return uint8_t();
}

void CPU::D(uint8_t val)
{
}

uint8_t CPU::D()
{
	return uint8_t();
}

void CPU::E(uint8_t val)
{
}

uint8_t CPU::E()
{
	return uint8_t();
}

void CPU::H(uint8_t val)
{
}

uint8_t CPU::H()
{
	return uint8_t();
}

void CPU::L(uint8_t val)
{
}

uint8_t CPU::L()
{
	return uint8_t();
}
