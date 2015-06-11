#pragma once

#include <concurrent_vector.h>
#include <thread>

#include "includes.hpp"
#include "concurrency\concurrent_queue.hpp"

#include "logging\event.hpp"
#include "logging\level.hpp"
#include "logging\logger.hpp"

namespace logging
{
	namespace detail
	{
		class manager
		{
		private:
			manager() :
				is_logging(false)
			{
				this->start();
			}
		public:

			manager(const manager&) = delete;

			manager& operator=(const manager&) = delete;

			manager(manager&&) = delete;

			manager& operator=(manager&&) = delete;

			~manager()
			{
				//this->stop();
			}

			static manager& get_instance()
			{
				static manager instance;
				return instance;
			}

			void start()
			{
				this->is_logging = true;
				this->writer = std::thread([this]()
				{
					while (this->is_logging == true)
					{
						event event;
						this->event_queue.pop(event);
						if (event.level != -1)
							for (auto& logger : this->loggers)
								logger.write(event);
					}
				});
			}

			void stop()
			{
				this->is_logging = false;
				this->log_async(event(-1));
				if (this->writer.joinable() == true)
					this->writer.join();
			}

			void log_async(const event& event)
			{
				this->event_queue.push(event);
			}

			void add_logger(const logger& logger)
			{
				this->loggers.push_back(logger);
			}

		private:
			bool_t is_logging;
			con::concurrent_queue<event> event_queue;
			concurrency::concurrent_vector<logger> loggers;
			std::thread writer;
		};
	}
}