/*++
 *
 * SimpleRpc Generator
 *
 *
--*/
#ifndef __SIMPLERPC_GENERATOR_HPP__
#define __SIMPLERPC_GENERATOR_HPP__

#include <string>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/cpp/cpp_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/io/printer.h>

class SimpleRpcGenerator :
	public google::protobuf::compiler::CodeGenerator
{
public:
	SimpleRpcGenerator();
	~SimpleRpcGenerator();

	bool Check(const google::protobuf::FileDescriptor* file) const;
	bool GenerateInitialize(google::protobuf::compiler::GeneratorContext* generator_context) const;

	bool Generate(const google::protobuf::FileDescriptor* file,
				  const std::string& parameter,
				  google::protobuf::compiler::GeneratorContext* generator_context,
				  std::string* error) const;

private:
	GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(SimpleRpcGenerator);
};

#endif // __SIMPLERPC_GENERATOR_HPP__
