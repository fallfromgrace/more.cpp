#pragma once

#ifndef MORE_CPP_STRING_TRAITS_HPP
#define MORE_CPP_STRING_TRAITS_HPP

#include <type_traits>
#include <string>

namespace more
{
	namespace detail
	{
		template<class _Ty>
		struct is_string : public std::false_type
		{

		};

		template<>
		struct is_string<std::string> : public std::true_type
		{

		};
	}

	template<typename string_type>
	struct is_string : public detail::is_string<std::decay_t<string_type>>
	{

	};
}

#endif//MORE_CPP_STRING_TRAITS_HPP