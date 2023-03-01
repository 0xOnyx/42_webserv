#include "syslog.hpp"

Syslog::Syslog()
{
	openlog(NAME, LOG_PERROR | LOG_PID | LOG_NOWAIT, LOG_USER);
	setlogmask(LOG_MASK(LOG_ERR) | LOG_MASK(LOG_WARNING) | LOG_MASK(LOG_INFO) | LOG_MASK(LOG_DEBUG));
	//setlogmask(LOG_MASK(LOG_INFO));
}

Syslog::~Syslog()
{
	closelog();
}