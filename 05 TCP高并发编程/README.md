> TCP状态转换

![image](https://user-images.githubusercontent.com/42632290/133098061-f85ea94b-aca7-4911-8d0f-981f9544e159.png)

    注意：当调用了close函数后，会发送FIN包

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


> linux网络编程的5种IO模型

  - linux网络编程的5种IO模型：阻塞IO、非阻塞IO、多路复用IO、信号驱动IO以及异步IO
  
  （1）阻塞IO

    当用户线程发出IO请求之后，内核会去查看数据是否就绪，如果没有就绪就会等待数据就绪，而用户线程就会处于阻塞状态，用户线程交出CPU。当数据就绪之后，内核会将数据拷贝到用户线程，并返回结果给用户线程，用户线程才解除block状态
      
![image](https://user-images.githubusercontent.com/42632290/133789513-38813c7d-9c2e-4e45-af53-f7aaf4cf5b0f.png)

    缺点：同一时刻只能处理一个操作, 效率低

  （2）非阻塞IO
  
    当用户线程发起一个IO操作后，并不需要等待，而是马上就得到了一个结果。如果结果是一个error时，它就知道数据还没有准备好，于是它可以再次发送IO操作。一旦内核中的数据准备好了，并且又再次收到了用户线程的请求，那么它马上就将数据拷贝到了用户线程，然后返回

![image](https://user-images.githubusercontent.com/42632290/133790623-e908a062-17db-440e-a0a0-52bc70ce11a4.png)

    在非阻塞IO模型中，用户线程需要不断地询问内核数据是否就绪，也就说非阻塞IO不会交出CPU，而会一直占用CPU
    对于非阻塞IO就有一个非常严重的问题，在while循环中需要不断地去询问内核数据是否就绪，这样会导致CPU占用率非常高，因此一般情况下很少使用while循环这种方式来读取数据

  （3）多路复用IO
  
     所谓I/O多路复用机制，就是说通过一种机制，可以监视多个描述符，一旦某个描述符就绪（一般是读就绪或者写就绪），能够通知程序进行相应的读写操作。这种机制的使用需要额外的功能来配合： select、poll、epoll
    - select、poll，epoll本质上都是同步I/O，因为他们都需要在读写事件就绪后自己负责进行读写，也就是说这个读写过程是阻塞的。
    - select时间复杂度O(n)，它仅仅知道了，有I/O事件发生了，却并不知道是哪几个流（可能有一个，多个，甚至全部），我们只能无差别轮询所有流，找出能读出数据，或者写入数据的流，对他们进行操作。所以select具有O(n)的无差别轮询复杂度，同时处理的流越多，无差别轮询时间就越长。
    - poll（翻译：轮询）时间复杂度O(n)，poll本质上和select没有区别，它将用户传入的数组拷贝到内核空间，然后查询每个fd对应的设备状态， 但是它没有最大连接数的限制，原因是它是基于链表来存储的.
    - epoll时间复杂度O(1)，epoll可以理解为event poll，不同于忙轮询和无差别轮询，epoll会把哪个流发生了怎样的I/O事件通知我们。所以我们说epoll实际上是事件驱动（每个事件关联上fd）的，此时我们对这些流的操作都是有意义的。（复杂度降低到了O(1)）。  
  
![image](https://user-images.githubusercontent.com/42632290/133792988-1ea389d4-1d6d-4568-b288-8887cb34b775.png)
  
    在多路复用IO模型中，会有一个内核线程不断去轮询多个socket的状态，只有当真正读写事件发生时，才真正调用实际的IO读写操作。因为在多路复用IO模型中，只需要使用一个线程就可以管理多个socket，系统不需要建立新的进程或者线程，也不必维护这些线程和进程，并且只有在真正有读写事件进行时，才会使用IO资源，所以它大大减少了资源占用。
  
  （4）信号驱动IO
  
    在信号驱动IO模型中，当用户线程发起一个IO请求操作，会给对应的socket注册一个信号函数，然后用户线程会继续执行，当内核数据就绪时会发送一个信号给用户线程，用户线程接收到信号之后，便在信号函数中调用IO读写操作来进行实际的IO请求操作。这个一般用于UDP中，对TCP套接口几乎是没用的，原因是该信号产生得过于频繁，并且该信号的出现并没有告诉我们发生了什么事情。
  
  ![image](https://user-images.githubusercontent.com/42632290/133793109-4f3b8d63-0fb2-42e0-83d1-70e8aa272297.png)

    在UDP上，SIGIO信号会在下面两个事件的时候产生：

     1 数据报到达套接字

     2 套接字上发生错误

     因此我们很容易判断SIGIO出现的时候，如果不是发生错误，那么就是有数据报到达了。

    而在TCP上，由于TCP是双工的，它的信号产生过于频繁，并且信号的出现几乎没有告诉我们发生了什么事情。因此对于TCP套接字，SIGIO信号是没有什么使用的。

  （5）异步IO

    前面四种IO模型实际上都属于同步IO，只有最后一种是真正的异步IO，因为无论是多路复用IO还是信号驱动模型，IO操作的第2个阶段都会引起用户线程阻塞，也就是内核进行数据拷贝的过程都会让用户线程阻塞

![image](https://user-images.githubusercontent.com/42632290/133794545-213b4c41-b74a-4742-9fb7-829074050aa7.png)

    异步IO模型是比较理想的IO模型，在异步IO模型中，当用户线程发起read操作之后，立刻就可以开始去做其它的事。而另一方面，从内核的角度，当它收到一个asynchronous read之后，它会立刻返回，说明read请求已经成功发起了，因此不会对用户线程产生任何block。然后内核会等待数据准备完成，将数据拷贝到用户内存，当这一切都完成之后，kernel会给用户进程发送一个signal，告诉它read操作完成了。
    
    也就说用户线程完全不需要关心实际的整个IO操作是如何进行的，只需要先发起一个请求，当接收内核返回的成功信号时表示IO操作已经完成，可以直接去使用数据了。

    也就说在异步IO模型中，IO操作的两个阶段都不会阻塞用户线程，这两个阶段都是由内核自动完成，然后发送一个信号告知用户线程操作已完成。用户线程中不需要再次调用IO函数进行具体的读写

参考：
（1）https://www.cnblogs.com/liushui-sky/p/12917347.html


> TCP高并发编程-IO多路复用详解

  - 什么是IO多路复用

     ○ 先构造一张有关文件描述符的列表, 将要监听的文件描述符添加到该表中
     
     ○ 然后调用一个函数,监听该表中的文件描述符,直到这些描述符表中的一个进行I/O操作时，该函数才返回。
     
         § 该函数为阻塞函数
	   
         § 函数对文件描述符的检测操作是由内核完成的。
	   
     ○ 在返回时，它告诉进程有多少(哪些)描述符要进行I/O操作
	
  - 多路复用基本常识

    （1）linux中每个进程默认情况下，最多可以打开1024个文件描述符
    
    （2）文件描述符为非负数
    
    （3）从最小可用的数字来分配
    
    （4）每个进程启动时默认打开0，1，2三个文件描述符

   - Selete实现
 
    （1）函数原型
    
```cpp
    #include <sys/select.h>
    #include <sys/time.h>

    int select(int maxfd,fd_set *readset,fd_set *writeset,fd_set *exceptset,const struct timeval *timeout)
参数：
    maxfd     指定待测试的描述符个数，它的值是待测试的最大描述符加1
    readfds   指定让内核测试读条件的描述符,传入NULL值，表示不关心任何文件的读变化
    writefds  指定让内核测试写条件的描述符,传入NULL值，表示不关心任何文件的写变化
    exceptfds 指定让内核测试异常条件的描述符
    timeout   参数设置为NULL表示永久阻塞，当检测到fd变化的时候返回
    
返回值：
    就绪描述符的数目，超时返回0，出错返回-1
```

    (2)文件描述符操作函数
    
```cpp
  
    文件描述符集类型: 
        fd_set  rdset;
	
    文件描述符操作函数:
	- 全部清空
		○  void FD_ZERO(fd_set *set);
	- 从集合中删除某一项
		○  void FD_CLR(int fd, fd_set *set);
	- 将某个文件描述符添加到集合
		○  void FD_SET(int fd, fd_set *set);
	- 判断某个文件描述符是否在集合中
                ○  int  FD_ISSET(int fd, fd_set *set);  
```
    （3）工作机制
    
![image](https://user-images.githubusercontent.com/42632290/133985013-2f090d9f-33bc-42e3-8259-61e98a3d8b21.png)
    
     (4) select优缺点

	- 优点:
		○ 跨平台
	- 缺点:
		○ 每次调用select，都需要把fd集合从用户态拷贝到内核态，这个开销在fd很多时会很大
		○ 同时每次调用select都需要在内核遍历传递进来的所有fd，这个开销在fd很多时也很大
                ○ select支持的文件描述符数量太小了，默认是1024
 
    （5）代码练习
 
         参考附件


  - Poll实现

    (1)函数原型
    
```cpp
    #include <poll.h>

    int poll ( struct pollfd * fds, unsigned int nfds, int timeout);
    
 参数：
     fds:       要监视的文件描述符结构体数组地址
		struct pollfd{
			int fd;			//文件描述符
			short events;	        //等待的事件
			short revents;	        //实际发生的事件
		};
		
    nfds:       数组的最大长度, 数组中最后一个使用的元素下标+1
                内核会轮询检测fd数组的每个文件描述符
		
    timeout:    
                -1: 永久阻塞
		 0: 调用完成立即返回
                >0: 等待的时长毫秒
返回值：
    io发生变化的文件描述符的个数
    
注意：
    每一个pollfd结构体指定了一个被监视的文件描述符，可以传递多个结构体，指示poll()监视多个文件描述符。每个结构体的events域是监视该文件描述符的事件掩码，由用户来设置这个域。
revents域是文件描述符的操作结果事件掩码，内核在调用返回时设置这个域

```

常见event和revent值：

![image](https://user-images.githubusercontent.com/42632290/134358557-9f2e8201-d11c-49b4-a477-05c14d45ddee.png)

    
    (2)优缺点
    
        优点：
    
	    （a）poll的机制与select类似，与select在本质上没有多大差别，管理多个描述符也是进行轮询，根据描述符的状态进行处理
            （b）poll在应付大数目的文件描述符的时候速度更快，相比于select
            （c）它没有最大文件描述符数量的限制，原因是它是基于链表来存储的
            （d）在调用函数时，只需要对参数进行一次设置就好了
	
        缺点：
   
	    （a）大量的fd的数组被整体复制于用户态和内核地址空间之间，，而不论这些文件描述符是否就绪
            （b）与select一样，poll返回后，需要轮询pollfd来获取就绪的描述符，这样会使性能下降
            （c）同时连接的大量客户端在一时刻可能只有很少的就绪状态，因此随着监视的描述符数量的增长，其效率也会线性下降
    
    (3)代码练习

        参考附件


  - Epoll实现(重要)

         epoll是Linux下多路复用IO接口select/poll的增强版本，它能显著减少程序在大量并发连接中只有少量活跃的情况下的系统CPU利用率，因为它不会复用文件描述符集合来传递结果而迫使开发者每次等待事件之前都必须重新准备要被侦听的文件描述符集合，另一点原因就是获取事件的时候，它无须遍历整个被侦听的描述符集，只要遍历那些被内核IO事件异步唤醒而加入Ready队列的描述符集合就行了。epoll除了提供select/poll 那种IO事件的电平触发（Level Triggered）外，还提供了边沿触发（Edge Triggered），这就使得用户空间程序有可能缓存IO状态，减少epoll_wait/epoll_pwait的调用，提高应用程序效率。

    (1)函数原型
    
```cpp
    #include <sys/epoll.h>
        int epoll_create(int size);
	
描述：
    创建一个epoll对象(实际上它也是一个文件描述符)，用于添加或删除指定的连接
    
参数：
    size：想关注的文件描述符数量（用于在内核申请一片空间，用来存放你想关注的socket fd上是否发生以及发生了什么事件。）

返回值：
    成功：返回一个epoll文件描述符
    失败：返回-1。

注意： 
    epoll会占用一个fd值，在linux下如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在使用完epoll后，必须调用close()关闭，否则可能导致fd被耗尽。

```
  
```cpp
    #include <sys/epoll.h>
        int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);   

/* 有关的结构体 */
typedef union epoll_data {
    void *ptr;
    int fd;
    __uint32_t u32;
    __uint64_t u64;
} epoll_data_t;

struct epoll_event {
    __uint32_t events; /* Epoll events */
    epoll_data_t data; /* User data variable */
};

描述：
    控制epoll事件，可以是注册、修改或删除一个fd
    
参数：
    epfd： epoll_create 返回的对象
    op：   操作类型，取值如下
           EPOLL_CTL_ADD：注册新的fd到epfd中
           EPOLL_CTL_MOD：修改已经注册的fd的监听事件
           EPOLL_CTL_DEL：从epfd中删除一个fd
    fd：   需要监听的fd
    event：监听的事件
           events：取值如下
                   EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
                   EPOLLOUT：表示对应的文件描述符可以写；
                   EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
                   EPOLLERR：表示对应的文件描述符发生错误；
                   EPOLLHUP：表示对应的文件描述符被挂断；
                   EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。缺省是水平触发(Level Triggered)。
                   EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里。
            data：用户数据，在TCP中一般传递我们需要监听的fd
	    
返回值：成功返回0，失败返回-1

```

```cpp
    #include <sys/epoll.h>
        int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);  

/* 有关的结构体 */
typedef union epoll_data {
    void *ptr;
    int fd;
    __uint32_t u32;
    __uint64_t u64;
} epoll_data_t;

struct epoll_event {
    __uint32_t events; /* Epoll events */
    epoll_data_t data; /* User data variable */
};

描述：
    等待事件的产生，类似于select()调用
    
    epoll_wait运行的原理是：等侍注册在epfd上的socket fd的事件的发生，如果发生则将发生的sokct fd和事件类型放入到events数组中。并且将注册在epfd上的socket fd的事件类型给清空，所以如果下一个循环你还要关注这个socket fd的话，则需要epoll_ctl(epfd,EPOLL_CTL_MOD,listenfd,&ev)来重新设置socket fd的事件类型。这时不用EPOLL_CTL_ADD,因为socket fd并未清空，只是事件类型清空。这一步非常重要。

参数：
    epfd：epoll_create 返回的对象

    events：用来从内核得到所有的读写事件（从内核返回给用户），

    maxevents：告诉内核需要监听的所有的socket的句柄数（从用户传给内核），值不能大于创建epoll_create()时的size。

    timeout：超时时间（毫秒，0会立即返回，-1永久等待）。

返回值：

    成功返回需要处理的事件数目，若已超时则返回0；失败返回-1

```
    
    (2)epoll 两种触发模式
    
    水平触发模式：
    
        LT（level-triggered）是缺省的工作方式, 并且同时支持block和no-block socket。内核告诉你一个文件描述符是否就绪了，然后你可以对这个就绪的fd进行IO操作。如果你不作任何操作，内核还是会继续通知你的
    
    理解：
        a) 只要fd对应的缓冲区中有数据，就进行通知, 即epoll_wait会返回
	
	b) epoll_wait返回的次数与发送数据的次数没有关系

    边沿触发模式：
    
        ET （edge-triggered）是高速工作方式，在这种模式下, 只支持no-block socket. 当描述符从未就绪变为就绪时，然后它会假设你知道文件描述符已经就绪，并且不会再为那个文件描述符发送更多的就绪通知，直到你做了某些操作导致那个文件描述符不再为就绪状态了
    
    理解：

        a) client给server发送数据，发送一次数据，server的epoll_wait就返回一次，不在乎数据是否读完
	
	b) 如果数据读不完，如何全部读出来？
	   
	   1）使用while(recv()>0) 能够全部读出数据
	       
	       问题：读完之后，recv函数会阻塞
	       
	   2）解决阻塞问题
	   
	       设置非阻塞fd
	       
	c) 边沿非阻塞触发 - 解决阻塞问题
    
            1）open函数使用
	    
	        设置flags，O_WDRW | O_NONBLOCK 
		
		缺点：只适应于终端文件的操作，比如：/dev/tty
		
	    2）fcntl函数使用
	    
	        作用：将fd属性设置为非阻塞
		
```cpp
    #include <fcntl.h>
	int flag = fcntl(fd, F_GETFL)
        flag |= O_NONBLOCK
	fcntl(fd, F_SETFL, flag)
	
将缓冲区空间数据都读读出：
    
    while(recv()>0)
    {}
    
注意：

    (1) 当缓冲区数据读完之后，会强制读空的数据缓冲区，此时返回为-1，并设置errno=EAGAIN [可以判断此值，进入下一次等待，如果不是此值，则为错误发生，进行其他处理即可]
    (2) 当客户端断开连接，返回0
    
    示例：
    if(len = -1)
    {
        if(errno = EAGAIN)
	{
	    //缓冲区读完
	}
	else
	{
	    //读取出错
	}
    }
    else if(len = 0)
    {
        //客户端断开连接
    }
    else
    {
        //接收到数据
    }

```
	
    (3)代码练习








