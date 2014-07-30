#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

#include <utility>
#include <string>
#include <vector>
#include <map>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/scoped_ptr.hpp>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/compiler/cpp/cpp_generator.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.pb.h>

#include "SimpleRpcGenerator.hpp"

SimpleRpcGenerator::SimpleRpcGenerator() {}

SimpleRpcGenerator::~SimpleRpcGenerator() {}

void SimpleRpcGenerator::GetTypeDict(const google::protobuf::Descriptor* pTypeDesc,
									 ctemplate::TemplateDictionary* pTypeDict) const
{
	pTypeDict->SetValue("TYPE_NAME", pTypeDesc->name());

	const google::protobuf::FileDescriptor* pTypeFileDesc = pTypeDesc->file();
	if(!pTypeFileDesc->package().empty())
	{
		std::vector<std::string> vPackages;
		boost::split(vPackages, pTypeFileDesc->package(), boost::is_any_of("."));
		for(std::vector<std::string>::iterator iter = vPackages.begin();
			iter != vPackages.end();
			++iter)
		{
			ctemplate::TemplateDictionary* pPackageSectionDict = pTypeDict->AddSectionDictionary("PACKAGES");
			pPackageSectionDict->SetValue("PACKAGE_NAME", *iter);
		}
	}
}

void SimpleRpcGenerator::GetOptions(const google::protobuf::UnknownFieldSet* pOptionSet,
								    ctemplate::TemplateDictionary* pDict) const
{
	for(int i=0; i<pOptionSet->field_count(); ++i)
	{
		const ::google::protobuf::UnknownField& stField = pOptionSet->field(i);
		ctemplate::TemplateDictionary* pOptionSectionDict = pDict->AddSectionDictionary("OPTIONS");

		ctemplate::TemplateDictionary* pValueSectionDict = pOptionSectionDict->AddSectionDictionary(
			(boost::format("OPTION_%u") % stField.number()).str());

		switch(stField.type())
		{
		case ::google::protobuf::UnknownField::TYPE_LENGTH_DELIMITED:
			pValueSectionDict->SetValue("OPTION_VALUE", stField.length_delimited());
			pValueSectionDict->ShowSection((boost::format("OPTION_%u_%s") % stField.number() % stField.length_delimited()).str());
			break;
		case ::google::protobuf::UnknownField::TYPE_VARINT:
			pValueSectionDict->SetIntValue("OPTION_VALUE", stField.varint());
			pValueSectionDict->ShowSection((boost::format("OPTION_%u_%lu") % stField.number() % stField.varint()).str());
			break;
		case ::google::protobuf::UnknownField::TYPE_FIXED32:
			pValueSectionDict->SetIntValue("OPTION_VALUE", stField.fixed32());
			pValueSectionDict->ShowSection((boost::format("OPTION_%u_%u") % stField.number() % stField.fixed32()).str());
			break;
		case ::google::protobuf::UnknownField::TYPE_FIXED64:
			pValueSectionDict->SetIntValue("OPTION_VALUE", stField.fixed64());
			pValueSectionDict->ShowSection((boost::format("OPTION_%u_%lu") % stField.number() % stField.fixed64()).str());
			break;
		default:
			pValueSectionDict->SetValue("OPTION_VALUE", "");
			break;
		}
	}
}

