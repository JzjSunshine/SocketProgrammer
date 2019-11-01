// ctrl + k + c
//  Ctrl + K+ U 
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>

using namespace std;
//加入静态连接库
#pragma comment(lib,"ws2_32.lib")
//输出目录 $(SolutionDir)..bin/$(Platform)/$(Configuration)\
//中间目录 $(SolutionDir)../temp/$(Platform)/$(Configuration)/$(ProjectName)\

enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	LOG_ERROR
};
//消息头
struct  DataHeader {
	short dataLength;//数据长度
	short cmd;//
};
//DataPackage
struct Login :public DataHeader
{
	Login()
	{
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	/*Login(char *name, char *pwd)
	{
		strcpy(userName, name);
		strcpy(PassWord, pwd);
	}*/
	char userName[32];
	char PassWord[32];
};

struct LoginResult :public DataHeader
{
	LoginResult()
	{
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};

struct LogOut :public DataHeader
{
	LogOut()
	{
		dataLength = sizeof(LogOut);
		cmd = CMD_LOGOUT;
	}
	/*LogOut(char *name)
	{
		strcpy(userName, name);
	}*/
	char userName[32];
};
struct LogOutResult :public DataHeader
{
	LogOutResult()
	{
		dataLength = sizeof(LogOutResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};
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
	cout << "新客户端加入: socket = " << _cSock << ", IP = " << inet_ntoa(clientAddr.sin_addr) /*转换为 字符串 刻度的字符串数字*/ << endl;
	
	while (true)
	{
		DataHeader header = {};
		//5.接收客户端的请求数据
		int nLen = recv(_cSock,(char*)&header, sizeof(DataHeader), 0);
		if (nLen <= 0)
		{
			cout << "客户端已退出，任务结束" << endl;
			break;
		}
		
		//6.处理请求
		switch (header.cmd) {
			case CMD_LOGIN: 
			{
				Login login = {};
				//做数据的偏移
				recv(_cSock, (char*)&login + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
				cout << "收到命令：数据命令：" << login.cmd << "，数据长度：" << login.dataLength << "，userName = " 
					<< login.userName << ",PassWord = " << login.PassWord << endl;
				LoginResult ret;
				//忽略判断用户名和密码是否正确
				send(_cSock, (char*)&ret, sizeof(LoginResult), 0);
			}
			break;
			case CMD_LOGOUT: 
			{
				LogOut logOut = {};
				recv(_cSock, (char*)&logOut + sizeof(DataHeader), sizeof(LogOut) - sizeof(DataHeader), 0);
				cout << "收到命令：数据命令：" << logOut.cmd << "，数据长度：" << logOut.dataLength << "，userName = "
					<< logOut.userName << endl;
				LogOutResult ret;
				//忽略判断用户名和密码是否正确
				send(_cSock, (char*)&ret, sizeof(LogOutResult), 0);
			}
			 break;
			default:
			{
				header.cmd = LOG_ERROR;
				header.dataLength = 0;
				send(_cSock, (char *)&header, sizeof(header), 0);//strlen(msgBuf) + 1 将结尾符一并发送过去
				break;
			}
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