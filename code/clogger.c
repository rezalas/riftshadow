#include <memory>
#include <utility>
#include "rift.h"
#include "stdlibs/clogger.h"
#include "../include/spdlog/spdlog.h"
#include "../include/spdlog/sinks/stdout_color_sinks.h"
#include "../include/spdlog/sinks/basic_file_sink.h"

CLogger::CLogger()
{
}

CLogger::~CLogger()
{
}

/// Sets up the various loggers used to log messages.
/// @note This is currently configured to log to both the console and log file.
void CLogger::Initialize()
{
	if (spdlog::get("rs") == nullptr)
	{
		auto level = spdlog::level::trace;
		spdlog::set_level(level);

		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_level(level);
		console_sink->set_pattern("[%^%l%$] %v");

		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(RS_LOG_FILE, true);
		file_sink->set_level(level);

		std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink };
		auto logger = std::make_shared<spdlog::logger>("rs", sinks.begin(), sinks.end());
		logger->set_level(level);
		logger->flush_on(level);

		spdlog::register_logger(logger);
	}
}

/// Stops the logger from logging any more messages.
void CLogger::Shutdown()
{
	spdlog::shutdown();
}

void CLogger::SetLevel(spdlog::level::level_enum level)
{
	if (spdlog::get("rs") != nullptr)
	{
		spdlog::get("rs")->set_level(level);
		spdlog::get("rs")->flush_on(level);
	}
}