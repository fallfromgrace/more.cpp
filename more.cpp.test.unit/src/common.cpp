#include "CppUnitTest.h"

#include "common\string_format.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace common
{
	TEST_CLASS(common)
	{
	public:

		TEST_METHOD(string_format)
		{
			std::string result = format("result: %f, %s", 1.101, "123");
			Assert::IsTrue(result.size() == 21);
		}

	};
}