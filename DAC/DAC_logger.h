#pragma once

#include <boost/log/expressions.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include <fstream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/mutable_constant.hpp>

#define DAC_LOGGER_TRACE  BOOST_LOG_SEV(DAC_logger::get(), boost::log::trivial::trace)
#define DAC_LOGGER_INFO  BOOST_LOG_SEV(DAC_logger::get(), boost::log::trivial::info)
#define DAC_LOGGER_DEBUG  BOOST_LOG_SEV(DAC_logger::get(), boost::log::trivial::debug)
#define DAC_LOGGER_WARNING  BOOST_LOG_SEV(DAC_logger::get(), boost::log::trivial::warning)
#define DAC_LOGGER_ERROR BOOST_LOG_SEV(DAC_logger::get(), boost::log::trivial::error)

#define SYS_LOGFILE             "example.log"

namespace attrs   = boost::log::attributes;
namespace expr    = boost::log::expressions;
namespace logging = boost::log;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

//Narrow-char thread-safe logger.
typedef boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> logger_t;


//declares a global logger with a custom initialization
BOOST_LOG_GLOBAL_LOGGER(DAC_logger, logger_t)

#define CUSTOM_LOG(logger, sev) \
	BOOST_LOG_STREAM_WITH_PARAMS( \
	(logger), \
	(set_get_attrib("Function", dac_format_func(__FUNCTION__))) \
	(::boost::log::keywords::severity = (boost::log::trivial::sev)) \
	)

	// Set attribute and return the new value
template<typename ValueType>
ValueType set_get_attrib(const char* name, ValueType value) {
	auto attr = logging::attribute_cast<attrs::mutable_constant<ValueType>>(logging::core::get()->get_global_attributes()[name]);
	attr.set(value);
	return attr.get();
}

// do nothing here actually, but deleted, cause expection
std::string dac_format_func(std::string func);