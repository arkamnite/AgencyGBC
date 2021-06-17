#include <stdio.h>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include "CPU.h"

void printReg(CPU* cpu)
{
	std::vector<std::string> v = cpu->getRegisterValues();
	for (const auto& i : v)
		std::cout << i << '\n';
}

int main()
{
	std::cout << "hello world! This is the GB emulator." << std::endl;
	int x = 0;
	
	/*uint8_t val1 = 0x00FA;
	uint16_t val2 = 0x1000;
	uint16_t val3 = val1 + val2;
	uint8_t castTest = (uint8_t)0xAFFE;
	std::cout << std::hex << val3 << std::endl;
	std::cout << std::hex << castTest << std::endl;*/

	CPU cpu;
	cpu.reset();
	/*cpu.loadOpcode(0x0004);
	cpu.loadOpcode(0x0004);
	cpu.loadOpcode(0x0004);
	cpu.loadOpcode(0x0004);
	cpu.loadOpcode(0x0004);*/
	/*cpu.loadOpcode(0x0001);
	cpu.loadOpcode(0x00FF);
	cpu.loadOpcode(0x00AA);
	cpu.loadOpcode(0x003E);
	cpu.loadOpcode(0x004F);
	cpu.loadOpcode(0x004F);
	cpu.loadOpcode(0x0041);*/

	/*std::cout <<"BC Pair: " << cpu.BC.getPair() << std::endl;
	cpu.BC.setHigh(0b10101011);
	std::cout << "BC Pair: " << cpu.BC.getPair() << std::endl;
	cpu.BC.setLow(0b10001011);
	std::cout << "BC Pair: " << cpu.BC.getPair() << std::endl;
	std::cout << "B value: " << cpu.BC.high << std::endl;
	std::cout << "C value: " << cpu.BC.low << std::endl;*/
	/*cpu.resetPC();
	printReg(&cpu);
	std::cin;
	cpu.cycle();
	printReg(&cpu);
	std::cin;
	cpu.cycle();
	printReg(&cpu);
	std::cin;
	cpu.cycle();
	printReg(&cpu);
	std::cin;
	cpu.cycle();
	printReg(&cpu);
	std::cin;
	cpu.cycle();
	printReg(&cpu);
	std::cin;
	cpu.cycle();
	printReg(&cpu);
	std::cin;
	cpu.cycle();
	cpu.cycle();
	cpu.cycle();
	cpu.cycle();
	cpu.cycle();
	printReg(&cpu);*/

	cpu.loadOpcode(0x0006);
	cpu.loadOpcode(0x00FA);
	cpu.loadOpcode(0x0004);
	cpu.loadOpcode(0x0003);
	cpu.resetPC();

	/*cpu.cycle();
	printReg(&cpu);
	cpu.cycle();
	printReg(&cpu);
	cpu.cycle();
	printReg(&cpu);
	cpu.cycle();
	printReg(&cpu);
	cpu.cycle();
	printReg(&cpu);*/

	cpu.cycleLog();
	cpu.cycleLog();
	cpu.cycleLog();
	cpu.cycleLog();

	/*cpu.cycle();
	cpu.cycle();
	cpu.cycle();
	cpu.cycle();*/

	std::cin >> x;
}
