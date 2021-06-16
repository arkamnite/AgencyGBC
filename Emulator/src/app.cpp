#include <stdio.h>
#include <iostream>
#include <chrono>
#include "CPU.h"

int main()
{
	std::cout << "hello world! This is the GB emulator." << std::endl;
	int x = 0;
	
	CPU cpu;

	/*std::cout <<"BC Pair: " << cpu.BC.getPair() << std::endl;
	cpu.BC.setHigh(0b10101011);
	std::cout << "BC Pair: " << cpu.BC.getPair() << std::endl;
	cpu.BC.setLow(0b10001011);
	std::cout << "BC Pair: " << cpu.BC.getPair() << std::endl;
	std::cout << "B value: " << cpu.BC.high << std::endl;
	std::cout << "C value: " << cpu.BC.low << std::endl;*/

	cpu.cycle();

	uint8_t value = 0b10010010;
	std::cout << cpu.BC.getPair().to_string() << "\n" << std::bitset<8>(value).to_string() << std::endl;
	cpu.load(&cpu.BC.low, value);
	std::cout << cpu.BC.getPair().to_string() << "\n" << std::bitset<8>(value).to_string() << std::endl;

	std::cin >> x;
}