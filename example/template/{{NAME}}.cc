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

{{#PROTOLIB}}#include "{{PROTOLIB_NAME}}.pb.h"{{/PROTOLIB}}

{{#SERVICES}}int Create{{SERVICE_NAME}}(uint16_t wPort)
{
	int fd = socket(PF_INET, SOCK_STREAM, 0);

	sockaddr_in addr;
	bzero(&addr, sizeof(sockaddr_in));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(wPort);
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");

	int reused = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reused, sizeof(int));

	bind(fd, (const sockaddr*)&addr, sizeof(sockaddr_in));

	listen(fd, 10);
	return fd;
}

{{#METHODS}}bool On{{METHOD_NAME}}(int fd, char* buffer, ssize_t size)
{
	{{#INPUT_TYPE}}::{{#PACKAGE}}{{PACKAGE_NAME}}::{{/PACKAGE}}{{TYPE_NAME}}{{/INPUT_TYPE}} stReq;
	{{#OUTPUT_TYPE}}::{{#PACKAGE}}{{PACKAGE_NAME}}::{{/PACKAGE}}{{TYPE_NAME}}{{/OUTPUT_TYPE}} stResp;

	if(!stReq.ParseFromArray(buffer, size))
		return false;

	// implement code
	stResp.set_seq(stReq.seq() + 1);

	std::string retMsg = stReq.message();
	retMsg.append(" from server");
	stResp.set_message(retMsg);

	std::string data = stResp.SerializeAsString();
	send(fd, data.c_str(), data.length(), 0);
	return true;
}

{{/METHODS}}
void {{SERVICE_NAME}}Loop(int fd)
{
	int epfd = epoll_create(100);

	epoll_event ev;
	bzero(&ev, sizeof(epoll_event));
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

	while(true)
	{
		bzero(&ev, sizeof(epoll_event));
		epoll_wait(epfd, &ev, 1, -1);
		if(ev.data.fd == fd)
		{
			sockaddr_in cliaddr;
			socklen_t size = sizeof(sockaddr_in);
			int clifd = accept(fd, (sockaddr*)&cliaddr, &size);
			printf("[%s:%d] connected.\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

			ev.data.fd = clifd;
			ev.events = EPOLLIN;
			epoll_ctl(epfd, EPOLL_CTL_ADD, clifd, &ev);
		}
		else
		{
			char buffer[65535];
			ssize_t recvSize = recv(ev.data.fd, buffer, 65535, 0);
			if(recvSize == 0)
			{
				sockaddr_in cliaddr;
				socklen_t size = sizeof(sockaddr_in);
				getpeername(ev.data.fd, (sockaddr*)&cliaddr, &size);
				printf("[%s:%d] disconnected.\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

				epoll_ctl(epfd, EPOLL_CTL_DEL, ev.data.fd, NULL);
			}
			else
			{
				{{#METHODS}}
					if(On{{METHOD_NAME}}(ev.data.fd, buffer, recvSize))
						continue;
				{{/METHODS}}
			}
		}
	}
}

{{/SERVICES}}


