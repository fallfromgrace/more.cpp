#ifndef SPSC_BOUNDED_QUEUE_HPP
#define SPSC_BOUNDED_QUEUE_HPP

#include <array>
#include <atomic>

#include "includes.hpp"

namespace ez
{
	// from Correct and Efficient Bounded FIFO Queues
	template<typename value_t, size_t size = 1024>
	class spsc_bounded_queue
	{
	public:
		typedef value_t value_type;

		spsc_bounded_queue()
		{
			head.store(0, std::memory_order_relaxed);
			tail.store(0, std::memory_order_relaxed);
			phead = 0;
			ctail = 0;
		}

		// 
		template<typename buffer>
		bool try_push(buffer&& buffer)
		{
			// acquire head.
			size_t t, n = buffer.size();
			t = tail.load(std::memory_order_relaxed);
			if (phead + size - t < n)
			{
				phead = head.load(std::memory_order_acquire);
				if (phead + size - t < n)
					return false;
			}

			// write to buffer
			for (size_t i = 0; i < n; i++)
				this->buffer[(t + i) % size] = buffer[i];

			// advance tail.
			tail.store(t + n, std::memory_order_release);
			return true;
		}

		template<>
		bool try_push(value_type&& value)
		{
			// acquire head.
			size_t t = tail.load(std::memory_order_relaxed);
			if (phead + size - t < 1)
			{
				phead = head.load(std::memory_order_acquire);
				if (phead + size - t < 1)
					return false;
			}

			// write to buffer
			this->buffer[t % size] = std::move(value);

			// advance tail.
			tail.store(t + 1, std::memory_order_release);

			return true;
		}

		template<>
		bool try_push(const value_type& value)
		{
			// acquire head.
			size_t t = tail.load(std::memory_order_relaxed);
			if (phead + size - t < 1)
			{
				phead = head.load(std::memory_order_acquire);
				if (phead + size - t < 1)
					return false;
			}

			// write to buffer
			this->buffer[t % size] = value;

			// advance tail.
			tail.store(t + 1, std::memory_order_release);

			return true;
		}

		// 
		template<typename buffer>
		bool try_pop(buffer& buffer)
		{
			size_t h, n = buffer.size();
			h = head.load(std::memory_order_relaxed);
			if (ctail - h < n)
			{
				ctail = tail.load(std::memory_order_acquire);
				if (ctail - t < n)
					return false;
			}

			for (size_t i = 0; i < n; i++)
				buffer[i] = this->buffer[(h + i) % size];
			head.store(h + n, std::memory_order_release);
			return true;
		}

		// 
		template<>
		bool try_pop(value_type& value)
		{
			size_t h = head.load(std::memory_order_relaxed);
			if (ctail - h < 1)
			{
				ctail = tail.load(std::memory_order_acquire);
				if (ctail - t < 1)
					return false;
			}

			value = this->buffer[h % size];
			head.store(h + n, std::memory_order_release);
			return true;
		}

	private:
		L1CACHE_ALIGN std::atomic_size_t head;
		L1CACHE_ALIGN size_t phead;
		L1CACHE_ALIGN std::atomic_size_t tail;
		L1CACHE_ALIGN size_t ctail;
		L1CACHE_ALIGN std::array<value_type, size> buffer;
	};
}

#endif//SPSC_BOUNDED_QUEUE_HPP