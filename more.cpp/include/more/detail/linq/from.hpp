#pragma once

#ifndef MORE_CPP_LINQ_FROM_HPP
#define MORE_CPP_LINQ_FROM_HPP

namespace more
{
	template<typename iterator>
	class from_expression
	{
	public:
		typedef typename iterator::pointer pointer_type;
		typedef typename iterator::reference reference_type;
		typedef typename iterator::value_type value_type;

		class enumerator
		{
		public:

			enumerator(const iterator& begin, const iterator& end) :
				current(begin), end(end)
			{

			}

			bool empty() const
			{
				return current == end;
			}

			reference_type get() const
			{
				return *current;
			}

			void next()
			{
				++current;
			}

		private:
			iterator current;
			const iterator end;
		};

		// 
		from_expression(const iterator& begin, const iterator& end) :
			begin(begin), end(end)
		{

		}

		// 
		enumerator get_enumerator() const
		{
			return enumerator(begin, end);
		}

	private:
		const iterator begin;
		const iterator end;
	};
}

#endif//MORE_CPP_LINQ_FROM_HPP