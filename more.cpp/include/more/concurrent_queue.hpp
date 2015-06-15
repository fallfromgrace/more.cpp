#pragma once

#ifndef MORE_CPP_CONCURRENT_QUEUE_HPP
#define MORE_CPP_CONCURRENT_QUEUE_HPP

#include "more\detail\concurrency\mpmc_bounded_queue.hpp"
#include "more\detail\concurrency\wait_strategy.hpp"

namespace more
{
	// Generic concurrent queue.
	template<
		typename value, 
		typename queue_t = mpmc_bounded_concurrent_queue<value>, 
		typename wait_strategy = progressive_backoff_wait_strategy<>>
	class concurrent_queue
	{
	public:
		typedef wait_strategy wait_strategy_type;
		typedef typename queue_t queue_type;
		typedef typename queue_type::value_type value_type;

		void pop(value_type& value)
		{
			pop_wait_strategy.wait_for([&]{ return queue.try_pop(value); });
			push_wait_strategy.end_wait();
		}

		void push(const value_type& value)
		{
			push_wait_strategy.wait_for([&]{ return queue.try_push(value); });
			pop_wait_strategy.end_wait();
		}

	private:
		queue_type queue;
		wait_strategy_type pop_wait_strategy;
		wait_strategy_type push_wait_strategy;
	};
}

#endif//MORE_CPP_CONCURRENT_QUEUE_HPP