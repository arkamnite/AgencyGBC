#include "pch.h"
#include <iostream>
#include <cstdint>
#include "../Emulator/src/CPU.cpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EmulatorTestSuite
{
	TEST_CLASS(EmulatorTestSuite)
	{
	public:
		
		CPU cpu;

		TEST_METHOD(LDBD8)
		{
			cpu.loadOpcode(0x0006);
			cpu.loadOpcode(0x00FA);
			cpu.resetPC();
			cpu.cycle();
			cpu.cycle();
			Assert::IsTrue(0x00FA == cpu.getRegisters()[1]);
		}

	};
}
