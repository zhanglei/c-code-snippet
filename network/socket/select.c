/*
 * =====================================================================================
 *
 *       Filename:  select.c
 *
 *    Description:  select.c
 *
 *        Version:  1.0
 *        Created:  01/26/2014 06:08:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dingjing 
 *        Company:  sina
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <asm/ioctls.h>
#include <errno.h>
#include <sys/select.h>
#define ZERO (fd_set *)0
#define TIMEOUT 50000

int main(int argc, char** argv)
{
    char *ip;
    int port, startport, endport;
    in_addr_t _ip, startip, endip;
    int sockfd;
    struct sockaddr_in addr; /* 定义addr为sockaddr_in类型 */
    int error;
    fd_set readfds, writefds;
    struct timeval timeout;
    unsigned long non_blocking = 1;
    unsigned long blocking = 0;

    timeout.tv_sec=0; 
    timeout.tv_usec=TIMEOUT;

    void help()
    {
        system("clear");
        printf("Usage:\n");
        printf("scan startip endip startport endport\n");
        printf("scan 192.168.1.1 192.168.1.100 20 50\n");
        printf("scan 192.168.1.1 20 50\n");
    }
    if (argc!=4 && argc!=5)
    {
        help();
        return 0;
    }

    if (4 == argc)
    {
        ip = argv[1];
        startport = atoi(argv[2]); /* 转换为长整型 */
        endport = atoi(argv[3]);
        if (startport<1 || endport>65535)
        {
            printf("port error 1<port<65535\n");
            return 0;
        }               
        addr.sin_family = AF_INET; /* addr结构体中的sin_family定义地址族 */
        addr.sin_addr.s_addr = inet_addr(ip); /* 填写IP，并转换成in_addr */
        
        for (port = startport; port <= endport ; port++)
        {   
            printf("try to connecting %s:%d...\n", ip, port);

            /*************************************************************/
            /* Create an AF_INET stream socket to receive incoming       */
            /* connections on                                            */
            /*************************************************************/
            sockfd = socket(AF_INET, SOCK_STREAM, 0);         
            if (-1 == sockfd)
            {
                printf("Err number: %d\n", errno);
                perror("socket");
                continue;
            }
            /*************************************************************/
            /* Bind the socket                                           */
            /*************************************************************/
            addr.sin_port = htons(port);
            ioctl(sockfd, FIONBIO, &non_blocking);
            int nRet = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));

            if (-1 == nRet)
            {         
                if(errno != EINPROGRESS)
                {
                    perror("connect");
                    continue;
                }

                FD_ZERO(&writefds);
                FD_SET(sockfd, &writefds);
                switch (select(sockfd + 1, &readfds, &writefds, NULL, &timeout)) 
                {
                    case -1:
                        close(sockfd);
                        printf("select error\n"); 
                        perror("select");
                        break;
                    case 0:
                        close(sockfd); 
                        break;
                    default:
                        if (FD_ISSET(sockfd, &writefds)) //测试sock是否可读
                        { 
                            printf("Connected: %s %d open\n", ip, port);
                            close(sockfd);
                        }
                        break;
                }
            }
            else if (0 == nRet)
            {
                close(sockfd);
                printf("Connected: %s %d open\n", ip, port);
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
        
        startport=atoi(argv[3]);
        endport=atoi(argv[4]);
        if (startport<1 || endport>65535)
        {
            printf("port error 1<port<65535\n");
            return 0;
        }
                        
        addr.sin_family=AF_INET;
        
        for (_ip = startip; _ip <= endip; _ip++)
        {
            addr.sin_addr.s_addr = htonl(_ip);
        
            for (port = startport; port <= endport ; port++)
            {
                printf("try to connecting %s:%d...\n", inet_ntoa(addr.sin_addr), port);

                addr.sin_port = htons(port); 
                
                sockfd = socket(AF_INET, SOCK_STREAM, 0);     
       
                if (-1 == sockfd)
                {
                    printf("Err number: %d\n", errno);
                    perror("socket");
                    //exit( -1 );
                    continue;
                }

                ioctl(sockfd, FIONBIO, &non_blocking);
                int nRet = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));

                if (-1 == nRet)
                {         
                    if(errno != EINPROGRESS)
                    {
                        perror("connect");
                        continue;
                    }

                    FD_ZERO(&writefds);
                    FD_SET(sockfd, &writefds);
                    switch (select(sockfd + 1, &readfds, &writefds, NULL, &timeout)) 
                    {
                        case -1:
                            close(sockfd);
                            printf("select error\n"); 
                            perror("select");
                            break;
                        case 0:
                            close(sockfd); 
                            break;
                        default:
                            if (FD_ISSET(sockfd, &writefds)) //测试sock是否可读
                            {
                                printf("Connected: %s %d open\n", _ip, port);
                                close(sockfd);
                            }
                            break;
                    }
                }
                else if (0 == nRet)
                {
                    close(sockfd);
                    printf("Connected: %s %d open\n", _ip, port);
                }
            }           
        }
    }
    
    return 0;
}