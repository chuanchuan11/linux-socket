> 01 TCP 工作流程

![image](https://user-images.githubusercontent.com/42632290/132089554-fa8b7d21-9d14-4823-b3fa-cbb2c46f37b6.png)


- 服务端实现的步骤如下：

  (1)socket()函数创建套接字 lfd

  (2)bind()函数将创建的套接字绑定到指定的地址结构

  (3)listen()函数设置套接字为监听模式，使服务器进入被动打开的状态

  (4)accept()函数接受客户的连接请求，建立连接，拿到新的cfd

  (5)send/recv函数与客户端的通信

  **注意：服务器端会维护两个fd，监听lfd和通信cfd**

- 客户端实现的步骤如下：

  (1)socket()函数创建套接字
  
  (2)connect()函数建立一个与TCP服务器的连接
  
  (3)send/recv函数与服务器进行数据通信
  
  (4)终止连接

> 02 简单Server与Client通讯练习

- server代码
  见附件
- client代码
  见附件
- 简述bind函数中使用通配地址的作用
```cpp
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htol(INADDR_ANY); //使用通配地址
    bind(sockfd, (struct sockaddr *)&server, sizeof(server));
```

> 03 简述TCP三次握手和数据传输过程



> 04 简述TCP四次挥手



> 05 TCP滑动窗口机制










> 06 简述使用TCP套接字基本函数编程的过程中，TCP链接的状态变化




