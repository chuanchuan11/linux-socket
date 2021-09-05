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
  **绑定时：使用通配地址，自动获取当前主机的ip，方便代码移植到任意电脑上都可直接使用**
  
> 03 简述TCP三次握手和数据传输过程

![image](https://user-images.githubusercontent.com/42632290/132097784-0d31a86f-6a36-42f4-9234-337589069ef6.png)

- TCP三次握手过程

  **(1)其中mss为告诉对方，一次传输能接收的最大数据长度(bytes)**
![image](https://user-images.githubusercontent.com/42632290/132098289-49d0ef2f-1ee1-4577-9dd9-87add2aa5bda.png)

  **(2)三次握手状态变化参考：https://www.cnblogs.com/onesea/p/13053697.html**

  **(3)为什么需要三次握手**
```cpp
    我们假设client发出的第一个连接请求报文段并没有丢失，而是在某个网络结点长时间的滞留了，以致延误到连接释放以后的某个时间才到达server。
本来这是一个早已失效的报文段。但server收到此失效的连接请求报文段后，就误认为是client再次发出的一个新的连接请求。于是就向client发出确认报文段，同意建立连接。

    假设不采用“三次握手”，那么只要server发出确认，新的连接就建立了。由于现在client并没有发出建立连接的请求，因此不会理睬server的确认，也不会向server发送数据。
但server却以为新的运输连接已经建立，并一直等待client发来数据。这样，server的很多资源就白白浪费掉了。

    所以，采用“三次握手”的办法可以防止上述现象发生。例如刚才那种情况，client不会向server的确认发出确认。server由于收不到确认，就知道client并没有要求建立连接。
```


- TCP数据传输过程

  **(1)ACK确认序号为，对方的seq号+成功接收到的字节数【表示接收到了多少数据】**

  **(2)seq序号为，对方回复的ACK确认序号【表示此次数据从几号位置开始的】**
![image](https://user-images.githubusercontent.com/42632290/132098912-f52527f7-9b86-4334-9dac-afbad54dd8b5.png)



- TCP四次挥手过程

![image](https://user-images.githubusercontent.com/42632290/132113319-e405f053-57d3-4654-8a66-63d8b050135b.png)


  **(1)为什么建立连接需三次握手，断开连接要四次挥手**
```cpp
    建立连接时因为当Server端收到Client端的SYN连接请求报文后，可以直接发送SYN+ACK报文。其中ACK报文是用来应答的，SYN报文是用来同步的。
所以建立连接只需要三次握手。

    由于TCP协议是一种面向连接的、可靠的、基于字节流的运输层通信协议，TCP是全双工模式。
    这就意味着，关闭连接时，当Client端发出FIN报文段时，只是表示Client端告诉Server端数据已经发送完毕了。当Server端收到FIN报文并返回ACK报文段，
表示它已经知道Client端没有数据发送了，但是Server端还是可以发送数据到Client端的，所以Server很可能并不会立即关闭SOCKET，直到Server端把数据也发送完毕。
当Server端也发送了FIN报文段时，这个时候就表示Server端也没有数据要发送了，就会告诉Client端，我也没有数据要发送了，之后彼此就会愉快的中断这次TCP连接
```

  **(2)为什么要等待2MSL的时间，client端才关闭**
```cpp
    MSL：报文段最大生存时间，它是任何报文段被丢弃前在网络内的最长时间。
有以下两个原因：

    第一点：保证TCP协议的全双工连接能够可靠关闭：
        由于IP协议的不可靠性或者是其它网络原因，导致了Server端没有收到Client端的ACK报文，那么Server端就会在超时之后重新发送FIN，
     如果此时Client端的连接已经关闭处于CLOESD状态，那么重发的FIN就找不到对应的连接了，从而导致连接错乱，所以，Client端发送完最后的ACK不能直接进入CLOSED状态，
     而要保持TIME_WAIT，没有再次收到FIN，确保对方已经收到ACK，最后正确关闭连接。
     
    第二点：保证这次连接的重复数据段从网络中消失
      如果Client端发送最后的ACK直接进入CLOSED状态，然后又再向Server端发起一个新连接，这时不能保证新连接的与刚关闭的连接的端口号是不同的，
    也就是新连接和老连接的端口号可能一样了，那么就可能出现问题：如果前一次的连接某些数据滞留在网络中，这些延迟数据在建立新连接后到达Client端，
    由于新老连接的端口号和IP都一样，TCP协议就认为延迟数据是属于新连接的，新连接就会接收到脏数据，这样就会导致数据包混乱。所以TCP连接需要在TIME_WAIT状态等待2倍MSL，
    才能保证本次连接的所有数据在网络中消失
```

  **参考：https://www.cnblogs.com/onesea/p/13053697.html**
  
  
> 04 TCP滑动窗口机制

![image](https://user-images.githubusercontent.com/42632290/132114108-882615ff-c79e-4a3d-90e5-301a5e69cee4.png)

**(01)滑动窗口原理**
    
  如果每次传输数据都只能发送一个MSS，就需要等待接收方的ACK，这显然会极大的影响传输的速率。
  
  在TCP中，滑动窗口是为了实现流量控制。该协议允许发送方在停止并等待接收确认报文前可以连续发送多个分组。由于发送方不必每发一个分组就停下来等待确认，因此该协议可以加速数据的传输。只有在接收窗口向前滑动时（与此同时也发送了确认），发送窗口才有可能向前滑动。收发两端的窗口按照以上规律不断地向前滑动，因此这种协议又称为滑动窗口协议。
     
![image](https://user-images.githubusercontent.com/42632290/132114298-c19b86e4-a5d5-4916-abb9-375940126e5b.png)

  所以TCP并不是每一个报文段都会回复ACK的，可能会对两个报文段发送一个ACK，也可能会对多个报文段发送1个ACK【累计ACK】，比如说发送方有1/2/3 3个报文段，先发送了2,3 两个报文段，但是接收方期望收到1报文段，这个时候2,3报文段就只能放在缓存中等待报文1的空洞被填上，如果报文1，一直不来，报文2/3也将被丢弃，如果报文1来了，那么会发送一个ACK对这3个报文进行一次确认

  
**参考：**
**（1）https://www.cnblogs.com/hujinshui/p/10472671.html**
**（2）https://www.pianshen.com/article/3251546981/**
**（3）https://zhuanlan.zhihu.com/p/101134290**


