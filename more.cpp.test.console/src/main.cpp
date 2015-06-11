#include <tchar.h>
#include <cstdio>
#include <string>
#

// 
template<typename... arg>
std::string format(const std::string& format, arg&&... args)
{
#pragma warning(suppress:4996)
	int size = _snprintf(
		nullptr, 0,
		format.c_str(),
		std::forward<arg>(args)...);
	std::string result(size, 0);
#pragma warning(suppress:4996)
	size = _snprintf(
		const_cast<char*>(result.data()),
		result.length(),
		format.c_str(),
		std::forward<arg>(args)...);

	return result;
}

int _tmain(int argc, _TCHAR* argv[])
{
	auto format = "result: %f, %s";
	auto s = "123";
	auto f = 1.101;
	auto result = ::format(format, f, s);

	return 0;
}