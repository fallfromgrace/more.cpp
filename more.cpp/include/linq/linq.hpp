#pragma once

#ifndef LIBEZM_ENUMERABLE_HPP
#define LIBEZM_ENUMERABLE_HPP

#include <algorithm>
#include <assert.h>
#include <exception>
#include <numeric>
#include <vector>
#include <map>
#include <unordered_map>

#include "linq\from.hpp"
#include "linq\select.hpp"
#include "linq\skip.hpp"
#include "linq\take.hpp"
#include "linq\where.hpp"
#include "linq\zip_simple.hpp"

#include "linq\iterator.hpp"

namespace more
{
	/*I feel like enumerators and expressions do too much redundant copying of data, hurting performance.*/

	// Represents an enumerable sequence of values.
	// Defines various functional-style, lazily-evaluated, composable operators that enable easy 
	// modification of container contents.
	// Supports stl-style begin/end in addition to exposing the enumerator.
	// This implementation is actually very similar to Microsoft's Ix++.
	template<typename expression>
	class enumerable
	{
	public:
		typedef typename expression::pointer_type pointer_type;
		typedef typename expression::reference_type reference_type;
		typedef typename expression::value_type value_type;
		typedef typename expression::enumerator enumerator;

		// initializes a new enumerable using the specified expression.
		enumerable(const expression& source) :
			source(source)
		{

		}

		// Aggregates the sequence using the specified aggregator.  
		// Throws if the sequence is empty.
		template<typename function>
		value_type aggregate(function&& aggregator) const
		{
			enumerator enumerator = source.get_enumerator();
			assert(enumerator.empty() == false);

			value_type seed = enumerator.get();
			enumerator.next();
			while (enumerator.empty() == false)
			{
				seed = aggregator(seed, enumerator.get());
				enumerator.next();
			}

			return seed;
		}

		// Aggregates the sequence using the specified seed value and aggregator.  
		template<typename seed, typename function>
		seed aggregate(seed seed, function&& aggregator) const
		{
			enumerator enumerator = source.get_enumerator();
			while (enumerator.empty() == true)
			{
				seed = aggregator(seed, enumerator.get());
				enumerator.next();
			}

			return seed;
		}

		// Tests if the sequence has any elements.
		bool any() const
		{
			return source.get_enumerator().empty() == false;
		}

		// Tests if the sequence has any elements satisfying the specified predicate.
		template<typename function>
		bool any(function&& predicate) const
		{
			enumerator enumerator = source.get_enumerator();
			while (enumerator.empty() == false)
			{
				if (predicate(enumerator.get()) == true)
					return true;
				enumerator.next();
			}

			return false;
		}

		// Test if the all the elements of the sequence satisfy the specifed predicate.
		template<typename function>
		bool all(const function& predicate) const
		{
			return any([=](auto& _l) { return predicate(_l) == false; }) == false;
		}

		// Counts the total number of elements in the sequence.
		size_t count() const
		{
			enumerator enumerator = source.get_enumerator();
			size_t count = 0;
			while (enumerator.empty() == false)
			{
				++count;
				enumerator.next();
			}

			return count;
		}

		// Counts the total number of elements in the sequence satisfying the specfied predicate.
		template<typename function>
		size_t count(function&& predicate) const
		{
			return where(std::forward<function>(predicate)).count();
		}

		// Gets the element at specified position.  
		// Throws if moved past the end of the enumerable.
		value_type element_at(size_t position) const
		{
			enumerator enumerator = source.get_eumerator();
			assert(enumerator.empty() == false);
			while (position > 0)
			{
				enumerator.next();
				assert(enumerator.empty() == false);
				--position;
			}

			return enumerator.get();
		}

		// Gets the first element in the sequence.
		// Throws if the sequnce is empty.
		value_type first() const
		{
			enumerator enumerator = source.get_enumerator();
			assert(enumerator.empty() == false);
			return enumerator.get();
		}

		// Gets the first element in the sequence satisfying the specified predicate.
		template<typename function>
		reference_type first(function&& predicate) const
		{
			return where(std::forward<function>(predicate)).first();
		}

		// Invokes an action on each element of the sequence.
		template<typename function>
		void for_each(function&& action) const
		{
			enumerator enumerator = source.get_enumerator();
			while (enumerator.empty() == false)
			{
				action(enumerator.get());
				enumerator.next();
			}
		}

		// Gets the first element in the sequence.
		// Throws if the sequnce is empty.
		value_type last() const
		{
			enumerator enumerator = source.get_enumerator();
			assert(enumerator.empty() == false);
			
			value_type last = enumerator.get();
			enumerator.next();
			while (enumerator.empty() == false)
			{
				last = enumerator.get();
				enumerator.next();
			}

			return last;
		}

		template<typename function>
		value_type max_by(function&& selector)
		{
			enumerator enumerator = source.get_enumerator();
			assert(enumerator.empty() == false);

			value_type max = enumerator.get();
			auto max_compare = selector(max);
			enumerator.next();
			while (enumerator.empty() == false)
			{
				value_type next = enumerator.get();
				auto next_compare = selector(next);
				if (next_compare > max)
				{
					max = next;
					max_compare = next_compare;
				}
				enumerator.next();
			}

			return max;
		}

		// Projects the sequence to to a new type using the specified selector.
		template<typename function>
		enumerable<select_expression<expression, function>> select(function&& selector) const
		{
			return enumerable<select_expression<expression, function>>(
				select_expression<expression, function>(source, std::forward<function>(selector)));
		}

		// Gets the only element of the sequence.  
		// Throws if there is anything other than a single element in the sequence.
		value_type single() const
		{
			enumerator enumerator = source.get_enumerator();
			assert(enumerator.empty() == false);

			value_type value = enumerator.get();
			enumerator.next();
			while (enumerator.empty() == false)
			{
				value = enumerator.get();
				enumerator.next();
			}

			return value;
		}

