/*
 * =====================================================================================
 *
 *       Filename:  socket.c
 *
 *    Description:  socket.c
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
#include <unistd.h>

int main(int argc, char** argv)
{
	int op;
    printf("samples:\n 1. poll\n 2. select\n");

    scanf("%d", op);

    switch(op)
    {
    	case 1:
    		execl("./poll", "poll");
    		break;
    	case 2:
    		execl("./select", "select");
    		break;
    }
    return 1;
}