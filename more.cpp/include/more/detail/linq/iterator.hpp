#pragma once

#ifndef MORE_CPP_LINQ_ITERATOR_HPP
#define MORE_CPP_LINQ_ITERATOR_HPP

#include <cassert>
#include <iterator>

namespace more
{
	// Wraps an enumerator to support stl containers and algorithms.
	template<typename expression>
	class iterator : public std::iterator<
		std::forward_iterator_tag,
		typename expression::value_type,
		ptrdiff_t,
		typename expression::pointer_type,
		typename expression::reference_type>
	{
	public:
		typedef typename expression::enumerator enumerator;

		// Creates an iterator the specifies the end of a sequence.
		iterator() :
			end(true)
		{

		}

		// Creates an iterator that wraps the specified enumerator.
		iterator(const enumerator& source) :
			source(source), end(source.empty())
		{

		}

		// Creates an iterator that wraps the specified enumerator.
		iterator(enumerator&& source) :
			source(std::move(source)), end(this->source.empty())
		{

		}

		reference operator *()
		{
			assert(end == false);
			return source.get();
		}

		// Increments the 
		iterator& operator ++()
		{
			source.next();
			end = source.empty();
			return *this;
		}

		iterator operator ++(int)
		{
			iterator temp(*this);
			operator++();
			return temp;
		}

		bool operator !=(const iterator& other) const
		{
			return operator==(other) == false;
		}

		bool operator ==(const iterator& other) const
		{
			bool same_end = end == other.end;
			if (end == true && same_end)
				return true;
			else if (end == false && same_end)
				return (&*this == &*this);
			else
				return false;
		}

	private:
		enumerator source;
		bool end;
	};
}

#endif//MORE_CPP_LINQ_ITERATOR_HPP