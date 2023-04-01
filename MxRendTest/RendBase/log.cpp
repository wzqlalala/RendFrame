#include "log.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/common.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"



namespace mxr
{
	std::shared_ptr<spdlog::logger> Log::logger;
	void Log::Init()
	{
		if (!logger)
		{

			using wincolor_sink_ptr = std::shared_ptr<spdlog::sinks::stdout_color_sink_mt>;
			std::vector<spdlog::sink_ptr> sinks;  // pointers to sinks that support setting custom color
			sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());  // console sink
			sinks[0]->set_pattern("%^%T > [%L] %v%$");  // e.g. 23:55:59 > [I] sample message
			//sinks[0]->set_color(spdlog::level::trace, sinks[0]->CYAN);
			//sinks[0]->set_color(spdlog::level::debug, sinks[0]->BOLD);


			std::string logger_name = "log";
			std::size_t max_size = 1024 * 1024 * 10;
			std::size_t max_files = 10;
			sinks.emplace_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logger_name, max_size, max_files));
			sinks[1]->set_pattern("%^%T > [%L] %v%$");  // e.g. 23:55:59 > [I] sample message

			logger = std::make_shared<spdlog::logger>("MxRender", begin(sinks), end(sinks));
			spdlog::register_logger(logger);
			logger->set_level(spdlog::level::trace);  // log level less than this will be silently ignored
			logger->flush_on(spdlog::level::trace);   // the minimum log level that will trigger automatic flush
		}
	}

	void Log::Shutdown()
	{
		spdlog::shutdown();
	}

	
	

	

	


}

