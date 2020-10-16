#include <func.h>
int tcpInit(int*,char*,char*);
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3);
    int socketFd;
    tcpInit(&socketFd,argv[1],argv[2]); //ip地址argv[1] 端口号argv[2]
    //init用来初始化一些传进来的参数
    //accept 接收远程计算机的连接请求
    int newFd = accept(socketFd,NULL,NULL); 
 //接受远程计算机的连接请求，建立起与客户机之间的通信连接。
   // fd_set rdset;
    //FD_ISSET(fd, &set); /*在调用select()函数后，用FD_ISSET来检测fd是否在set集合中，
    //当检测到fd在set中则返回真，否
    
    int ret;
    char buf[10];
    int epfd = epoll_create(1);
    struct epoll_event event,evs[2];  //event是注册时候用的
    event.events = EPOLLIN;  //监听读事件
    event.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);//最后一个参数是一个传入参数
    ERROR_CHECK(ret,-1,"epoll_ctl");
   
    event.data.fd = newFd;
    
    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,newFd,&event);//最后一个参数是一个传入参数
 //                                             event是需要监听的事                                                           
    ERROR_CHECK(ret,-1,"epoll_ctl");
    int readyFdCount,i;
    while(1)
    {//PS:另外说一下，STDIN_FILENO==0，STDOUT_FILENO=1，STDERR_FILENO==2
    //这也说明了为什么我们建立某些文件描述符从3开始 。
        readyFdCount = epoll_wait(epfd,evs,2,-1);
    //                                  
        for(i=0;i<readyFdCount;i++)
        {
            if(evs[i].events==EPOLLIN && evs[i].data.fd == STDIN_FILENO)
            {
                bzero(buf,sizeof(buf));
                ret = read(STDIN_FILENO,buf,sizeof(buf)-1);
                if(0==ret)
                {
                    printf("byebye\n");
                    break;
                }
                send(newFd,buf,strlen(buf)-1,0);//用新的套接字发送数据给指定的远端主机
            }
             if(evs[i].data.fd==newFd)   // newFd 如果是可读的
            { //接收
                bzero(buf,sizeof(buf));
                ret = recv(newFd,buf,sizeof(buf)-1,0); //用新的套接字来接收远端主机传来的数据
                                                    // 并把数据存到由参数 buf 指向的内存空间
                if(0==ret)
                {
                    printf("byebye\n");
                    break;
                }
                printf("%s\n",buf);
            }

        }
        
    }
    return 0;
}

