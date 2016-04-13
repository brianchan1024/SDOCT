
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/attributes/mutable_constant.hpp>

namespace logging  = boost::log;
namespace attrs    = boost::log::attributes;
namespace expr     = boost::log::expressions;
namespace src      = boost::log::sources;
namespace keywords = boost::log::keywords;

#if 0
// New macro that includes severity, filename and line number
#define CUSTOM_LOG(logger, sev) \
	BOOST_LOG_STREAM_WITH_PARAMS( \
	(logger), \
	(set_get_attrib("Function", format_function(__FUNCTION__))) \
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
std::string format_function(std::string func) {
	return func;
}

void init2() {
	// New attributes that hold filename and line number
	logging::core::get()->add_global_attribute("Function", attrs::mutable_constant<std::string>(""));

	logging::add_file_log (
		keywords::file_name = "sample.log",
		keywords::format = 
		(
		expr::stream
		<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
		<< std::hex
		<< " <" << logging::trivial::severity << '>'
		<< "\t["   << expr::attr<std::string>("Function") << ']'
		<< "\tmsg: " << expr::smessage
		)
		);

	logging::add_common_attributes();
}

int main(int argc, char* argv[]) {
	init2();
	src::severity_logger<logging::trivial::severity_level> lg;

	CUSTOM_LOG(lg, debug) << "A regular message";
	return 0;
}
#endif