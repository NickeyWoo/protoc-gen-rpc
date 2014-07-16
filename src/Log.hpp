/*++
 *
 *
 *
--*/
#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

#ifndef BUILD_LOG
#define BUILD_LOG(msg)		\
				Log::Instance() << msg;
#endif

class Log
{
public:
	static Log& Instance();
	Log& operator << (std::string str);

private:
	Log();
	int m_fd;
};


#endif // define __LOG_HPP__
