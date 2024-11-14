#include "myuart.h"

/**
 * 串口初始化
 * 参数：1.设备 2.波特率
 * 返回值: >0 文件标识符 表示成功 ， -1表示没有成功
 */
int UART_Init(char* device , int baud) {
    int fd;
    struct termios termios_p; // 设置串口的结构体
    int ret; // 用于正常查看返回值

    //O_NOCTTY : 不受当前进程控制信号的影响
    fd = open(device , O_NOCTTY | O_RDWR);
    if(fd == -1) {
        perror("open uart device failure\n");
        return -1;
    }

	memset(&termios_p, 0 ,sizeof(termios_p));
	termios_p.c_cflag |= CREAD;     //使能接收器的接收功能！必须配置的。
	termios_p.c_cflag |= CLOCAL;    //忽略调制解调器线路状态。
	termios_p.c_cflag |= CS8;        //设置八位数据位。
	
    //这里为了方便观看，所以写出来下面的配置，这里其实可以不写，因为我们已经先前清空结构体了。
	termios_p.c_cflag &=~ CSTOPB; //设置一位停止位。
	termios_p.c_cflag &=~ PARENB; //无奇偶校验位。
	termios_p.c_lflag &=~ ECHO ; //不回显
	
	// 设置超时和最小读取字符数。必须配置！！
    termios_p.c_cc[VTIME] = 1; // 超时时间为0.1s
    termios_p.c_cc[VMIN] = 128; // 至少读取128个字节（如果超时 ， 也可以少于128个字节）

    switch(baud) {
        case 115200:
            	cfsetispeed(&termios_p,B115200);  //设置接收波特率
				cfsetospeed(&termios_p,B115200);  //设置发送波特率
				break;
        case 9600:
            	cfsetispeed(&termios_p,B9600);  //设置接收波特率
				cfsetospeed(&termios_p,B9600);  //设置发送波特率
				break; 
        default:
                printf("no support this baud\n");
                break;
    }

    /**
     * TCIFLUSH:清空正读的数据，且不会读出
     * TCOFLUSH:清空正写入的数据，且不会发送到终端
     * TCIOFLUSH:清空所有正在发生的 I/O 数据.
     * fflush:用于stdio流 vs tcflush:用于终端设备
     */
    tcflush(fd , TCIOFLUSH);

    /**
     * TCSANOW:不等数据传输完毕，立即改变属性
     * TCSADRAIN:等所有数据传输完毕，再改变属性
     * TCSAFLUSH:清空输入输出缓冲区才改变属性
     */
    ret = tcsetattr(fd , TCSAFLUSH , &termios_p);
    if(ret < 0) {
        perror("bind termios failure\n");
        return -1;
    }

    return fd;
}


/**
 * 关闭串口
 */
void UART_Close(int fd) {
   close(fd);
}


/**
 * 串口发送程序
 * 参数：1.文件标识符 2. 消息
 * 返回值：-1 没有发送成功 >0 发送出去的字节数
 */
int UART_Send(int fd , char* data) {
    struct timeval timeout;
    fd_set write_set; // 写操作集合
    int ret; // select监听返回结果
    int n; // 写入的个数

    // 设置100ms的超时时间
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;

    FD_ZERO(&write_set);
    FD_SET(fd , &write_set);

    ret = select(fd + 1 , NULL , &write_set , NULL , &timeout);
    if(ret == -1) {
        perror("select failure\n");
        return -1;
    }
    else if(ret > 0 ) {
        if(FD_ISSET(fd , &write_set)) {
            n = write(fd , data , strlen(data)); // 用strlen能返回char* 类型的字节数
            return n;
        } 
    }
}


/**
 * 串口接收程序
 * 参数：1.文件标识符 2. 接收缓冲区 3. 缓冲区的大小
 * 返回值：-1 没有发送成功 >0 发送出去的字节数
 */
int UART_Receive(int fd, char *receive , size_t bufsize) {
    int num;
	int ret;

	fd_set readfds;
	struct timeval timeout;
    
    // 设置超时时间100ms
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;

    FD_ZERO(&readfds); //清空结构体，丢弃原来旧的数据。
    FD_SET(fd, &readfds);  //在 fd_set 结构中添加一个文件描述符。

    ret=select(fd+1,  &readfds, NULL, NULL, &timeout);
	if (ret == -1) {
		perror("select read failure\n");
		return -1;
	} 
	else if (ret > 0) {
		if(FD_ISSET(fd, &readfds)){
		    num = read(fd, receive, bufsize); // sizeof(receive) 只能获取指针本事的大小
            if(num > 0) {
                receive[num] = '\0';
            }
		    return num;
		}
	}
}
