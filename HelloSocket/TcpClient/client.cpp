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
	//-----------------------------
	//TCP客户端
	//1.建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == INVALID_SOCKET)
	{
		cout << "连接服务器失败..." << endl;
	}
	else {
		cout << "连接服务器成功.." << endl;
	}
	//2.连接服务器connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//声明服务端地址
	int ret = connect(_sock, (sockaddr*)(&_sin), sizeof(sockaddr_in));
	if (ret == SOCKET_ERROR)
	{
		cout << "连接服务器失败" << endl;
	}
	else {
		cout << "连接服务器成功" << endl;
	}
	while (true)
	{
		//3.输入请求命令
		char cmdBuf[128] = {};
		//cin >> cmdBuf;
		scanf("%s", cmdBuf);
		//4.处理请求
		if (strcmp(cmdBuf, "exit") == 0) {
			cout << "已退出客户端，任务结束！" << endl;
			break;
		}
		else {
			//5.把命令发送给服务端
			send(_sock, cmdBuf, strlen(cmdBuf) + 1, 0);
		}
		//6.接受服务器信息recv
		char recBuf[256] = {};//创建数据缓冲区
		int nlen = recv(_sock, recBuf, 256, 0);
		if (nlen > 0) {
			cout << "接受的数据为：" << recBuf << endl;
		}
		else {
			cout << "接受数据失败" << endl;
		}
	}
	
	//7.关闭socket closesocket
	closesocket(_sock);
	//----------------------------
	//清除windows socket环境
	WSACleanup();
	getchar();
	return 0;
}