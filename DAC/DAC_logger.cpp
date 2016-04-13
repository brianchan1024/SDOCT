#include "DAC_logger.h"

std::string dac_format_func(std::string func) {
	std::cout << func << std::endl;
	return func;
}

//Defines a global logger initialization routine
BOOST_LOG_GLOBAL_LOGGER_INIT(DAC_logger, logger_t)
{
	logger_t lg;

	logging::core::get()->add_global_attribute("Function", attrs::mutable_constant<std::string>(""));

	logging::add_file_log
		(
		keywords::file_name = "../log/sdoct_0.log",                                        // file name pattern
		keywords::rotation_size = 10 * 1024 * 1024,                                   // rotate files every 10 MiB... 
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0), // ...or at midnight
		keywords::open_mode = std::ios::app,
		keywords::auto_flush = true,
		//keywords::format = "[%TimeStamp%]: %Severity% %Message%"                      // log record format
		keywords::format = 
		(
		expr::stream
		<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
		<< std::hex
		<< " <" << logging::trivial::severity << '>'
		//<< "\t["   << expr::attr<std::string>("Function") << ']'
		//<< "\tmsg: " << expr::smessage
		<< "\t" << expr::smessage
		)
		);
	logging::add_common_attributes();
	return lg;
}