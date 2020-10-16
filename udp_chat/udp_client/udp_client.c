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

    char buf[128]={0};
    //发送一个字节
    sendto(socketFd,"1",1,0,(struct sockaddr*)&serAddr,sizeof(struct sockaddr));
    
   // recvfrom(socketFd,buf,sizeof(buf),0,NULL,NULL);
   // printf("client gets %s\n",buf);
    fd_set rdset;
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(socketFd,&rdset);
        select(socketFd+1,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&rdset))
        {//键盘输入流
            bzero(buf,sizeof(buf));
            read(STDIN_FILENO,buf,sizeof(buf));
            sendto(socketFd,buf,strlen(buf)-1,0,(struct sockaddr*)&serAddr,sizeof(serAddr));
        }
        if(FD_ISSET(socketFd,&rdset))
        {
            bzero(buf,sizeof(buf));
            recvfrom(socketFd,buf,sizeof(buf),0,NULL,NULL);
            printf("%s\n",buf);
        }
    }

    close(socketFd);

    return 0;
}

