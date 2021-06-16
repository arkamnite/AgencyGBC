#include "pch.h"
#include "../Emulator/src/CPU.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EmulatorTestSuite
{
	TEST_CLASS(EmulatorTestSuite)
	{
	public:
		
		CPU cpu;

		TEST_METHOD(TestMethod1)
		{
		}

		TEST_METHOD(LoadSingleRegisterValue)
		{
			uint8_t value = 0b10010010;
			cpu.load(&cpu.BC.low, value);
			Assert::IsTrue(cpu.BC.low == std::bitset<8>(value));
		}

	};
}
