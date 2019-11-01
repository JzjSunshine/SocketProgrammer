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
struct Login:public DataHeader
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
		//5.把命令发送给服务端
		if (strcmp(cmdBuf, "exit") == 0) {
			cout << "已退出客户端，任务结束！" << endl;
			break;
		}
		else if (strcmp(cmdBuf, "login") == 0) {
			Login login/*{ "jmm","lovely" }*/;
			strcpy(login.userName, "jmm");
			strcpy(login.PassWord, "lovely");
			//5.把命令发送给服务端
			send(_sock, (char *)&login, sizeof(login), 0);
			// 接受服务器返回的数据
			LoginResult loginRet = {};
			recv(_sock, (char*)&loginRet, sizeof(loginRet), 0);
			cout << "LoginResult: " << loginRet.result << endl;
		}
		else if (strcmp(cmdBuf, "logout") == 0) {
			LogOut logout;//("jmm");
			strcpy(logout.userName, "jmm");
			//5.把命令发送给服务端
			send(_sock, (char *)&logout, sizeof(logout), 0);
			// 接受服务器返回的数据
			LoginResult logoutRet;
			recv(_sock, (char*)&logoutRet, sizeof(logoutRet), 0);
			cout << "LogOutResult: " << logoutRet.result << endl;
		}
		else {
			cout << "不支持该命令，请重新输入！" << endl;
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