#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/epoll.h>

#include <utility>
#include <string>
#include <vector>

{{#PROTOLIB}}#include "{{PROTOLIB_NAME}}.pb.h"{{/PROTOLIB}}

int main(int argc, char* argv[])
{
	int fd = socket(PF_INET, SOCK_STREAM, 0);

	sockaddr_in addr;
	bzero(&addr, sizeof(sockaddr_in));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(strtoul(argv[1], NULL, 10));
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");

	connect(fd, (const sockaddr*)&addr, sizeof(sockaddr_in));
	
	char buffer[65535];
	ssize_t sz = 0;
	{{#SERVICES}}
	{{#METHODS}}

	{{#INPUT_TYPE}}::{{#PACKAGE}}{{PACKAGE_NAME}}::{{/PACKAGE}}{{TYPE_NAME}}{{/INPUT_TYPE}} stReq{{METHOD_NAME}}Msg;
	{{#OUTPUT_TYPE}}::{{#PACKAGE}}{{PACKAGE_NAME}}::{{/PACKAGE}}{{TYPE_NAME}}{{/OUTPUT_TYPE}} stResp{{METHOD_NAME}}Msg;

	stReq{{METHOD_NAME}}Msg.set_seq(random());
	stReq{{METHOD_NAME}}Msg.set_message("hello nickeywoo");
	send(fd, stReq{{METHOD_NAME}}Msg.SerializeAsString().c_str(), stReq{{METHOD_NAME}}Msg.SerializeAsString().length(), 0);

	bzero(buffer, 65535);
	sz = recv(fd, buffer, 65535, 0);
	if(sz == 0)
		return 0;

	stResp{{METHOD_NAME}}Msg.ParseFromArray(buffer, sz);

	printf("send: %s\n", stReq{{METHOD_NAME}}Msg.DebugString().c_str());
	printf("recv: %s\n", stResp{{METHOD_NAME}}Msg.DebugString().c_str());
	{{/METHODS}}
	{{/SERVICES}}
	return 0;
}


