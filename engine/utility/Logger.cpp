#include "Logger.hpp"

void initBoostLogging()
{
#if 0	
	namespace logging = boost::log;
	namespace keywords = boost::log::keywords;

	logging::register_simple_formatter_factory <
		logging::trivial::severity_level, 
		char
	> ("Severity");

	logging::add_console_log(
		std::cout, 
		keywords::format = 
			"[%TimeStamp%] "
			"[%ThreadID%] "
			"[%Severity%] "
			"[%ProcessID%] "
			"[%LineID%] "
			"%Message%"
	);

	logging::core::get()->set_filter(
		logging::trivial::severity >= logging::trivial::trace
	);

	logging::add_common_attributes();
#endif	
}