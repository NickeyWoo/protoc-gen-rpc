#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <utility>
#include <string>
#include <vector>
#include <map>
#include <boost/format.hpp>
#include <google/protobuf/compiler/cpp/cpp_generator.h>

#include <vector>
#include <utility>

#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.pb.h>

#include "Log.hpp"

Log::Log() :
	m_fd(-1)
{
}

Log& Log::Instance()
{
	static Log instance;
	if(instance.m_fd == -1)
		instance.m_fd = open("./build.log", O_RDWR|O_CREAT|O_APPEND, 0666);
	return instance;
}

Log& Log::operator << (std::string str)
{
	write(m_fd, str.c_str(), str.length());
	return *this;
}


