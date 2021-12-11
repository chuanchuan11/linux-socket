> 01 TCP 编程API

(1)socket函数

描述：获取socket文件描述符
```cpp
  #include <sys/types.h>
  #include <sys/socket.h>
  
  int socket(int domain, int type, int protocol)
 参数：
    domain：
         AF_INET           IPv4 internet protocol        ip(7)
         AF_INET6          IPv6 internet protocol        ipv6(7)
         AF_UNIX,AF_LOCAL  Local communication           unix(7)
         AF_NETLINK        Kernel user interface device  netlink(7)
         AF_PACKET         Low level packet interface    packet(7)
  
    type:
         SOCK_STREAM       流式套接字，唯一对应TCP
         SOCK_DGRAM        数据报套接字，唯一对应UDP
         SOCK_RAW          原始套接字
         
         SOCK_NONBLOCK     非阻塞，一般与上边三个type组合使用，默认为阻塞
  
    protocol:              一般为0，原始套接字编程时需填充
  
return：
        成功：返回文件描述符
        失败：返回-1， errno错误码被设置，具体参考man
```

(2)bind函数

描述：指定套接字使用的ip和port
```cpp
  #include <sys/types.h>
  #include <sys/socket.h>
  
  int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
  
参数：
  socket:   通过socket()函数拿到的fd
  addr:     struct sockaddr的结构体变量的地址，包含地址信息：IP和Port
  addrlen:  地址信息长度

return:
  成功：返回0
  失败：返回-1，errno错误码被设置
  
注意：
  bind()可以在程序中自动获取电脑ip和端口
  xxx.sin_port = 0  //表示随机选择一个端口
  xxx.sin_addr.s_addr = INADDR_ANY //使用电脑ip
  
```

(3)listen函数

描述：等待客户端连接，进行系统侦听客户端连接请求
```cpp
  #include <sys/types.h>
  #include <sys/socket.h>
  
  int listen(int sockfd, int backlog)
  
参数：
    sockfd:  通过socket()函数获得的fd
    backlog: 同时允许几路客户端和服务器进行正在连接的过程(正在三次握手)
       详解：每个连入请求都要进入一个连入请求队列，等待listen程序调用accept函数来接受这个连接，当系统还没有调用accept函数的时候，如果有很多连接，那么本地能够等待的最大数目就是backlog的数值。一般推荐设置成5-10之间的数值
    
return:
    成功：返回0
    失败：返回-1，errno错误码被设置

```

(4)accept函数

描述：阻塞等待客户端连接请求
```cpp
  #include <sys/types.h>
  #include <sys/socket.h>
  
  int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
  
参数：
  sockfd:     正在listen()的套接字描述符fd，非0值
  addr:       存储远程连接过来的计算机信息，即远程客户端的IP和port
  addr_len:   获取的客户端信息的长度，在传递给accept()之前值为sizeof(struct sokcetaddr_in), accept()不会在addr中存储大于addr_len 长度的数据，如果accept()函数在addr中存储的数据量小于addr_len，则accept()会改变addr_len的值来反应这个情况
  
return:
  成功：返回新的套接字描述符newfd，代表已经建立的连接。并继续阻塞等待其他客户端连接其他的newfd
  失败：返回-1，errno错误码被设置

注意：
  (1)addr和addr_len可以为null，表示不关心客户端的信息
  
  (2)如果想获取一个单独的连接，可以在获取newfd之后，将原来的sockfd关掉(调用close()函数)，这样就可以阻止以后的连接
```

(5)connet()函数

描述：向服务端发送连接请求
```cpp
  #include <sys/types.h>
  #include <sys/socket.h>
  
  int connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen)
  
参数：
  sockfd:    通过socket()函数拿到的fd
  addr:      struct sockaddr的结构体变量地址,存储着要连接的远程计算机的ip和port信息
  addrlen:   要连接的远程计算机地址信息长度
  
返回值：
  成功：返回0
  失败：返回-1，errno错误码被设置
  
注意：
  客户端调用该函数时不同调用bind()函数，因为我们并不关心本地用什么端口来通讯，只关心要接连到的那台远程主机用的哪个端口。所以linux内核会自动为我们选择一个本地未使用的端口
  
```

(6)send()/write()函数

描述：通过连接的套接字流发送数据
```cpp
  #include <sys/types.h>
  #include <sys/socket.h>
      int send(int sockfd, const void *msg, int len, int flags)
      
  #include <unistd.h>
      int write(int fd, const void *msg, int len)
  
参数：
  sockfd:    已经建立连接的套接字描述符
  msg:       指向要发送的信息的地址
  len:       要发送的信息长度
  flags:     发送方式，一般设为0。填写0时send和write作用一样，阻塞式发送
             MSG_DONTWAIT    非阻塞方式发送
             MSG_OOB         用于发送TCP类型的带外数据
                             可通过man查看其他不同的发送方式
return:
  成功：返回实际发送的数据长度
  失败：返回-1，errno错误码被设置
  
注意：
  (1)send函数有最大发送数据长度限制，如果返回的值小于len的话，则需要再次调用发送剩下的数据
  (2)socket中一般使用send发送数据
```
(7)recv()和read()函数

