#include <func.h>

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd = socket(AF_INET,SOCK_DGRAM,0);  //IPv4  流式socket 用于UDP通信
    ERROR_CHECK(socketFd,-1,"socket");
    struct sockaddr_in serAddr;
    //初始化一些参数
    bzero(&serAddr,sizeof(serAddr));
    serAddr.sin_family = AF_INET;  
    serAddr.sin_port = htons(atoi(argv[2]));   //  host  to  net short
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret;
 //对于UDP 一旦bind  端口就激活了
    //bind用来绑定一个端口号和IP地址， 使套接口的与指定的端口号和IP地址相关联
    ret = bind(socketFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
 
    ERROR_CHECK(ret,-1,"bind");
    //使服务器的这个端口和 IP 处于监听状态，等待网络中某一客户机的连接请求。如果客户
    //端有连接请求，端口就会接受这个连接。
    struct sockaddr_in client;
    bzero(&client,sizeof(client));
    socklen_t fromLen = sizeof(struct sockaddr_in);
    char buf[128]={0};
    recvfrom(socketFd,buf,sizeof(buf),0,(struct sockaddr*)&client,&fromLen);
    //                              网络字节序 转 主机字节序
    printf("gets %s client ip=%s,port=%d\n",buf,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
    sendto(socketFd,"I hen niu",9,0,(struct sockaddr*)&client,sizeof(client));
    close(socketFd);

    return 0;
}

