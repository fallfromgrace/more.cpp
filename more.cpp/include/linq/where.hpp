#pragma once

#ifndef MORE_CPP_LINQ_WHERE_HPP
#define MORE_CPP_LINQ_WHERE_HPP

namespace more
{
	template<typename expression, typename function>
	class where_expression
	{
	public:
		typedef expression source_expression;
		typedef typename source_expression::enumerator source_enumerator;
		typedef typename source_expression::pointer_type source_pointer_type;
		typedef typename source_expression::reference_type source_reference_type;
		typedef typename source_expression::value_type source_value_type;

		typedef source_pointer_type pointer_type;
		typedef source_reference_type reference_type;
		typedef source_value_type value_type;

		class enumerator
		{
		public:

			enumerator(const source_enumerator& source, const function& predicate) :
				source(source), predicate(predicate)
			{
				if (source.empty() == false &&
					predicate(source.get()) == false)
					next();
			}

			bool empty() const
			{
				return source.empty();
			}

			reference_type get() const
			{
				return source.get();
			}

			void next()
			{
				do
				{
					source.next();
				} while (source.empty() == false && predicate(source.get()) == false);
			}
		private:
			source_enumerator source;
			const function predicate;
		};

		where_expression(const source_expression& source, const function& predicate) :
			source(source), predicate(predicate)
		{

		}

		enumerator get_enumerator() const
		{
			return enumerator(source.get_enumerator(), predicate);
		}
	private:
		const source_expression source;
		const function predicate;
	};
}

#endif//MORE_CPP_LINQ_WHERE_HPP