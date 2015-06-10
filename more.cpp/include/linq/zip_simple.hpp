#ifndef LINQ_ZIPSIMPLE_HPP
#define LINQ_ZIPSIMPLE_HPP

namespace more
{
	template<typename expression, typename function, typename input_expression>
	class zip_expression
	{
	public:
		typedef expression source_expression;
		typedef typename source_expression::enumerator source_enumerator;
		typedef typename source_expression::pointer_type source_pointer_type;
		typedef typename source_expression::reference_type source_reference_type;
		typedef typename source_expression::value_type source_value_type;

		typedef typename input_expression::reference_type input_reference_type;
		typedef typename input_expression::enumerator input_enumerator;

		typedef decltype(std::declval<function>()(
			std::declval<source_reference_type>(),
			std::declval<input_reference_type>())) reference_type;
		typedef typename std::remove_reference<reference_type>::type value_type;
		typedef typename std::add_pointer<value_type>::type pointer_type;


		class enumerator
		{
		public:

			enumerator(
				const source_enumerator& source, 
				const function& selector,
				const input_enumerator& input) :
				source(source), selector(selector), input(input)
			{

			}

			bool empty() const
			{
				return source.empty() || input.empty();
			}

			reference_type get() const
			{
				return selector(source.get(), input.get());
			}

			void next()
			{
				source.next();
				input.next();
			}
		private:
			source_enumerator source;
			const function selector;
			input_enumerator input;
		};

		zip_expression(
			const source_expression& source, 
			const function& selector,
			const input_expression& input) :
			source(source), selector(selector), input(input)
		{

		}

		enumerator get_enumerator() const
		{
			return enumerator(source.get_enumerator(), selector, input.get_enumerator());
		}

	private:
		const source_expression source;
		const function selector;
		const input_expression input;
	};
}

#endif