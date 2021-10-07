
> 本地套接字概述

    (1) socket API 原本是为多台主机之间网络通信设计的，同时这种网络 socket 也支持单台主机上的进程间通信，（通过 loopback 地址 127.0.0.1）。
    (2) Unix本地套接字，其实就是一种专门用于本地（也就是单个主机上的）网络通信的一种方法。
    (3) 实际上，这种通信方式更类似于IPC（进程间通信）的方式，比如无名管道（pipe）、有名管道（mkfifo）。但是，Unix域套接字所提供的控制方式会更多一些,
    比如说TCP（字节流套接字）提供等待连接的功能，UDP（数据报套接字）提供帧同步功能，同时也是全双工的（既可读又可写）
    
    虽然网络socket也可用于同一台主机的进程间通讯（通过lo地址127.0.0.1），但是unix domain socket用于IPC更有效率：不需要经过网络协议栈，不需要打包拆包/计算校验和/维护信号和应答等。只是将应用层数据从一个进程拷贝到另一个进程。这是因为IPC机制本质上是可靠的通讯，而网络协议是不可靠的通讯
    
![image](https://user-images.githubusercontent.com/42632290/136314593-343c8fd5-a3ba-4123-bdea-98a2930b07d4.png)

- 本地套接字通信原理

      本地套接字用于实现本机进程间通信，有tcp和udp类似两种，本地套接字也是一种文件格式，和管道文件一样，是一种伪文件,
      存在于内核的缓冲区中，大小不变，一直是0. 套接字一定是成对出现的，有server套接字，就一定有client套接字，它是一种全双工通信方式，分别有读写缓冲区
      通信框图如下：

![image](https://user-images.githubusercontent.com/42632290/136176710-d978a431-a800-4f78-a180-601f7fe4c03c.png)

      （1）server与client在内核中分别对应有缓冲区
      （2）应用层通过绑定到磁盘伪文件获取读写句柄，进行进程之间的通信


- 相关头文件及数据类型

![image](https://user-images.githubusercontent.com/42632290/136178112-b7e9cd47-1fb8-421c-8459-3b327261496f.png)

```cpp
    头文件:  #include <sys/un.h>
             #define UNIX_PATH_MAX 108
struct sockaddr_un {
	__kernel_sa_family_t sun_family; 
	char sun_path[UNIX_PATH_MAX];
};
```

> 本地套接字创建流程

(1) TCP方式

```cpp
- server端
     
     1. 创建socket，socket(AF_LOCAL, SOCK_STREAM, 0)
     2. 服务器端绑定到套接字文件，bind()
     3. 监听，listen（）
     4. 等待连接请求，accept（）
     5. recv/send 进行通信

- client端

     1. 创建socket，socket(AF_LOCAL, SOCK_STREAM, 0)
     2. 客户端绑定到套接字文件，bind()
     3. 初始化server信息
     4. 连接服务器，connect()
     5. recv/send 进行通信
```

注意：

    当不再需要这个 Unix 域套接字时，应删除路径名对应的文件，使用unlink函数实现快速重入

- 代码示例

    见附件
    
(2) UDP 方式

```cpp
- server端

     1. 创建socket，socket(AF_LOCAL, SOCK_STREAM, 0)
     2. 服务器端绑定到套接字文件，bind()
     3. recv/send 进行通信

- client端

     1. 创建socket，socket(AF_LOCAL, SOCK_STREAM, 0)
     2. 客户端绑定到套接字文件，bind()
     3. 初始化server信息
     4. recv/send 进行通信

```

注意：

    当不再需要这个 Unix 域套接字时，应删除路径名对应的文件，使用unlink函数实现快速重入

- 代码示例

    见附件


参考：
（1）https://www.cnblogs.com/embedded-linux/p/5002947.html
（2）https://luhuadong.blog.csdn.net/article/details/78352230?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-17.no_search_link&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-17.no_search_link

