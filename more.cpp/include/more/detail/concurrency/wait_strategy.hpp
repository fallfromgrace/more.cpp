#ifndef MORE_CPP_WAIT_STRATEGY_HPP
#define MORE_CPP_WAIT_STRATEGY_HPP

#include <condition_variable>
#include <mutex>
#include <thread>

namespace more
{
	// Uses mutex and condition_variable to halt the thread.
	class blocking_wait_strategy
	{
	public:
		template<typename predicate_type>
		void wait_for(predicate_type&& predicate)
		{
			if (predicate() == true)
				return;

			std::unique_lock<std::mutex> lock(mutex);
			cv.wait(lock, predicate);
		}

		void end_wait()
		{
			cv.notify_one();
		}
	private:
		std::mutex mutex;
		std::condition_variable cv;
	};

	// Uses the sleep function.
	template<size_t duration>
	class sleeping_wait_strategy
	{
	public:
		template<typename predicate_type>
		void wait_for(predicate_type&& predicate)
		{
			while (predicate() == false)
				std::this_thread::sleep_for(std::chrono::microseconds(duration));
		}

		void end_wait()
		{

		}
	};

	// Uses yield.
	class yielding_wait_strategy
	{
	public:
		template<typename predicate_type>
		void wait_for(predicate_type&& predicate)
		{
			while (predicate() == false)
				std::this_thread::yield();
		}

		void end_wait()
		{

		}
	};

	// Spins.
	class spinning_wait_strategy
	{
	public:
		template<typename predicate_type>
		void wait_for(predicate_type&& predicate)
		{
			while (predicate() == false);
		}

		void end_wait()
		{

		}
	};

	// Uses spinning, yielding, sleeping, then finally blocking.  Acts as a compromise between 
	// other wait strategies.  Values can be tweaked to provide optimal performance for specific 
	// scenarios.
	template<
		size_t max_spin_iterations = 1,
		size_t max_yield_iterations = 10,
		size_t max_sleep_iterations = 100,

		size_t spin_threshold = max_spin_iterations,
		size_t yield_threshold = spin_threshold + max_yield_iterations,
		size_t sleep_threshold1 = yield_threshold + max_sleep_iterations,
		size_t sleep_threshold2 = sleep_threshold1 + max_sleep_iterations,
		size_t sleep_threshold3 = sleep_threshold2 + max_sleep_iterations>
	class progressive_backoff_wait_strategy
	{
	public:
		template<typename predicate_type>
		void wait_for(predicate_type&& predicate)
		{
			size_t i = 0;
			spin_wait.wait_for([&]{ return predicate() || spin_threshold < ++i; });
			if (spin_threshold >= i)
				return;

			yield_wait.wait_for([&]{ return predicate() || yield_threshold < ++i; });
			if (yield_threshold >= i)
				return;

			sleep_wait1.wait_for([&]{ return predicate() || sleep_threshold1 < ++i; });
			if (sleep_threshold1 >= i)
				return;

			sleep_wait10.wait_for([&]{ return predicate() || sleep_threshold2 < ++i; });
			if (sleep_threshold2 >= i)
				return;

			sleep_wait100.wait_for([&]{ return predicate() || sleep_threshold3 < ++i; });
			if (sleep_threshold3 >= i)
				return;

			block_wait.wait_for(std::forward<predicate_type>(predicate));
		}

		void end_wait()
		{
			block_wait.end_wait();
		}

	private:
		spinning_wait_strategy spin_wait;
		yielding_wait_strategy yield_wait;
		sleeping_wait_strategy<1> sleep_wait1;
		sleeping_wait_strategy<100> sleep_wait10;
		sleeping_wait_strategy<10000> sleep_wait100;
		blocking_wait_strategy block_wait;
	};

	// Generalized wait strategy.
	template<typename fallback_wait_strategy, typename... wait_strategy>
	class phasing_wait_strategy
	{
	public:
		template<typename predicate_type>
		void wait_for(predicate_type&& predicate)
		{
			tuple_for_each(
				wait_strategies,
				functor<predicate_type>(std::forward<predicate_type>(predicate)));
			fallback_wait.wait_for(std::forward<predicate_type>(predicate));
		}
	private:
		template<typename predicate_type>
		class functor
		{
		public:
			functor(const predicate_type& predicate) :
				predicate(predicate)
			{

			}

			template<typename wait_strategy_type>
			void operator()(wait_strategy_type&& wait_strategy) const
			{
				wait_strategy.wait_for(predicate);
			}

			predicate_type predicate;
		};

		fallback_wait_strategy fallback_wait;
		std::tuple<wait_strategy...> wait_strategies;
	};
}

#endif//LIBEZM_WAIT_STRATEGY_HPP