#include "CppUnitTest.h"

#include "more\string.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace more
{
	TEST_CLASS(string)
	{
	public:

		TEST_METHOD(string_format)
		{
			std::string result = more::format("result: %f, %s", 1.101, "123");
			Assert::IsTrue(result.size() == 21);
		}

	};
}