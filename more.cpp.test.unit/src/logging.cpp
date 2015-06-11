#include "CppUnitTest.h"
#include <thread>

#include "common\string_format.hpp"
#include "chrono\date.hpp"
#include "logging\log.hpp"
#include "win32\event.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace logging
{		
	TEST_CLASS(logging_tests)
	{
	public:

		TEST_METHOD(log_async_assert)
		{
			win32::manual_reset_event logging_complete;
			int count = 0;
			add_logger([&](const event& e)
			{
				auto result = common::format(
					"%i | %s | %s",
					e.level,
					chrono::to_string("%Y-%b-%d %H:%M:%S", e.timestamp).c_str(),
					e.message.c_str());
				count++;
				if (count == 100)
					logging_complete.set();
			});

			auto action = []()
			{
				for (int i = 0; i < 25; i++)
					log((i % 5) + 1, "Message #%i", i + 1);
			};

			std::thread t1(action);
			std::thread t2(action);
			std::thread t3(action);
			std::thread t4(action);

			logging_complete.wait_one();

			t1.join();
			t2.join();
			t3.join();
			t4.join();

			detail::manager::get_instance().stop();
		}

	};
}