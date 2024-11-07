#include "mySocket.h"

/**
 * 初始化服务器的socket
 * 返回值：
 *      socket套接字（>0 创建成功 =0 创建失败）
 * 参数：
 *      协议族：   AF_INET: IPV4  AF_UNIX: 域套接字
 *      type：    SOCK_STREAM: tcp协议    SOCK_DGRAM: UDP协议   
 *      port端口号 监听数量  
 */
int Mysocket::Sock_init_server(int domain , int type , int port , int listen_num)
{
    int sock_fd = socket(domain , type , 0);
    if(sock_fd == -1) {
        printf("create server socket failure\n");
        return 0;
    }

    struct sockaddr_in addr;
    memset(&addr , 0 , sizeof(addr));
    addr.sin_family = domain;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock_fd , (struct sockaddr*)&addr , sizeof(addr)) < 0) {
        printf("bind socket failure\n");
        return 0;
    }

    if(listen(sock_fd , listen_num) < 0) {
        printf("listen soclet failure\n");
        return 0;
    }

    return sock_fd;
}

/**
 * 客户端初始化套接字
 * 返回值：
 *      socket套接字（>0 创建成功 =0 创建失败）
 * 参数：
 *      协议族：   AF_INET: IPV4  AF_UNIX: 域套接字
 *      type：    SOCK_STREAM: tcp协议    SOCK_DGRAM: UDP协议
 *      port:     服务器端口号
 *      server_ip 服务器ip地址
 */
int Mysocket::Sock_init_client(int domain , int type , int port , char* server_ip)
{
    int sock_fd = socket(domain , type , 0);
    if(sock_fd == -1) {
        printf("create client socket failure\n");
        return 0;
    }

    struct sockaddr_in addr;
    memset(&addr , 0 , sizeof(addr));
    addr.sin_family = domain;
    addr.sin_port = htons(port);

    int ret = inet_pton(domain , server_ip , &addr.sin_addr.s_addr);
    if(ret == 0) {
        printf("ip address false\n");
    }
    else if(ret < 0) {
        printf("transform ip faliure\n");
    }

    if(connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("client connect server failure\n");
    }

    return sock_fd;
}




/**
 * 关闭套接字
 */
void Mysocket::Sock_close(int fd)
{
    close(fd);
}