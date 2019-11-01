socket基础api介绍
一.简易TCP客户端Client
1. 建立一个套接字
2. 连接服务器 connect；
3. 向服务器发送数据； send
4. 接收服务端数据 recv；
5. 关闭 closesocket；
二. 简易TCP服务端 server
1. 建立一个socket
2. 绑定端口 bind；（启动端口监听模式）
3. 监听网络端口；（listen）
4. 等待客户端连接；（accept）
5. 接收客户端的数据；（recv）
6. 向客户端返回数据；（send）
7. 关闭客户端socket closesocket
