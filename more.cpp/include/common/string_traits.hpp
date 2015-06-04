#ifndef LIBEZM_STRING_TRAITS_HPP
#define LIBEZM_STRING_TRAITS_HPP

#include <type_traits>
#include <string>

namespace common
{
	template<typename string_type>
	struct is_string :
		public _is_string<std::decay_t<string_type>>
	{

	};

	template<class _Ty>
	struct _is_string
		: public std::false_type
	{

	};

	template<>
	struct _is_string<std::string>
		: public std::true_type
	{

	};
}

#endif//LIBEZM_STRING_TRAITS_HPP