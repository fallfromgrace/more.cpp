#pragma once

#ifndef MORE_CPP_STRING_FORMAT_HPP
#define MORE_CPP_STRING_FORMAT_HPP

#include <cassert>
#include <cstdio>
#include <string>

#if _MSC_VER != 1900
#define snprintf _snprintf
#pragma warning(push)
#pragma warning(disable:4996)
#endif

namespace common
{
	// formats a string 
	template<typename... arg>
	std::string format(const std::string& format, arg&&... args)
	{
		auto format_c = format.c_str();
		auto size = snprintf(nullptr, 0, format_c, std::forward<arg>(args)...);
		std::string result(size, '\0');
		auto buffer = const_cast<char*>(result.data());
		auto buffer_size = result.size();
		size = snprintf(buffer, buffer_size, format_c, std::forward<arg>(args)...);
		assert(size == buffer_size);
		return result;
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#undef snprintf;
#endif

#endif//MORE_CPP_STRING_FORMAT_HPP