描述：通过连接的套接字流接收数据
```cpp
  #include <sys/types.h>
  #include <sys/socket.h>
      int recv(int sockfd, void *buf, int len, int flags)
      
  #include <unistd.h>
      int read(int fd, void *buf, int len)
  
参数：
   sockfd:  要读取的套接字描述符
   buf:     指向存储数据的buffer
   len:     buffer的大小
   flag:    一般填写0，此时recv和read作用一样
            MSG_DONTWAIT    非阻塞接收
            MSG_OOB         用于发送TCP类型的带外数据
            MSG_PEEK        读过的数据不从队列中移除，每次都从头读数据
                            其他特殊标志参考man手册
return:
  成功：返回接收到的数据长度
  失败：返回-1，errno错误码被设置

注意：
  socket中一般使用recv接受数据
```

(8)sendto()和recvfrom()函数

描述：这两个函数是进行无连接的UDP通讯时使用
```cpp
  #include <sys/types.h>
  #include <socket.h>
  
  int sendto(in sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, int tolen)
  
参数：
  sockfd:    已经建立连接的套接字描述符
  msg:       要发送的信息地址
  len:       发送的信息长度
  flags:     一般为0，特殊标志参考man
  to:        指向sockaddr结构体的指针，存放远程主机的ip和port
  tolen:     sockaddr结构体的大小

return:
  成功：返回真正发送的字节数(与send一样，发送的字节数可能小于所传入的字节数，需判断再次发送剩余数据)
  失败：返回-1， errno错误码被设置
```
```cpp
  #include <sys/types.h>
  #include <socket.h>
  
  int recefrom(int sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from, int *fromlen)
  
参数：
  sockfd:    已经建立连接的套接字描述符
  buf:       存储数据的buffer
  len:       buffer的大小
  flags:     一般是0.特殊标志参考man
  from:      指向sockaddr结构体，其中存有源ip地址和port
  fromlen:   当函数返回时，存放sockaddr结构体的实际大小

return:
  成功：返回真实接收的数据大小
  失败：返回-1，errno错误码被设置
  
注意：
  (1)如果信息大的传入的buffer都放不下，那么附加信息将被扔掉。该调用可以立即返回，也可以阻塞等待，甚至可以设置超时等待，通过flags进行设置。
  
  (2)如果使用conncet函数连接了一个数据报套接字的服务器端，那么可以使用send和recv进行传输数据，但是不要认为此时用的是流式套接字，所使用的仍然是数据报套接字，只不过套接字在send和recv时候，自动帮助加上了目标地址和端口
  
```
(9)close()和shutdowm()函数

描述：关闭套接字描述符所代表的连接
```cpp
  #include <sys/socket.h>
    clse(sockfd)
    shutdown(sockfd, int how)

参数:
  sockfd:    要关闭的套接字描述符
  how:       0:表示不允许以后数据的接收操作
             1：表示不允许以后数据的发送操作
             2：与close一样，不允许以后的任何操作
返回：
  成功：返回0
  失败：返回-1,errno错误码被设置

注意：
  (1)如果不允许套接字在关闭后任何操作的话，使用标准关闭文件close。之后任何有关的套接字的读和写都会接受到一个错误
  (2)如果想要关闭之后进一步操作，则用shutdown
```

(9)setsockopt()和getsockpt()函数

描述：获取或者设置与某个套接字关联的选项。主要进行快速地址重用时使用：
      socket关闭之后并不会立即收回，而是要经历一个TIME_WAIT的阶段。此时对这个端口进行重新绑定就会出错。要想立即绑定，需要在bind之前先设置SO_REUSEADDR
