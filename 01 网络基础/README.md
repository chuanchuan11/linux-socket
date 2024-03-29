> 01 什么是socket？
```
      (1)socket编程属于网络IO编程
      (2)是一种网络通信函数接口
      (3)一种特殊的文件描述符
```
![image](https://user-images.githubusercontent.com/42632290/131515668-62db4e51-67c4-4716-b775-e47ed995ee4a.png)

> 02 socket类型
```
      流式套接字(SOCK_STREAM)
          提供一个面向连接的，可靠的数据传输服务，数据无差错，无重复的发送且按顺序接受。内设置流量控制，避免数据流淹没慢的接受方。数据被看作是字节流，无长度限制。比如：通过流式套接字发送了顺序数据“1”  “2”，那么数据到达远程的时候也是“1”  “2”
          面向连接的SOCKET工作流程：
```
![image](https://user-images.githubusercontent.com/42632290/131518253-da6ed1aa-c1e3-4f46-bc79-80a708b88b35.png)
```

      报式套接字(SOCK_DGRAM)
          提供无连接服务。数据包以独立数据包的形式被发送，不提供无差错保证，数据可能丢失或重复，顺序发送，可能乱序接收。UDP协议传输效率比TCP高，为了保证数据的无差错传输，一般使用UDP传输数据的程序都有自己对数据确认的协议，进行数据校验。
          面向无连接的SOCKET工作流程：
```
![image](https://user-images.githubusercontent.com/42632290/131518940-cb2c8570-8852-422c-aa6b-bd0ce915441f.png)
```
      原始套接字(SOCK_RAW)
            可以对较低层次的协议，如：IP,ICMP直接访问， 主要用于一些协议的开发，一般程序涉及不到原始套接字   
```

> 03 socket数据结构

      (1)通用套接字地址结构名为sockaddr，定义在头文件<sys/socket.h>中，其结构定义如下
```cpp
      struct sockaddr{
            sa_family_t sa_family;  //2byte   address 族，AF-xxx   
            char sa_data[14];       //14byte  包含地址，端口和套接字数目等内容，混杂在一起
      };
```
      为了能够方便访问sockaddr中的数据，程序员建立了另外一个相似的结构：sockaddr_in

      (2)IPv4套接字地址结构通常也称为“网际套接字地址结构”，它的名字为sockaddr_in，定义在头文件<netinet/in.h>中，其结果定义如下:
```cpp
            struct sockaddr_in{
                  sa_family_t sin_family;  //2byte   协议族，必须设置为AF_INET
                  in_port_t sin_port;      //2byte   端口号，以网络字节序存储
                  struct in_addr sin_addr; //4byte   ip地址, 以网络字节序存储
                  char sin_zero[8];        //8byte   暂时没有使用，总是设置为0
            };
            
            struct in_addr{
                  unsigned long s_addr;  
            };
            
            注意：使用时候sin_zero一定要置0，一般填写通信结构体，然后强制转化为通用结构体
```
      (3)IPv6套接字地址结构的名字为sockaddr_in6，定义在头文件<netinet/in.h>中，其结构定义如下：
```cpp
            struct sockaddr_in6{
                  sa_family_t sin6_family;   //2byte   协议族，必须设置为AF_INET6
                  in_port_t sin6_port;       //2byte   端口号，以网络字节序存储
                  uint32_t sin6_flowinfo;    //4byte   窗口信息,流标签(24位 4位 4位）
                  struct in6_addr sin6_addr; //16byte  ip地址, 以网络字节序存储
            };

            struct in6_addr{
                  uint8_t s6_addr[16]; //存储网络字节序的128位IPv6地址
            };
```

> 04 主机字节序和网络字节序转换函数
       
       把给定系统所采用的字节序称为主机字节序，不同机器内部对字节的存储顺序不同，有大端和小端。为了避免不同类别主机之间在数据交换时由于对于字节序的不同而导致的差错，引入了网络字节序（采用大端模式)
```cpp
      #include <netinet/in.h>
      uint16_t htons(uint16_t hosts); //host to netword short, 2byte
      uint32_t htonl(uint32_t hosts); //host to netword long , 4byte
      uint16_t ntohs(uint16_t nets);  //netword to host short, 2byte
      uint32_t ntohl(uint32_t netl);  //netword to host long , 4byte
```
      注意：2byte的经常用于port的转换，4byte的经常用于ip的转换
> 05 IP地址转换函数
```cpp
      头文件  #include <arpa/inet.h>
      (1)int inet_aton(const char *strptr, struct in_addr *addrptr)
         描述：将strptr所指的字符串转换成32位的网络字节序二进制值，然后存于addrptr所指的结构体中
         特点：仅适用于IPV4
         返回值：成功返回非0值，失败返回0
            
      (2)char * inet_ntoa(struct in_addr inaddr)
         描述：将32位网络字节序二进制地址转换成点分十进制的字符串
         特点：仅适用于IPV4
         返回值：返回一个字符指针，指向inet_ntoa()中的static类型字符串。所以每次调用完inet_ntoa()都会改变最后一次调用inet_ntoa()函数时得到的结果，比如：
```
![1](https://user-images.githubusercontent.com/42632290/131684927-104bb278-5ddc-4f3b-b0f2-ca0326bd10c0.png)
``` cpp
      所以如果想把结果保存下来，再每次调用完inet_ntoa()后，使用strcpy()将结果拷贝出来
      (3)int_addr inet_addr(const char *strptr)
         描述：将strptr所指的字符串转换成32位的网络字节序二进制值（无符号长整形）
         特点：仅适用于IPV4，不能用于255.255.255.255的转换
         返回值：成功返回转换后的结果，失败返回-1
         注意：使用时进行返回值判断，如果返回-1，直接存储到int_addr中时地址变为255.255.255.255
      
      (4)int inet_pton(int af, const char *src, void *dst)
         描述：将IPV4/IPV6的地址转换为网络字节序IP
         特点：能正确处理255.255.255.255的转换问题
         参数：af    地址协议族(AF_INET或AF_INET6)
               src   点分形式的IP地址(主要指IPV4)
               dst   转换的结果给到dst
         返回值：成功返回1，输入不是有效的表达式返回0，出错返回-1
      
      (5)const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)
         描述：将IPV4/IPV6的网络字节序地址转化为本地的字符串形式的IP地址
         特点：能正确处理-1 的转换问题
         参数：af    地址协议族(AF_INET或AF_INET6)
               src   网络字节序IP
               dst   转换的结果给到dst
         返回值：成功返回一个指向dst的非空指针，失败返回NULL
```

> 06 字节操纵函数
```cpp
      #include <string.h>
      void bzero(void *dest, size_t len);          //字节置0 
      void bcopy(const void *src, void *dest, size_t len);       //拷贝
      int bcmp(const void *src, const void *dest, size_t len);   //比较

      void *memset(void *dest, int x, size_t len); //字节置0
      void *memcpy(void *dest, const void *src, size_t len);     //拷贝
      int memcmp(const void *str1, const void *str2, size_t len);//比较
```
