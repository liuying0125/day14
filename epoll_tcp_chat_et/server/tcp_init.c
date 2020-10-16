#include <func.h>
//需要传输描述符出去  就是*sfd
//
int tcpInit(int *sfd,char*ip,char* port)
{
    int socketFd = socket(AF_INET,SOCK_STREAM,0);  //IPv4  流式socket 用于TCP通信
    ERROR_CHECK(socketFd,-1,"socket");
    struct sockaddr_in serAddr;
    //初始化一些参数
    bzero(&serAddr,sizeof(serAddr));
    serAddr.sin_family = AF_INET;  
    serAddr.sin_port = htons(atoi(port));   //  host  to  net short
    serAddr.sin_addr.s_addr = inet_addr(ip);
    int ret;
    //bind用来绑定一个端口号和IP地址， 使套接口的与指定的端口号和IP地址相关联
    ret = bind(socketFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    
    ERROR_CHECK(ret,-1,"bind");
    //使服务器的这个端口和 IP 处于监听状态，等待网络中某一客户机的连接请求。如果客户
    //端有连接请求，端口就会接受这个连接。
    listen(socketFd,10);//10指能够同时处理的最大连接要求 通常为10或者5
    *sfd = socketFd;  //传输出去
    return 0;
    //成功返回0  失败返回-1
}
/*struct sockaddr_in 
{
    short int sin_family; //地址族
    unsigned short int sin_port; //端口号
    struct in_addr sin_addr; //IP 地址
    unsigned char sin_zero[8]; //填充 0 以保持与 struct sockaddr 同样大小
};*/
