#include <func.h>

int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd = socket(AF_INET,SOCK_STREAM,0);
            //      AF_INET ipv4协议  tcp：SOCK_STREAM  后面socket所使用的传输协议编号，通常为0
    ERROR_CHECK(socketFd,-1,"socket");
    
    struct sockaddr_in serAddr;
   
    bzero(&serAddr,sizeof(serAddr));  //清零是必须的
   
    serAddr.sin_family = AF_INET;  
    serAddr.sin_port = htons(atoi(argv[2])); //atoi函数将字符串转换为整型数
                    //htons函数  将主机的  无符号短整型  转换成网络字节
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret;
    ret = connect(socketFd,(struct sockaddr*)&serAddr,sizeof(serAddr)); 
    ERROR_CHECK(ret,-1,"connect");
    fd_set rdset;
    char buf[1024];
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(socketFd,&rdset);
        ret = select(socketFd+1,&rdset,NULL,NULL,NULL);
        if(ret > 0)
        {
            if(FD_ISSET(STDIN_FILENO,&rdset))
            {//发送
                bzero(buf,sizeof(buf));
                ret = read(STDIN_FILENO,buf,sizeof(buf));
                if(ret == 0)
                {
                    printf("byebye\n");
                    break;
                }
                send(socketFd,buf,strlen(buf)-1,0);
            }
             if(FD_ISSET(socketFd,&rdset))
            { //接收
                bzero(buf,sizeof(buf));
                ret = recv(socketFd,buf,sizeof(buf),0); 
                if(ret == 0)
                {
                    printf("byebye\n");
                    break;
                }
                printf("%s\n",buf);
            }

        }
        
    }
    close(socketFd);
}
