#include <memory>
#include <utility>
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

void CLogger::Initialize()
{
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::info);
	console_sink->set_pattern("[%^%l%$] %v");

	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("../logs/rs.log", true);
	file_sink->set_level(spdlog::level::info);

	std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink };
	auto logger = std::make_shared<spdlog::logger>("rs", sinks.begin(), sinks.end());
	logger->set_level(spdlog::level::info);

	spdlog::register_logger(logger);
}

void CLogger::Shutdown()
{
	spdlog::shutdown();
}
