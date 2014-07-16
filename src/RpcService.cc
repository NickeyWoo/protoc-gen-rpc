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
#include "SimpleRpcGenerator.hpp"
#include "RpcService.hpp"


RpcService::RpcService(const google::protobuf::ServiceDescriptor* service) :
	m_Service(service)
{
}

void RpcService::GenerateHeader(google::protobuf::io::Printer* printer)
{
}

void RpcService::GenerateSource(google::protobuf::io::Printer* printer)
{
}


