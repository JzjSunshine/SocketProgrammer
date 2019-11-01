#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>

using namespace std;
//加入静态连接库
#pragma comment(lib,"ws2_32.lib")
//输出目录 $(SolutionDir)..bin/$(Platform)/$(Configuration)\
//中间目录 $(SolutionDir)../temp/$(Platform)/$(Configuration)/$(ProjectName)\

int main()
{
	//启动windows socket 2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//---------------------------
	//TCP 服务端
	//启动windows scoket编程环境
	//1.建立一个socket

	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//ipv4 数据流 TCP


	//2.绑定接受客户端连接的端口号bind
	sockaddr_in _sin = {};//
	_sin.sin_family = AF_INET;//网络类型 ipv4
	_sin.sin_port = htons(4567);// host to net unsigned short 端口号 主机数据转换到网络数据，无符号数据类型

	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");// 服务器的ip地址
													   //当不知道自己的本机ip地址时可以写为 INADDR_ANY
	if (bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) == SOCKET_ERROR)
	{
		cout << "绑定用于接收客户端连接的网络端口失败" << endl;
	}
	else {
		cout << "绑定网络端口成功..." << endl;
	}//网络端口是否被占用

	 //3.监听网络端口listen
	if (listen(_sock, 5) == SOCKET_ERROR)//等待连接的队列的长度
	{
		cout << "监听网络端口失败..." << endl;
	}
	else {
		cout << "监听网络端口成功..." << endl;
	}
	//4.等待接受客户端连接accept
	sockaddr_in clientAddr = {};//客户端的地址和端口
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;
	
	_cSock = accept(_sock, (sockaddr *)&clientAddr, &nAddrLen);
	if (_cSock == INVALID_SOCKET)
	{
		cout << "错误，接收到无效客户端SOCKET..." << endl;
	}
	else
	{
		cout << "接收数据客户端有效..." << endl;
	}
	cout << "新客户端加入:socket = " << _cSock << ", IP = " << inet_ntoa(clientAddr.sin_addr) /*转换为 字符串 刻度的字符串数字*/ << endl;
	char _recvBuf[128] = {};//接收缓存区
	while (true)
	{
		//5.接收客户端的请求数据
		int nLen = recv(_cSock, _recvBuf, 128, 0);
		if (nLen <= 0)
		{
			cout << "客户端已退出，任务结束" << endl;
			break;
		}
		cout << "收到命令：" << _recvBuf << endl;
		//6.处理请求
		if (strcmp(_recvBuf, "getName") == 0) {
			char msgBuf[] = "Xiao Qiang!";
			//7.向客户端发送一条数据send
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);//strlen(msgBuf) + 1 将结尾符一并发送过去
		}
		else if (strcmp(_recvBuf, "getAge") == 0) {
			char msgBuf[] = "18";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);//strlen(msgBuf) + 1 将结尾符一并发送过去
		}
		else {
			char msgBuf[] = "hello ,I'm server!";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);//strlen(msgBuf) + 1 将结尾符一并发送过去
		}
		
	}
	//8.关闭socket closesocket
	closesocket(_sock);

	//----------------------------
	//清除windows socket环境
	WSACleanup();
	getchar();
	return 0;
}