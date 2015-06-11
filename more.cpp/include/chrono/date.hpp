#pragma once

#include <chrono>
#include <ctime>
#include <string>

namespace chrono
{
	// 
	static std::string to_string(
		const std::string& format, 
		const std::chrono::system_clock::time_point& time_point)
	{
		auto time_t = std::chrono::system_clock::to_time_t(time_point);
		auto tm = std::localtime(&time_t);
		std::string result(64, '\0');
		auto buffer = const_cast<char*>(result.data());
		auto size = strftime(buffer, 64, format.c_str(), tm);
		return result;
	}
}