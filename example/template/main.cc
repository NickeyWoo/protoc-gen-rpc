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

{{#SERVICES}}int Create{{SERVICE_NAME}}(uint16_t wPort);
void {{SERVICE_NAME}}Loop(int fd);
{{/SERVICES}}

void print_usage()
{
	printf("Usage: {{NAME}} [port]\n");
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		print_usage();
		return -1;
	}

	{{#SERVICES}}{{SERVICE_NAME}}Loop(
		Create{{SERVICE_NAME}}(
			strtoul(argv[1], NULL, 10)
		)
	);
	{{/SERVICES}}
	return 0;
}

