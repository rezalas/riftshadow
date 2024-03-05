#ifndef CLOGGER
#define CLOGGER

#include <string>
#include "../include/spdlog/spdlog.h"

/*
	TODO: Add initialization and shutdown sequences to CMud.
	TODO: This is the basic gist of how to use the class.
	auto log = CLogger();
	log.Initialize();
	log.Bug("This is a test {}.", 0);
	log.Info("This is a test {} {}.", "Hello", "there");
	log.Log("This is a test {} {}.", 0, "oops");
	log.Shutdown();
*/

class CLogger
{
public:
	CLogger();
	~CLogger();

	void Initialize();
	void Shutdown();

	template <typename... Args>
	void Bug(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->warn(fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void Log(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->log(spdlog::level::info, fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void Trace(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->trace(fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void Debug(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->debug(fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void Info(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->info(fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void Warn(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->warn(fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void Error(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->error(fmt, std::forward<Args>(args)...);
	}

	template <typename... Args>
	void Critical(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->critical(fmt, std::forward<Args>(args)...);
	}
};

#endif