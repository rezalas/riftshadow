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

#ifdef SPDLOG_ACTIVE_LEVEL
	#undef SPDLOG_ACTIVE_LEVEL
	#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

class CLogger
{
public:
	CLogger();
	~CLogger();

	void Initialize();
	void Shutdown();
	void SetLevel(spdlog::level::level_enum level);

	template <typename... Args>
	void Bug(std::string_view fmt, Args &&...args)
	{		
		spdlog::get("rs")->warn(fmt, std::forward<Args>(args)...);

		//TODO: The old RS.Bug had this attached to it. Need to investigate if it is needed or not.
		//	wiznet(str.data(), 0, 0, WIZ_DEBUG, 0, 0);
	}

	template <typename... Args>
	void Log(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->log(spdlog::level::info, fmt, std::forward<Args>(args)...);
	}

	/// @brief Log trace messages
	/// @tparam ...Args: Template parameter pack used to specify the variadic arguments. 
	/// @param fmt: The message to log
	/// @param ...args: Variadic arguments used to fill the format specifiers identified in the fmt string. 
	/// @note Format specifiers are in the fmt::* format.
	template <typename... Args>
	void Trace(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->trace(fmt, std::forward<Args>(args)...);
	}

	/// @brief Log debug messages
	/// @tparam ...Args: Template parameter pack used to specify the variadic arguments. 
	/// @param fmt: The message to log
	/// @param ...args: Variadic arguments used to fill the format specifiers identified in the fmt string. 
	/// @note Format specifiers are in the fmt::* format.
	template <typename... Args>
	void Debug(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->debug(fmt, std::forward<Args>(args)...);
	}

	/// @brief Log informational messages
	/// @tparam ...Args: Template parameter pack used to specify the variadic arguments. 
	/// @param fmt: The message to log
	/// @param ...args: Variadic arguments used to fill the format specifiers identified in the fmt string. 
	/// @note Format specifiers are in the fmt::* format.
	template <typename... Args>
	void Info(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->info(fmt, std::forward<Args>(args)...);
	}

	/// @brief Log warning messages
	/// @tparam ...Args: Template parameter pack used to specify the variadic arguments. 
	/// @param fmt: The message to log
	/// @param ...args: Variadic arguments used to fill the format specifiers identified in the fmt string. 
	/// @note Format specifiers are in the fmt::* format.
	template <typename... Args>
	void Warn(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->warn(fmt, std::forward<Args>(args)...);
	}

	/// @brief Log error messages
	/// @tparam ...Args: Template parameter pack used to specify the variadic arguments. 
	/// @param fmt: The message to log
	/// @param ...args: Variadic arguments used to fill the format specifiers identified in the fmt string. 
	/// @note Format specifiers are in the fmt::* format.
	template <typename... Args>
	void Error(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->error(fmt, std::forward<Args>(args)...);
	}

	/// @brief Log critical messages
	/// @tparam ...Args: Template parameter pack used to specify the variadic arguments. 
	/// @param fmt: The message to log
	/// @param ...args: Variadic arguments used to fill the format specifiers identified in the fmt string. 
	/// @note Format specifiers are in the fmt::* format.
	template <typename... Args>
	void Critical(std::string_view fmt, Args &&...args)
	{
		spdlog::get("rs")->critical(fmt, std::forward<Args>(args)...);
	}
};

#endif