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
#include <boost/scoped_ptr.hpp>
#include <google/protobuf/compiler/cpp/cpp_generator.h>

#include <vector>
#include <utility>

#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.pb.h>

#include "Log.hpp"
#include "SimpleRpcGenerator.hpp"
#include "RpcService.hpp"

SimpleRpcGenerator::SimpleRpcGenerator() {}

SimpleRpcGenerator::~SimpleRpcGenerator() {}

bool SimpleRpcGenerator::Check(const google::protobuf::FileDescriptor* file) const
{
	return (file->message_type_count() > 0 ||
			file->enum_type_count() > 0 ||
			file->extension_count() > 0);
}

bool SimpleRpcGenerator::GenerateInitialize(google::protobuf::compiler::GeneratorContext* generator_context, const char* szTemplates) const
{
	
	return true;
}

bool SimpleRpcGenerator::Generate(const google::protobuf::FileDescriptor* file,
								  const std::string& parameter,
								  google::protobuf::compiler::GeneratorContext* generator_context,
								  std::string* error) const
{
	BUILD_LOG((boost::format("=========================== <<BEGIN CODE GENERATE %s>> ===========================\n") % file->name()).str());

	char* szTemplatePath = getenv("PROTOCGENRPC_TEMPLATE");
	if(!szTemplatePath)
	{
		error->append("[error] not set PROTOCGENRPC_TEMPLATE env.");
		BUILD_LOG((boost::format("%s\n") % *error).str());
		BUILD_LOG((boost::format("============================ <<END CODE GENERATE %s>> ============================\n") % file->name()).str());
		return false;
	}

	BUILD_LOG(">>> INITIALIZE PROJECT ENV\n");
	if(!GenerateInitialize(generator_context, szTemplatePath))
	{
		error->append("[error] initialize project env fail.");
		BUILD_LOG((boost::format("%s\n") % *error).str());
		BUILD_LOG((boost::format("============================ <<END CODE GENERATE %s>> ============================\n") % file->name()).str());
		return false;
	}

	BUILD_LOG((boost::format(">>> CHECKING %s\n") % file->name()).str());
	if(Check(file))
	{
		BUILD_LOG(">>> GENERATE DEFAULT CODE\n");
		::google::protobuf::compiler::cpp::CppGenerator cpp;
		bool bRet = cpp.Generate(file, parameter, generator_context, error);
		if(!bRet)
		{
			BUILD_LOG((boost::format("%s\n") % *error).str());
			BUILD_LOG((boost::format("============================ <<END CODE GENERATE %s>> ============================\n") % file->name()).str());
			return bRet;
		}
	}

	BUILD_LOG(">>> GENERATE SERVICE CODE\n");
	for(int i=0; i<file->service_count(); ++i)
	{
		const google::protobuf::ServiceDescriptor* pServiceDesc = file->service(i);
		RpcService service_generator(pServiceDesc);

		BUILD_LOG((boost::format("service name: %s\n") % pServiceDesc->name()).str());

		// Generate SERVICES Header
		{
			boost::scoped_ptr<google::protobuf::io::ZeroCopyOutputStream> output(
				generator_context->Open((boost::format("%s.hpp") % pServiceDesc->name()).str()));
			google::protobuf::io::Printer printer(output.get(), '$');
			service_generator.GenerateHeader(&printer);
		}

		// Generate SERVICES Source
		{
			boost::scoped_ptr<google::protobuf::io::ZeroCopyOutputStream> output(
				generator_context->Open((boost::format("%s.cc") % pServiceDesc->name()).str()));
			google::protobuf::io::Printer printer(output.get(), '$');
			service_generator.GenerateSource(&printer);
		}
	}

	BUILD_LOG((boost::format("============================ <<END CODE GENERATE %s>> ============================\n") % file->name()).str());
	return true;
}



