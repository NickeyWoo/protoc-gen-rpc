/*++
 *
 * SimpleRpc Generator
 *
 *
--*/
#ifndef __RPCSERVICE_HPP__
#define __RPCSERVICE_HPP__

class RpcService
{
public:
	RpcService(const google::protobuf::ServiceDescriptor* service);

	void GenerateHeader(google::protobuf::io::Printer* printer);
	void GenerateSource(google::protobuf::io::Printer* printer);

private:
	const google::protobuf::ServiceDescriptor* m_Service;
};

#endif // define __RPCSERVICE_HPP__