void SimpleRpcGenerator::GetServicesDict(const google::protobuf::FileDescriptor* pFileDesc,
										 ctemplate::TemplateDictionary* pProtoDict) const
{
	pProtoDict->SetValue("FILE", pFileDesc->name());
	pProtoDict->SetValue("NAME", pFileDesc->service(0)->name());

	time_t now = time(NULL);
	tm tmval;
	localtime_r(&now, &tmval);

	ctemplate::TemplateDictionary* pDateSectionDict = pProtoDict->AddSectionDictionary("DATE");
	pDateSectionDict->SetIntValue("YEAR", tmval.tm_year + 1900);
	pDateSectionDict->SetIntValue("MONTH", tmval.tm_mon + 1);
	pDateSectionDict->SetIntValue("DAY", tmval.tm_mday);

	if(pFileDesc->message_type_count() > 0 ||
		pFileDesc->enum_type_count() > 0 ||
		pFileDesc->extension_count() > 0)
	{
		ctemplate::TemplateDictionary* pProtolibSectionDict = pProtoDict->AddSectionDictionary("PROTOLIB");

		std::string strFile = pFileDesc->name();
		boost::replace_all(strFile, ".proto", "");
		pProtolibSectionDict->SetValue("PROTOLIB_NAME", strFile);
	}

	if(!pFileDesc->package().empty())
	{
		std::vector<std::string> vPackages;
		boost::split(vPackages, pFileDesc->package(), boost::is_any_of("."));
		for(std::vector<std::string>::iterator iter = vPackages.begin();
			iter != vPackages.end();
			++iter)
		{
			ctemplate::TemplateDictionary* pPackageSectionDict = pProtoDict->AddSectionDictionary("PACKAGES");
			pPackageSectionDict->SetValue("PACKAGE_NAME", *iter);
		}
	}

	for(int i=0; i<pFileDesc->service_count(); ++i)
	{
		const google::protobuf::ServiceDescriptor* pServiceDesc = pFileDesc->service(i);

		ctemplate::TemplateDictionary* pServiceSectionDict = pProtoDict->AddSectionDictionary("SERVICES");
		pServiceSectionDict->SetValue("SERVICE_NAME", pServiceDesc->name());

		for(int j=0; j<pServiceDesc->method_count(); ++j)
		{
			const google::protobuf::MethodDescriptor* pMethodDesc = pServiceDesc->method(j);
			ctemplate::TemplateDictionary* pMethodSectionDict = pServiceSectionDict->AddSectionDictionary("METHODS");
			pMethodSectionDict->SetValue("METHOD_NAME", pMethodDesc->name());

			GetTypeDict(pMethodDesc->input_type(), pMethodSectionDict->AddSectionDictionary("INPUT_TYPE"));
			GetTypeDict(pMethodDesc->output_type(), pMethodSectionDict->AddSectionDictionary("OUTPUT_TYPE"));

			GetOptions(&pMethodDesc->options().unknown_fields(), pMethodSectionDict);
		}

		GetOptions(&pServiceDesc->options().unknown_fields(), pServiceSectionDict);
	}
	GetOptions(&pFileDesc->options().unknown_fields(), pProtoDict);
}

void SimpleRpcGenerator::PushData(std::string strData,
							 	  ctemplate::TemplateDictionary* pDataSectionDict) const
{
	std::string::size_type pos = strData.find_first_of(':');
	if(pos == std::string::npos)
		return;

	std::string strKey = strData.substr(0, pos);
	boost::trim(strKey);
	std::string strVal = strData.substr(pos + 1);
	boost::trim(strVal);

	if(strKey.empty() || strVal.empty())
		return;

	pDataSectionDict->SetValue(strKey, strVal);
}

bool SimpleRpcGenerator::Generate(const google::protobuf::FileDescriptor* file,
								  const std::string& parameter,
								  google::protobuf::compiler::GeneratorContext* generator_context,
								  std::string* error) const
{
	char* szTemplatePath = getenv("PROTOCGENRPC_TEMPLATE");
	if(!szTemplatePath)
	{
		error->append("[error] not set PROTOCGENRPC_TEMPLATE env.");
		return false;
	}

	if(file->service_count() != 1)
	{
		error->append("[error] only support one proto service.");
		return false;
	}

	ctemplate::TemplateDictionary stProtoDict("PROTO");
	GetServicesDict(file, &stProtoDict);

	ctemplate::TemplateDictionary* pDataSectionDict = stProtoDict.AddSectionDictionary("DATA");
	char* szDataCount = getenv("PROTOCGENRPC_DATACOUNT");
	uint32_t dwDataCount = szDataCount==NULL?0:strtoul(szDataCount, NULL, 10);
	for(uint32_t i=0; i<dwDataCount; ++i)
	{
		char* szData = getenv((boost::format("PROTOCGENRPC_DATA_%u") % i).str().c_str());
		if(!szData)
			continue;

		std::string strData = std::string(szData);
		PushData(strData, pDataSectionDict);
	}

	if(strcmp(szTemplatePath, "") == 0)
	{
		stProtoDict.DumpToString(error);
		return false;
	}

	DIR* pDir = opendir(szTemplatePath);
	if(pDir == NULL)
	{
		error->append((boost::format("[error] open PROTOCGENRPC_TEMPLATE dir fail, %s.") % strerror(errno)).str());
		return false;
	}

	dirent* pstInfo;
	while((pstInfo = readdir(pDir)) != NULL)
	{
		if(pstInfo->d_type != DT_REG)
			continue;

		ctemplate::Template* pTemplate = ctemplate::Template::StringToTemplate(pstInfo->d_name, ctemplate::DO_NOT_STRIP);
		std::string strOutFile;
		pTemplate->Expand(&strOutFile, &stProtoDict);
		delete pTemplate;

		std::string strTempleFile = (boost::format("%s/%s") % szTemplatePath % pstInfo->d_name).str();
		std::string strOutput;
		ctemplate::ExpandTemplate(strTempleFile, ctemplate::DO_NOT_STRIP, &stProtoDict, &strOutput);

		boost::scoped_ptr<google::protobuf::io::ZeroCopyOutputStream> output_stream(
			generator_context->Open(strOutFile));
		google::protobuf::io::Printer ioPrinter(output_stream.get(), 0x0);
		ioPrinter.Print(strOutput.c_str());
	}

	closedir(pDir);
	return true;
}



