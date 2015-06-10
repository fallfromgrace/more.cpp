#pragma once

#ifndef MORE_CPP_LINQ_SKIP_HPP
#define MORE_CPP_LINQ_SKIP_HPP

#include <utility>

namespace more
{
	template<typename expression>
	class skip_expression
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
			enumerator(const source_enumerator& source, size_t nelements) :
				source(source)
			{
				while (this->source.empty() == false && nelements > 0)
				{
					this->source.next();
					--nelements;
				}
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
				source.next();
			}

		private:
			source_enumerator source;
		};

		skip_expression(const source_expression& source, size_t nelements) :
			source(source), nelements(nelements)
		{

		}

		enumerator get_enumerator() const
		{
			return enumerator(source.get_enumerator(), nelements);
		}

	private:
		const source_expression source;
		const size_t nelements;
	};
}

#endif//MORE_CPP_LINQ_SKIP_HPP