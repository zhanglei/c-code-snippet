#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
	struct hostent *host_entry;
	char **host_aliases;
	char **ip_addr;
	char str[32];
	if (!(host_entry = gethostbyname("i2.api.weibo.com")))
	{
		printf("%d", errno);
	}
	else
	{
		/* 将主机的规范名打出来 */
		printf("official hostname:%s\n",host_entry->h_name);
		/* 主机可能有多个别名，将所有别名分别打出来 */
		for(host_aliases = host_entry->h_aliases; *host_aliases != NULL; host_aliases++)
			printf("alias:%s\n",*host_aliases);
		/* 将主机的规范名打出来 */
		switch(host_entry->h_addrtype)
		{
			case AF_INET:
			case AF_INET6:
				ip_addr=host_entry->h_addr_list;
				/* 将刚才得到的所有地址都打出来。其中调用了inet_ntop()函数 */
				for(;*ip_addr!=NULL;ip_addr++)
					printf("address:%s\n", inet_ntop(host_entry->h_addrtype, *ip_addr, str, sizeof(str)));
			break;
			default:
				printf("unknown address type\n");
			break;
		}
	}
}