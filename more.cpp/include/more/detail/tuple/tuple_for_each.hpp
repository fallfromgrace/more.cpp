#pragma once

#include <tuple>
#include <type_traits>

namespace more
{
	template<size_t>
	struct tuple_index
	{

	};

	template<typename tuple_type, typename action_type>
	inline void tuple_for_each(tuple_type&& tuple, action_type&& action)
	{
		tuple_for_each(
			std::forward<tuple_type>(tuple),
			std::forward<action_type>(action),
			tuple_index<std::tuple_size<tuple_type>::value>());
	}

	template<typename tuple_type, typename action_type>
	inline void tuple_for_each(tuple_type&& tuple, action_type&& action, tuple_index<0>)
	{

	}

	template<typename tuple_type, typename action_type, size_t tuple_index_value>
	inline void tuple_for_each(tuple_type&& tuple, action_type&& action, tuple_index<tuple_index_value>)
	{
		tuple_for_each(
			std::forward<tuple_type>(tuple),
			std::forward<action_type>(action),
			tuple_index<tuple_index_value - 1>());
		action(std::get<tuple_index_value - 1>(std::forward<tuple_type>(tuple)));
	}
}