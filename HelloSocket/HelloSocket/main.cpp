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
	//----------------------------
	//TCP客户端
	//1.建立一个socket
	//2.连接服务器connect
	//3.接受服务器信息recv
	//4.关闭socket closesocket
	//---------------------------

	//TCP 服务端
	//启动windows scoket编程环境
	//1.建立一个socket
	//2.绑定接受客户端连接的端口号bind
	//3.监听网络端口listen
	//4.等待接受客户端连接accept
	//6.关闭socket closesocket
	//----------------------------
	//清除windows socket环境
	WSACleanup();
	return 0;
}