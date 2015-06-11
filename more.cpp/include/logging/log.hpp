#pragma once

#include "common\string_format.hpp"

#include "logging\manager.hpp"

namespace logging
{

	template<typename... arg>
	void log(
		int32_t level,
		const std::string& format,
		arg&&... args)
	{
		event e;
		e.level = level;
		e.message = common::format(format, std::forward<arg>(args)...);
		e.timestamp = std::chrono::system_clock::now();
		e.thread_id = 0;

		detail::manager::get_instance().log_async(e);
	}

	template<typename... arg>
	void debug(
		const std::string& format,
		arg&&... args)
	{
		log(0, format, std::forward<arg>(args)...);
	}

	template<typename... arg>
	void info(
		const std::string& format,
		arg&&... args)
	{
		log(1, format, std::forward<arg>(args)...);
	}

	template<typename... arg>
	void warn(
		const std::string& format,
		arg&&... args)
	{
		log(2, format, std::forward<arg>(args)...);
	}

	template<typename... arg>
	void error(
		const std::string& format,
		arg&&... args)
	{
		log(3, format, std::forward<arg>(args)...);
	}

	template<typename... arg>
	void fatal(
		const std::string& format,
		arg&&... args)
	{
		log(4, format, std::forward<arg>(args)...);
	}

	template<typename action>
	static void add_logger(action&& log_action)
	{
		logger logger;
		logger.write = std::function<void(const event&)>(std::forward<action>(log_action));
		detail::manager::get_instance().add_logger(logger);
	}
}