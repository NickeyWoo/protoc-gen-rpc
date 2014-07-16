#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <boost/format.hpp>
#include "Log.hpp"
#include "SimpleRpcGenerator.hpp"

int main(int argc, char* argv[])
{
	SimpleRpcGenerator generator;
	return ::google::protobuf::compiler::PluginMain(argc, argv, &generator);
}


