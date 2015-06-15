#ifndef LINQ_ZIP_HPP
#define LINQ_ZIP_HPP

#include <tuple>
//Currently appears to be broken in visual studio 2013.

namespace more
{
			
			template<size_t ...>
			struct seq { };

			template<size_t N, size_t ...S>
			struct gens : gens<N - 1, N - 1, S...> { };

			template<size_t ...S>
			struct gens<0, S...> {
				typedef seq<S...> type;
			};

	template<typename expression, typename function, typename... input_enumerable>
	class zip_expression
	{
	public:
		typedef expression source_expression;
		typedef typename source_expression::enumerator source_enumerator;
		typedef typename source_expression::pointer_type source_pointer_type;
		typedef typename source_expression::reference_type source_reference_type;
		typedef typename source_expression::value_type source_value_type;

		typedef decltype(std::declval<function>()(
			std::declval<source_reference_type>(), 
			std::declval<typename input_enumerable::reference_type>()...)) reference_type;
		typedef typename std::remove_reference<reference_type>::type value_type;
		typedef typename std::add_pointer<value_type>::type pointer_type;

		template<typename... input_enumerator>
		class enumerator_impl
		{
		public:

			enumerator_impl(const source_enumerator& source, const function& selector,
				const input_enumerator&... enumerators) :
				source(source), selector(selector), enumerators(enumerators)
			{

			}

			bool empty() const
			{
				return enumerators_empty(typename gens<sizeof...(input_enumerator)>::type());
			}

			reference_type get() const
			{
				return enumerators_get(typename gens<sizeof...(input_enumerator)>::type());
			}

			void next()
			{
				enumerators_next(typename gens<sizeof...(input_enumerator)>::type());
			}
		private:

			template<size_t... s>
			reference_type enumerators_get(seq<s...>) const
			{
				return selector(source.get(), std::get<s>(enumerators).get()...);
			}

			template<size_t... s>
			void enumerators_next(seq<s...>)
			{
				source.next();
				std::get<s>(enumerators).next()...;
			}

			template<size_t... s>
			bool enumerators_empty(seq<s...>) const
			{
				return any(std::get<s>(enumerators).empty()...);
			}

			bool any() const
			{
				return false;
			}

			template<typename head, typename... tail>
			bool any(head arg1, tail... args) const
			{
				return arg1 || any(args...);
			}

			source_enumerator source;
			function selector;
			std::tuple<input_enumerator...> enumerators;
		};

		zip_expression(
			const source_expression& source, 
			const function& selector, 
			const input_enumerable&... enumerables) :
			source(source), selector(selector), enumerables(std::make_tuple(enumerables...))
		{

		}

		template<size_t... s>
		enumerator_impl<decltype(std::get<s>(enumerables).get_enumerator())...> 
			enumerables_get_enumerator(seq<s...>) const
		{
			return enumerator_impl<decltype(std::get<s>(enumerables).get_enumerator())...>(
				source.get_enumerator(), 
				selector, 
				std::get<s>(enumerables).get_enumerator()...);
		}

		typedef decltype(enumerables_get_enumerator(typename gens<sizeof...(input_enumerable)>::type())) enumerator;

		enumerator get_enumerator() const
		{
			return enumerables_get_enumerator(typename gens<sizeof...(input_enumerable)>::type());
		}

	private:
		const source_expression source;
		const function selector;
		const std::tuple<input_enumerable...> enumerables;
	};
}

#endif//LINQ_ZIP_HPP