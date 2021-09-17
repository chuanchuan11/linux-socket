> TCP状态转换

![image](https://user-images.githubusercontent.com/42632290/133098061-f85ea94b-aca7-4911-8d0f-981f9544e159.png)

```cpp
	1. 2MSL
		a. 等待时长
		b. 主动关闭连接的一方, 处于TIME_WAIT状态
		c. 有的地方: 2分钟, 一般时候是30s(MSL)
    
	2. 半关闭
		○ 如何理解?
			§ A给B发送是FIN(A调用了close函数), 但是B没有给A发送FIN(B没有调用close)
			§ A断开了与B的连接, B没有断开与A的连接
		○ 特点:
			§ A不能给B发送数据, A可以收B发送的数据
			§ B可以给A发送数据
		○ 函数: int shutdown(int sockfd, int how);
			§ sockfd: 要半关闭的一方对应的文件描述符
				□ 通信的文件描述符
			§ how:
				□ SHUT_RD        - 0 - 读
				□ SHUT_WR        - 1 - 写
				□ SHUT_RDWR      - 2 - 读写
        
  3. 查看网络相关状态信息
		○ 命令：netstat
		○ 参数：
          -a (all)显示所有选项，默认不显示LISTEN相关
		      -p 显示建立相关链接的程序名
		      -n 拒绝显示别名，能显示数字的全部转化成数字
		      -t (tcp)仅显示tcp相关选项
		      -u (udp)仅显示udp相关选项
          -l 仅列出有在 Listen (监听) 的服务状态
          
  4. 端口复用最常用的用途是:
	    - 防止服务器重启时之前绑定的端口还未释放
	    - 程序突然退出而系统没有释放端口
  设置方法:
	    int opt = 1;    
	    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt)); 
  注意事项:
      绑定之前设置端口复用的属性
```



> TCP高并发编程-IO多路复用

  - linux网络编程的5种IO模型：阻塞IO、非阻塞IO、多路复用IO、信号驱动IO以及异步IO
  （1）阻塞IO
      当用户线程发出IO请求之后，内核会去查看数据是否就绪，如果没有就绪就会等待数据就绪，而用户线程就会处于阻塞状态，用户线程交出CPU。当数据就绪之后，内核会将数据拷贝到用户线程，并返回结果给用户线程，用户线程才解除block状态
      
![image](https://user-images.githubusercontent.com/42632290/133789513-38813c7d-9c2e-4e45-af53-f7aaf4cf5b0f.png)

  （2）非阻塞IO
  当用户线程发起一个IO操作后，并不需要等待，而是马上就得到了一个结果。如果结果是一个error时，它就知道数据还没有准备好，于是它可以再次发送IO操作。一旦内核中的数据准备好了，并且又再次收到了用户线程的请求，那么它马上就将数据拷贝到了用户线程，然后返回。

![image](https://user-images.githubusercontent.com/42632290/133790623-e908a062-17db-440e-a0a0-52bc70ce11a4.png)

  在非阻塞IO模型中，用户线程需要不断地询问内核数据是否就绪，也就说非阻塞IO不会交出CPU，而会一直占用CPU
  对于非阻塞IO就有一个非常严重的问题，在while循环中需要不断地去询问内核数据是否就绪，这样会导致CPU占用率非常高，因此一般情况下很少使用while循环这种方式来读取数据

  （3）多路复用IO
  
  （4）信号驱动IO
  
  （5）异步IO


  - Selete


  - Poll

  - Epoll


