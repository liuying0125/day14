#include <func.h>
int tcpInit(int*,char*,char*);
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd;
    //把socketFd也监控起来如果socketFd可读，就可以产生一个新的newFd
    //select也可以监控socketFd 任何一个newFd来信息了。。。。
    tcpInit(&socketFd,argv[1],argv[2]);//实现ip地址端口的初始化
    int newFd;
    ////接受远程计算机的连接请求，建立起与客户机之间的通信连接。
    fd_set rdset;
    fd_set needMoniterFd;  //始终记录要监控的描述符
    int ret;
    char buf[1024];

    FD_ZERO(&needMoniterFd);
    FD_SET(STDIN_FILENO,&needMoniterFd);
    FD_SET(socketFd,&needMoniterFd);
    while(1)
    {
       // 为了不改变原始的描述符
        memcpy(&rdset,&needMoniterFd,sizeof(fd_set));  //将&needMoniterFd地址的数值copy到rdset
        
        ret = select(14,&rdset,NULL,NULL,NULL);
        if(ret > 0)
        {
            if(FD_ISSET(socketFd,&rdset))
            {
                newFd = accept(socketFd,NULL,NULL);
                ERROR_CHECK(newFd,-1,"accept");
                FD_SET(newFd,&needMoniterFd); //将newFd 加入到 nnedMoniterFd中
            }

            if(FD_ISSET(STDIN_FILENO,&rdset))
            {//发送
                bzero(buf,sizeof(buf));
                ret = read(STDIN_FILENO,buf,sizeof(buf));
                if(0==ret)
                {
                    printf("byebye\n");
                    break;
                }
                send(newFd,buf,strlen(buf)-1,0);
            }
             if(FD_ISSET(newFd,&rdset))
            { //接收
                bzero(buf,sizeof(buf));
                ret = recv(newFd,buf,sizeof(buf),0); 
                if(0==ret)
                {
                    printf("byebye\n");
                    FD_CLR(newFd,&needMoniterFd);
                    close(newFd);
                }else
                {
                     printf("%s\n",buf);
                }
            }

        }
        
    }
    return 0;
}


//FD_ISSET(int fd, fd_set *fdset);     //检测fd是否在set集合中，不在则返回0

