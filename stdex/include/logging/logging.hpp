#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <vector>
#include "includes.hpp"
#include "concurrency\concurrent_queue.hpp"
#include <concurrent_vector.h>
#include "common\string_format.hpp"

#ifndef NDEBUG
#define log_debug(format, ...) (logging::detail::log_async<logging::log_level::debug>(format, ##__VA_ARGS__))
#define log_info(format, ...) (logging::detail::log_async<logging::log_level::info>(format, ##__VA_ARGS__))
#define log_warn(format, ...) (logging::detail::log_async<logging::log_level::warn>(format, ##__VA_ARGS__))
#define log_error(format, ...) (logging::detail::log_async<logging::log_level::error>(format, ##__VA_ARGS__))
#define log_fatal(format, ...) (logging::detail::log_async<logging::log_level::fatal>(format, ##__VA_ARGS__))
//#define log(level, format, ...) (logging::detail::log_manager_instance.log_async(logging::detail::make_log_event<level>(logging::detail::fmt(format, ##__VA_ARGS__))))
#else
#define log(level, message) 
#endif

namespace logging
{
	// 
	enum log_level
	{
		debug,
		info,
		warn,
		error,
		fatal,
	};

	// 
	class log_event
	{
	public:
		log_level level;
		std::string message;
		std::thread::id thread_id;
		std::chrono::system_clock::time_point timestamp;
	};

	namespace detail
	{
		// 
		template<log_level log_level>
		log_event make_log_event(const std::string& message)
		{
			log_event log_event;
			log_event.level = log_level;
			log_event.message = message;
			log_event.thread_id = std::this_thread::get_id();
			log_event.timestamp = std::chrono::system_clock::now();

			return log_event;
		}

		//class log_formatter
		//{
		//public:
		//	std::string format_log_event(log_event log_event)
		//	{

		//	}
		//};

		//
		class logger
		{
		public:
			// 
			logger(const std::function<void(const log_event&)>& write_log_event_function) :
				write_log_event_function(write_log_event_function)
			{

			}

			void write_log_event(const log_event& log_event)
			{
				this->write_log_event_function(log_event);
			}

		private:
			std::function<void(const log_event&)> write_log_event_function;
			//log_formatter formatter;
		};

		// 
		class log_manager
		{
		public:
			// 
			log_manager()
			{
				this->writer = std::thread([&]()
				{
					for (;;)
					{
						log_event log_event;
						this->log_event_queue.pop(log_event);
						for (auto& logger : this->loggers)
							logger.write_log_event(log_event);
					}
				});
			}

			void add_logger(const logger& logger)
			{
				this->loggers.push_back(logger);
			}

			log_manager(const log_manager&) = delete;

			log_manager& operator=(const log_manager&) = delete;

			void log_async(const log_event& log_event)
			{
				log_event_queue.push(log_event);
			}

			~log_manager()
			{

			}
		private:
			std::thread writer;
			concurrency::concurrent_vector<logger> loggers;
			con::concurrent_queue<log_event> log_event_queue;
		};

		static log_manager log_manager_instance;

		template<log_level log_level, typename... arg>
		void log_async(const std::string& format, arg&&... args)
		{
			log_manager_instance
				.log_async(make_log_event<log_level>(common::format(format, std::forward<arg>(args)...)));
		}
	}

	void add_logger(const std::function<void(const log_event&)>& f)
	{
		detail::log_manager_instance.add_logger(detail::logger(f));
	}
}