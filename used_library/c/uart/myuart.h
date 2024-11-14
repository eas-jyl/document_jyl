#ifndef __MYUART_H__
#define __MYUART_H__


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

int UART_Init(char* device , int baud);
void UART_Close(int fd);
int UART_Send(int fd , char* data);
int UART_Receive(int fd, char *receive , size_t bufsize);

#endif