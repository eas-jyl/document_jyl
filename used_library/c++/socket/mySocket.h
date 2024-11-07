#ifndef __Mysocket_H__
#define __Mysocket_H__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>

class Mysocket
{
public:
    //Mysocket();
    //~Mysocket();

    int Sock_init_server(int domain , int type , int port , int listen_num);
    int Sock_init_client(int domain , int type , int port , char* server_ip);
    void Sock_close(int fd);

private:
    
};


#endif