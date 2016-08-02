/*
 * =====================================================================================
 *
 *       Filename:  poll.c
 *
 *    Description:  poll.c
 *
 *        Version:  1.0
 *        Created:  01/26/2014 06:08:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dingjing 
 *        Company:  weibo
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <asm/ioctls.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#define MAX_CONNECTION_NUM 1024
#define TIMEOUT 50000

int main(int argc, char **argv)
{
    char *ip;
    int _ip, startport, endport;
    in_addr_t startip;
    in_addr_t endip;
    int sockfd, ret;
    int flags;
    struct pollfd fds;
    struct sockaddr_in addr; /* 定义addr为sockaddr_in类型 */
    void help()
    {
        system("clear");
        printf("Usage:\n");
        printf("scan startip endip startport endport\n");
        printf("scan 192.168.1.1 192.168.1.255 20 50\n");
        printf("scan 192.168.1.1 20 50\n");
    }
    if (argc!=4 && argc!=5)
    {
        help();
        return 0;
    }
    if (4 == argc)
    {
        ip=argv[1];
        startport=atoi(argv[2]); /* 转换为长整型 */
        endport=atoi(argv[3]);
        if (startport<1||endport>65535)
        {
            printf("port error 1<port<65535\n");
            return 0;
        }
                                    
        addr.sin_family = AF_INET; /* addr结构体中的sin_family定义地址族 */
        addr.sin_addr.s_addr = inet_addr(ip); /* 填写IP，并转换成in_addr */

        int i;
        for (i = startport; i <= endport; i++)
        {   
            printf("try to connecting %s:%d...\n", ip, i);

            /*************************************************************/
            /* Create an AF_INET stream socket to receive incoming       */
            /* connections on                                            */
            /*************************************************************/
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            printf("%d\n", sockfd);            
            if (-1 == sockfd)
            {
                printf("Err number: %d\n", errno);
                perror("socket");
                //exit( -1 );
                continue;
            }
            /*************************************************************/
            /* Bind the socket                                           */
            /*************************************************************/
            addr.sin_port = htons(i);
            flags = fcntl(sockfd, F_GETFL, 0);
            fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
            int nRet = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
                         
            if (-1 == nRet)
            {         
                if (errno != EINPROGRESS)
                {
                    perror("connect");
                    //return -1;
                    continue;
                }
                
                fds.fd = sockfd; //设置监控sockfd  
                fds.events = POLLIN|POLLPRI; //设置监控的事件 
            }
            else if (0 == nRet)
            {     
                close(sockfd);
                printf("Connected: %s %d open\n", ip, i);
            }
        }
        while(1)
        {
            ret = poll(&fds, sizeof(fds), 5000); //查询的设备队列是fds，里面有1个设备，查询不到就睡眠5s，在睡眠中如果有期望状态出现也是可以返回
            if (ret == 0)
            {
                close(sockfd); 
                printf("time out\n"); //没有查询到按键按下，睡眠中也没有按键按下
            }
            else
            {    
                read(sockfd, NULL, 1); //查询到按键按下，读取这个按键的值
                printf("%s %d open\n", ip, i);
                close(sockfd); 
            }
        }
    } 
    else if (5 == argc)
    {
        startip = ntohl(inet_addr(argv[1]));
        endip = ntohl(inet_addr(argv[2]));
        
        if (endip < startip)
        {
            in_addr_t nswap = endip;
            endip = startip;
            startip = nswap;
        }
        
        startport = atoi(argv[3]);
        endport = atoi(argv[4]);
        if (startport<1||endport>65535)
        {
            printf("port error 1<port<65535\n");
            return 0;
        }
                        
        addr.sin_family = AF_INET; 
        for (_ip = startip; _ip <= endip; _ip++)
        {
            addr.sin_addr.s_addr = htonl(_ip);
            int i;
            for (i =startport; i <=endport ; i++)
            {
                printf("try to connecting %s:%d...\n", inet_ntoa(addr.sin_addr), i);

                addr.sin_port = htons(i); 
                
                sockfd = socket(AF_INET, SOCK_STREAM, 0);/* 创建套接字 */
                printf("%d\n", sockfd);        
                if (-1 == sockfd)
                {
                    printf("Err number: %d\n", errno);
                    perror("socket");
                    //exit(-1);
                    continue;
                }
                
                flags = fcntl(sockfd, F_GETFL, 0);
                fcntl(sockfd, F_SETFL, flags|O_NONBLOCK);
                int nRet = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
                             
                if (-1 == nRet)
                {         
                    if(errno != EINPROGRESS)
                    {
                        perror("connect");
                        continue;
                    }
                    
                    fds.fd = sockfd; //设置监控sockfd  
                    fds.events = POLLIN|POLLPRI; //设置监控的事件
                }
                else if( 0 == nRet )
                {     
                    close(sockfd);
                    printf("Connected: %s %d open\n", _ip, i);
                }
            }
            while(1)
            {
                ret = poll(&fds, sizeof(fds), TIMEOUT);//查询的设备队列是fds，里面有1个设备，查询不到就睡眠5s，在睡眠中如果有期望状态出现也是可以返回
                if (ret == 0)
                {
                    close(sockfd); 
                    printf("time out\n"); //没有查询到按键按下，睡眠中也没有按键按下
                }
                else
                {    
                    read(sockfd, NULL, 1);           //查询到按键按下，读取这个按键的值
                    printf("%s %d open\n", _ip, i);
                    close(sockfd); 
                }
            }        
        }
    }
    return 0;
}