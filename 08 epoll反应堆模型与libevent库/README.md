
> epoll模型特点

(1) 方式1：

    在server -> 创建树的根节点 -> 在树上添加需要监听的节点 -> 监听读事件 -> 有返回- > 通信(读/写) -> epoll_wait

    特点： 直接写的方式，不一定能写成功，因为不知道缓冲区是否可写[例子：05 TCP高并发编程]
    
(2) 方式2：

    在server -> 创建树的根节点 -> 在树上添加需要监听的节点 -> 监听读事件 -> 有返回-> 通信(接收数据)
    -> 将这个fd从树上删除 -> 监听写事件 -> 写操作 -> fd从树上摘下来 -> 监听fd的读事件 -> epoll_wait

    特点： 采用这种监控的方式，保证了能够写成功[例子：见附件]


> EPOllOUT事件在不同工作模式下的区别

	- 水平模式:
		○ struct epoll_event ev;
			§ ev.events = EPLLOUT;
			§ epoll_wait会一直返回, 缓冲区只要能写数据, 该函数就返回, 缓冲区满的时候, 不返回
	- 边缘模式:
		○ 第一次设置的时候epoll_wait会返回一次
                ○ 缓冲区从满 -> 到不满的时候会返回

> libevent库

- 待更新
