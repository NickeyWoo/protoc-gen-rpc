/*++
 *
 * SimpleRpc Generator
 *
 *
--*/
#ifndef __SIMPLERPC_GENERATOR_HPP__
#define __SIMPLERPC_GENERATOR_HPP__

#include <string>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/unknown_field_set.h>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/cpp/cpp_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/io/printer.h>

#include <ctemplate/template.h>

class SimpleRpcGenerator :
	public google::protobuf::compiler::CodeGenerator
{
public:
	SimpleRpcGenerator();
	~SimpleRpcGenerator();

	void GetServicesDict(const google::protobuf::FileDescriptor* pFileDesc, 
						 ctemplate::TemplateDictionary* pTypeDict) const;

	void GetTypeDict(const google::protobuf::Descriptor* pTypeDesc,
					 ctemplate::TemplateDictionary* pProtoDict) const;

	void GetOptions(const google::protobuf::UnknownFieldSet* pOptionSet,
				    ctemplate::TemplateDictionary* pDict) const;

	bool Generate(const google::protobuf::FileDescriptor* file,
				  const std::string& parameter,
				  google::protobuf::compiler::GeneratorContext* generator_context,
				  std::string* error) const;

private:
	GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(SimpleRpcGenerator);
};

#endif // __SIMPLERPC_GENERATOR_HPP__