或者在closesocket的时候，使用setsockopt设置SO_DONTLINGER。才会消除TIME_WAIT时间。
```cpp
  #include <sys/types.h>
  #include <sys/socket.h>
    
    int getsockopt(int sockfd, int level, int name, char *value, int *optlen)
    int setsockopt(int sockfd, int level, int name, char *value, int *optlen)

参数：
  sockfd:       必须是一个已经打开的套接字
  level:        选项所在的协议层
  optname:      需要访问的选项名
  optvalue:     对于getsockopt()指向返回选项值的缓冲。对于setsockopt()指向包含新选项值的缓冲
  optlen:       对于getsockopt()作为入口参数时，为选项值的最大长度。作为出口参数时，为选项值的实际长度；对于setsockopt()为现选项值的长度
  
return:
  成功：返回0
  失败：返回-1，errno被设置

参数详细说明：
level指定控制套接字的层次.可以取三种值:
  1)SOL_SOCKET:通用套接字选项.
  2)IPPROTO_IP:IP选项.
  3)IPPROTO_TCP:TCP选项.　
optname指定控制的方式(选项的名称),下面详细解释　
optval获得或者是设置套接字选项.根据选项名称的数据类型进行转换

optname　　　　　　　　说明　　　　　　　　　　　　　　　　　　数据类型
========================================================================
　　　　　　　　　　　　SOL_SOCKET
------------------------------------------------------------------------
SO_BROADCAST　　　　　　允许发送广播数据　　　　　　　　　　　　int
SO_DEBUG　　　　　　　　允许调试　　　　　　　　　　　　　　　　int
SO_DONTROUTE　　　　　　不查找路由　　　　　　　　　　　　　　　int
SO_ERROR　　　　　　　　获得套接字错误　　　　　　　　　　　　　int
SO_KEEPALIVE　　　　　　保持连接　　　　　　　　　　　　　　　　int
SO_LINGER　　　　　　　 延迟关闭连接　　　　　　　　　　　　　　struct linger
SO_OOBINLINE　　　　　　带外数据放入正常数据流　　　　　　　　　int
SO_RCVBUF　　　　　　　 接收缓冲区大小　　　　　　　　　　　　　int
SO_SNDBUF　　　　　　　 发送缓冲区大小                         int
SO_RCVLOWAT　　　　　　 接收缓冲区下限　　　　　　　　　　　　　int
SO_SNDLOWAT　　　　　　 发送缓冲区下限　　　　　　　　　　　　　int
SO_RCVTIMEO　　　　　　 接收超时　　　　　　　　　　　　　　　　struct timeval
SO_SNDTIMEO　　　　　　 发送超时　　　　　　　　　　　　　　　　struct timeval
SO_REUSERADDR　　　　　 允许重用本地地址和端口　　　　　　　　　int
SO_TYPE　　　　　　　　 获得套接字类型　　　　　　　　　　　　　int
SO_BSDCOMPAT　　　　　　与BSD系统兼容　　　　　　　　　　　　　 int
========================================================================
　　　　　　　　　　　　IPPROTO_IP
------------------------------------------------------------------------
IP_HDRINCL　　　　　　　在数据包中包含IP首部　　　　　　　　　　int
IP_OPTINOS　　　　　　　IP首部选项　　　　　　　　　　　　　　　int
IP_TOS　　　　　　　　　服务类型                               
IP_TTL　　　　　　　　　生存时间　　　　　　　　　　　　　　　　int
========================================================================
　　　　　　　　　　　　IPPRO_TCP
------------------------------------------------------------------------
TCP_MAXSEG　　　　　　　TCP最大数据段的大小　　　　　　　　　　 int
TCP_NODELAY　　　　　　 不使用Nagle算法　　　　　　　　　　　　 int
========================================================================
一些使用场景参考：
（1）http://blog.chinaunix.net/uid-31397058-id-5775816.html
（2）https://blog.csdn.net/lrh406317290/article/details/8729586
```

(10)getpeername()函数

  描述：获取一个已经连接上的套接字远程信息，告诉你在远程和你连接的究竟是谁，比如：ip和port
```cpp
  #include <sys/socket.h>
    int getpeername(int sockfd, struct sockaddr *addr, int *addrlen)
    
参数：
  sockdf:    想获取远程信息的那个套接字描述符
  addr:      指向struct sockaddr结构体的指针，存储着远程信息
  addrlen:   入参为sockaddr结构体的大小，出参为sockaddr中远程信息的大小

return:
  成功：返回0
  失败：返回-1，errno错误码被设置
  
```

(11)gethostname()函数

  描述：获取本地主机的信息，返回正在执行它的计算机的名字。返回的这个名字可以被gethostbyname()函数使用，由此可以得到本地主机的ip地址
```cpp
  #include <unistd.h>
    int gethostname(char *hostname, size_t size)
    
参数:
  hostname:    是一个指向字符数组的指针，当函数返回时候，里边的数据就是本地的主机名字
  size:        hostname指向数据的长度
  
return:
  成功：返回0
  失败：返回-1，errno错误码被设置
 
```

(12)gethostbyname()函数

  描述：用域名或主机名获取主机的完整信息
```cpp
    #include <netdb.h>
    #include <sys/socket.h>
      struct hostent *gethostbyname(const char *name);

参数：
    name:    域名或者主机名，如"www.google.cn" 或 主机名
return:
   struct hostent{
    char *h_name;      //official name
    char **h_aliases;  //alias list
    int  h_addrtype;   //host address type
    int  h_length;     //address lenght
    char **h_addr_list;  //address list
}
```

(13)gethostbyaddr()

  描述：根据ip地址获取主机完整信息
```cpp
  #include <netdb.h>
  #include <sys/socket.h>
    struct hostent *gethostbyaddr(const char * addr, socklen_t len, int family)

```
  
(14)getifaddr()

  描述：通过网络设备接口名获取网络接口的完整信息
