#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>

using namespace std;
//���뾲̬���ӿ�
#pragma comment(lib,"ws2_32.lib")
//���Ŀ¼ $(SolutionDir)..bin/$(Platform)/$(Configuration)\
//�м�Ŀ¼ $(SolutionDir)../temp/$(Platform)/$(Configuration)/$(ProjectName)\

enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	LOG_ERROR
};
//��Ϣͷ
struct  DataHeader {
	short dataLength;//���ݳ���
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
	//����windows socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//-----------------------------
	//TCP�ͻ���
	//1.����һ��socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == INVALID_SOCKET)
	{
		cout << "���ӷ�����ʧ��..." << endl;
	}
	else {
		cout << "���ӷ������ɹ�.." << endl;
	}
	//2.���ӷ�����connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//��������˵�ַ
	int ret = connect(_sock, (sockaddr*)(&_sin), sizeof(sockaddr_in));
	if (ret == SOCKET_ERROR)
	{
		cout << "���ӷ�����ʧ��" << endl;
	}
	else {
		cout << "���ӷ������ɹ�" << endl;
	}
	while (true)
	{
		//3.������������
		char cmdBuf[128] = {};
		//cin >> cmdBuf;
		scanf("%s", cmdBuf);
		//4.��������
		//5.������͸������
		if (strcmp(cmdBuf, "exit") == 0) {
			cout << "���˳��ͻ��ˣ����������" << endl;
			break;
		}
		else if (strcmp(cmdBuf, "login") == 0) {
			Login login/*{ "jmm","lovely" }*/;
			strcpy(login.userName, "jmm");
			strcpy(login.PassWord, "lovely");
			//5.������͸������
			send(_sock, (char *)&login, sizeof(login), 0);
			// ���ܷ��������ص�����
			LoginResult loginRet = {};
			recv(_sock, (char*)&loginRet, sizeof(loginRet), 0);
			cout << "LoginResult: " << loginRet.result << endl;
		}
		else if (strcmp(cmdBuf, "logout") == 0) {
			LogOut logout;//("jmm");
			strcpy(logout.userName, "jmm");
			//5.������͸������
			send(_sock, (char *)&logout, sizeof(logout), 0);
			// ���ܷ��������ص�����
			LoginResult logoutRet;
			recv(_sock, (char*)&logoutRet, sizeof(logoutRet), 0);
			cout << "LogOutResult: " << logoutRet.result << endl;
		}
		else {
			cout << "��֧�ָ�������������룡" << endl;
		}
	}
	
	//7.�ر�socket closesocket
	closesocket(_sock);
	//----------------------------
	//���windows socket����
	WSACleanup();
	getchar();
	return 0;
}