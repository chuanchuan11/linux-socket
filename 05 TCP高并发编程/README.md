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








> TCP高并发编程


  - Selete


  - Poll

  - Epoll