		// Gets the only element of the sequence.  
		//template<typename... args>
		//value_type single_or_default(args&&... arg) const
		//{
		//	enumerator enumerator = source.get_enumerator();

		//	value_type value(std::forward<args>(arg)...);
		//	while (enumerator.empty() == false)
		//	{
		//		value = enumerator.get();
		//		enumerator.next();
		//	}

		//	return value;
		//}

		value_type single_or_default(value_type default_value) const
		{
			enumerator enumerator = source.get_enumerator();

			value_type value = default_value;
			while (enumerator.empty() == false)
			{
				value = enumerator.get();
				enumerator.next();
			}

			return value;
		}

		// Gets the only element of the sequence.  
		//value_type single_or_default(const value_type& default_value) const
		//{
		//	enumerator enumerator = source.get_enumerator();

		//	value_type value = default_value;
		//	while (enumerator.empty() == false)
		//	{
		//		value = enumerator.get();
		//		enumerator.next();
		//	}

		//	return value;
		//}

		// Gets the only element of the sequence.  
		// Throws if there is anything other than a single element in the sequence.
		//value_type single_or_default(value_type&& default_value) const
		//{
		//	enumerator enumerator = source.get_enumerator();

		//	value_type value = std::move(default_value);
		//	while (enumerator.empty() == false)
		//	{
		//		value = enumerator.get();
		//		enumerator.next();
		//	}

		//	return value;
		//}

		// Increments the enumerator by up to the specified number of elements and returns the remaining.
		enumerable<skip_expression<expression>> skip(size_t nelements) const
		{
			return enumerable<skip_expression<expression>>(
				skip_expression<expression>(source, nelements));
		}

		// Returns up to the specified number of beginning sequential elements.
		enumerable<take_expression<expression>> take(size_t nelements) const
		{
			return enumerable<take_expression<expression>>(
				take_expression<expression>(source, nelements));
		}

		// Converts the sequence to the specified container.
		template<typename container> 
		container to_container() const
		{
			//return container(begin(), end());
			enumerator enumerator = source.get_enumerator();
			container container;
			while (enumerator.empty() == false)
			{
				container.push_back(enumerator.get());
				enumerator.next();
			}
			return container;
		}

		// Converts the sequence to the specified container using the specified key selector.
		template<typename container, typename function>
		container to_map(function&& key_selector) const
		{
			auto pairs = this->select([&](const value_type& value) { return std::make_pair(key_selector(value), value); });
			return container(pairs.begin(), pairs.end());
		}

		// Converts the sequence into a std::list.
		std::list<value_type> to_list() const
		{
			return to_container<std::list<value_type>>();
		}

		// Converts the sequence into a std::vector.
		std::vector<value_type> to_vector() const
		{
			return to_container<std::vector<value_type>>();
		}

		// Converts the sequence into a std::unordered_map using the specified key selector.
		template<typename key_type, typename function>
		std::unordered_map<key_type, value_type> to_unordered_map(function&& key_selector) const
		{
			return to_map<std::unordered_map<key_type, value_type>>(std::forward<function>(key_selector))
		}

		// Converts the sequence into a std::map using the specified key selector.
		template<typename key_type, typename function>
		std::map<key_type, value_type> to_ordered_map(function&& key_selector) const
		{
			return to_map<std::map<key_type, value_type>>(std::forward<function>(key_selector))
		}

		// Projects the sequence with only the elements satisfying the specified predicate.
		template<typename function>
		enumerable<where_expression<expression, function>> where(function&& predicate) const
		{
			return enumerable<where_expression<expression, function>>(
				where_expression<expression, function>(source, std::forward<function>(predicate)));
		}

		// Projects the source sequence wuth the specified sequences into a new type using the 
		// specified selector.
		template<typename function, typename... input_expression>
		enumerable<zip_expression<expression, function, input_expression...>> zip(
			function&& selector, input_expression&&... expressions)
		{
			return enumerable<zip_expression<expression, function, input_expression...>>(
				zip_expression<expression, function, input_expression...>(
					source, 
					std::forward<function>(selector), 
					std::forward<input_expression>(expressions)...));
		}

		/* Iterators don't quite work cleanly yet.  
		 * enumerators and begin/end iterators are rather incompatible, tbh, as with an enumerator you do not need to 
		 * know the 'end()' immediately.  Essentially it implies you know exactly when the collection begins and ends, 
		 * which is not the case with a lazily evaluated list.
		 * Currently using 'end()' as just a flag.
		 */
		// Gets an iterator starting at the first value in the sequence.
		iterator<enumerator> begin() const
		{
			return iterator<enumerator>(source.get_enumerator());
		}

		// Gets an iterator for an empty sequence.
		iterator<enumerator> end() const
		{
			return iterator<enumerator>();
		}

		// Gets an enumerator used to iterate along the sequence.
		enumerator get_enumerator() const
		{
			return source.get_enumerator();
		}
	private:
		expression source;
	};

	// Creates an enumerable expression from the specified stl-style begin and end iterators in.  Does not copy container.
	template<typename iterator>
	enumerable<from_expression<iterator>> from(const iterator& begin, const iterator& end)
	{
		return enumerable<from_expression<iterator>>(
			from_expression<iterator>(begin, end));
	}

	// Creates an enumerable expression from the specified stl-style container.  Does not copy container.
	template<typename container>
	enumerable<from_expression<typename container::const_iterator>> from(const container& cont)
	{
		return from(cont.begin(), cont.end());
	}
}

#endif//LIBEZM_ENUMERABLE_HPP