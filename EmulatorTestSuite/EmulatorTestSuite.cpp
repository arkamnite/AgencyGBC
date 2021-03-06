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
			Assert::IsTrue(0x0000 == cpu.getRegisters()[3]);
		}
				
		TEST_METHOD(LOAD_STACKPOINTER)
		{
			cpu.loadOpcode(0x0008);
			cpu.loadOpcode(0x0FAF);
			cpu.setSP(0x08AB);
			cpu.resetPC();
			cpu.cycle();
			cpu.cycle();
			cpu.cycle();
			Assert::IsTrue(cpu.readMemory(0x0FAF) == 0x0008);
			Assert::IsTrue(cpu.readMemory(0x0FB0) == 0x00AB);
		}



	};
}
