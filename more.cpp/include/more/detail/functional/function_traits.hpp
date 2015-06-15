#ifndef MORE_CPP_FUNCTION_TRAITS_HPP
#define MORE_CPP_FUNCTION_TRAITS_HPP

#include <tuple>

namespace more
{
	template<typename T>
	struct function_traits :
		public function_traits<decltype(&T::operator())>
	{

	};

	// For generic types, directly use the result of the signature of its 'operator()'
	template<typename ClassType, typename ReturnType, typename... Args>
	struct function_traits<ReturnType(ClassType::*)(Args...) const>
		// we specialize for pointers to member function
	{
		enum { arity = sizeof...(Args) };
		// arity is the number of arguments.

		typedef ReturnType result_type;

		template <size_t i>
		struct arg
		{
			typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
			// the i-th argument is equivalent to the i-th tuple element of a tuple
			// composed of those arguments.
		};
	};

	// For generic types, directly use the result of the signature of its 'operator()'
	template<typename ClassType, typename ReturnType, typename... Args>
	struct function_traits<ReturnType(ClassType::*)(Args...)>
		// we specialize for pointers to member function
	{
		enum { arity = sizeof...(Args) };
		// arity is the number of arguments.

		typedef ReturnType result_type;

		template <size_t i>
		struct arg
		{
			typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
			// the i-th argument is equivalent to the i-th tuple element of a tuple
			// composed of those arguments.
		};
	};

	//template<class FPtr>
	//struct function_traits;

	//template<class R, class C, class A1>
	//struct function_traits<R(C::*)(A1)>
	//{   // non-const specialization
	//	typedef A1 arg_type;
	//	typedef R result_type;
	//	typedef R type(A1);
	//};

	//template<class R, class C, class A1>
	//struct function_traits<R(C::*)(A1) const>
	//{   // const specialization
	//	typedef A1 arg_type;
	//	typedef R result_type;
	//	typedef R type(A1);
	//};
}

#endif//MORE_CPP_FUNCTION_TRAITS_HPP