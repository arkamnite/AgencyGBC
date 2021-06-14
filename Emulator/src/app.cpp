#include <stdio.h>
#include <iostream>
#include <chrono>
#include "CPU.h"

int main()
{
	std::cout << "hello world! This is the GB emulator." << std::endl;
	int x = 0;
	std::cin >> x;
	std::cout << x << std::endl;
	
	CPU cpu;
	cpu.B(0b10010010);

	std::cin >> x;
}