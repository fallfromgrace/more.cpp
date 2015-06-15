#pragma once

#ifndef MORE_CPP_LINQ_SELECT_MANY_HPP
#define MORE_CPP_LINQ_SELECT_MANY_HPP

#include <utility>

namespace more
{
	// 
	template<typename expression, typename function>
	class select_many_expression
	{
	public:
		typedef expression source_expression;
		typedef typename source_expression::enumerator source_enumerator;
		typedef typename source_expression::pointer_type source_pointer_type;
		typedef typename source_expression::reference_type source_reference_type;
		typedef typename source_expression::value_type source_value_type;

		typedef decltype(std::declval<function>()(std::declval<source_reference_type>())) reference_type;
		typedef typename std::remove_reference<reference_type>::type value_type;
		typedef typename std::add_pointer<value_type>::type pointer_type;

		class enumerator
		{
		public:

			enumerator(const source_enumerator& source, function selector) :
				source(source), selector(selector)
			{

			}

			bool empty() const
			{
				return source.empty();
			}

			reference_type get() const
			{
				return selector(source.get());
			}

			void next()
			{
				source.next();
			}

		private:
			source_enumerator source;
			const function selector;
		};


		select_many_expression(const source_expression& source, function selector) :
			source(source), selector(selector)
		{

		}

		enumerator get_enumerator() const
		{
			return enumerator(source.get_enumerator(), selector);
		}
	private:
		const source_expression source;
		const function selector;
	};
}

#endif//MORE_CPP_LINQ_SELECT_MANY_HPP