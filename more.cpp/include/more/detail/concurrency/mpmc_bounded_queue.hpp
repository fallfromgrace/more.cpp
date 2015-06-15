#ifndef MORE_CPP_MPMC_BOUNDED_QUEUE_HPP
#define MORE_CPP_MPMC_BOUNDED_QUEUE_HPP

#include <array>
#include <atomic>
#include <type_traits>

#include "more\includes.hpp"

namespace more
{
	// Implementation of a concurrent queue using only one CAS operation.
	// courtesy of dmitry vyukyov
	template<typename value, size_t size = 1024>
	class mpmc_bounded_concurrent_queue
	{
	public:
		typedef value value_type;
		typedef size_t size_type;
	private:
		typedef class node
		{
		public:
			std::atomic<size_t> sequence;
			value_type value;
		} node_type;
	public:
		mpmc_bounded_concurrent_queue()
		{
			head.store(0, std::memory_order_relaxed);
			tail.store(0, std::memory_order_relaxed);

			for (size_t sequence = 0; sequence < size; sequence++)
				buffer[sequence].sequence.store(sequence, std::memory_order_relaxed);
		}

		//
		bool is_empty() const
		{
			size_type position = tail.load(std::memory_order_relaxed);
			node_type* node = &buffer[position & (size - 1)];
			size_type sequence = node->sequence.load(std::memory_order_relaxed);
			return sequence < position + 1;
		}

		// 
		bool is_full() const
		{
			size_type position = head.load(std::memory_order_relaxed);
			node_type* node = &buffer[position & (size - 1)];
			size_type sequence = node->sequence.load(std::memory_order_relaxed);
			return sequence < position;
		}

		// Attempts to retrieve the next value.
		// May fail
		bool try_pop(value_type& value)
		{
			node_type* node;
			size_type position;
			size_type sequence;

			position = tail.load(std::memory_order_relaxed);
			node = &buffer[position & (size - 1)];
			sequence = node->sequence.load(std::memory_order_acquire);

			if (sequence == position + 1)
			{
				if (tail.compare_exchange_weak(position, position + 1, std::memory_order_relaxed))
				{
					value = node->value;
					node->sequence.store(position + size, std::memory_order_release);
					return true;
				}
				// Another thread advanced the tail first or spurious fail.  
				// Retry by spinning/yielding
				return false;
			}
			else if (sequence < position + 1)
			{
				// Buffer empty.
				// Retry using backoff wait strategy.
				return false;
			}
			else
			{
				// Another thread has already read this node.
				// Retry by spinning
				return false;
			}
		}

		// 
		bool try_push(value_type&& value)
		{
			node_type* node;
			size_type position;
			size_type sequence;

			position = head.load(std::memory_order_relaxed);
			node = &buffer[position & (size - 1)];
			sequence = node->sequence.load(std::memory_order_acquire);

			// 
			if (sequence <= position)
			{
				if (head.compare_exchange_weak(position, position + 1, std::memory_order_relaxed))
				{
					node->value = std::move(value);
					node->sequence.store(position + 1, std::memory_order_release);
					return true;
				}
				// Another thread advanced the head first or spurious fail.  
				// Retry by spinning.
				return false;
			}
			//else if (sequence < position)
			//{
			//	// Buffer full.  
			//	// Retry using backoff wait strategy.
			//	return false;
			//}
			else
			{
				// Another thread has already written to this node.  
				// Retry by spinning
				return false;
			}
		}

		// 
		bool try_push(const value_type& value)
		{
			node_type* node;
			size_type position;
			size_type sequence;

			position = head.load(std::memory_order_relaxed);
			node = &buffer[position & (size - 1)];
			sequence = node->sequence.load(std::memory_order_acquire);

			// 
			if (sequence <= position)
			{
				if (head.compare_exchange_weak(position, position + 1, std::memory_order_relaxed))
				{
					node->value = value;
					node->sequence.store(position + 1, std::memory_order_release);
					return true;
				}
				// Another thread advanced the head first or spurious fail.  
				// Retry by spinning.
				return false;
			}
			//else if (sequence < position)
			//{
			//	// Buffer full.  
			//	// Retry using backoff wait strategy.
			//	return false;
			//}
			else
			{
				// Another thread has already written to this node.  
				// Retry by spinning
				return false;
			}
		}

	private:
		L1CACHE_ALIGN std::atomic<size_type> head;
		L1CACHE_ALIGN std::atomic<size_type> tail;
		L1CACHE_ALIGN std::array<node_type, size> buffer;
	};

}

#endif//MORE_CPP_MPMC_BOUNDED_QUEUE_HPP