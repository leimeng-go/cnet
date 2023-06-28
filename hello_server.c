#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void error_handling(char *message);

int main(int argc,char *argv[])
{
    int serv_sock;
    int clnt_sock;
    
    /*
      在c语言中，结构体（struct）是一种自定义的数据类型，允许你将不同类型的数据组合在一起
      形成一个新的复合类型。结构体可以包含多个成员，每个成员可以是不同的数据类型。
      结构体名称 变量名称
      struct Person {
         char name[20];
         int age;
      };

      struct Person person1;
    */
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[]="Hello world";

    if (argc!=2){
        printf("Usage: %s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1)
      error_handling("socket() error");

    /*
     memset() 函数用于将一块内存空间的值设置为特定的值。
     void *memset(void *ptr,int value,size_t num)
     函数参数说明如下:
     ptr: 指向要填充的内存块指针
     value: 要设置的值,以 int 类型表示

    */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    //需要将
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    //需要将端口号从主机字节序转变成网络字节序
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*) &serv_addr,sizeof(serv_addr))==-1)
      error_handling("bind() error");

    if(listen(serv_sock,5)==-1)
      error_handling("listen() error");

    clnt_addr_size=sizeof(clnt_addr);
    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if (clnt_sock==-1)
      error_handling("accept() error");

    write(clnt_sock,message,sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;  
}

void error_handling(char *message)
{
  fputs(message,stderr);
  fputc('\n',stderr);
  exit(1);
}
