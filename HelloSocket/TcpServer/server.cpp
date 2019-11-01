// ctrl + k + c
//  Ctrl + K+ U 
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
	//����windows socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//---------------------------
	//TCP �����
	//����windows scoket��̻���
	//1.����һ��socket

	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//ipv4 ������ TCP


	//2.�󶨽��ܿͻ������ӵĶ˿ں�bind
	sockaddr_in _sin = {};//
	_sin.sin_family = AF_INET;//�������� ipv4
	_sin.sin_port = htons(4567);// host to net unsigned short �˿ں� ��������ת�����������ݣ��޷�����������

	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");// ��������ip��ַ
													   //����֪���Լ��ı���ip��ַʱ����дΪ INADDR_ANY
	if (bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) == SOCKET_ERROR)
	{
		cout << "�����ڽ��տͻ������ӵ�����˿�ʧ��" << endl;
	}
	else {
		cout << "������˿ڳɹ�..." << endl;
	}//����˿��Ƿ�ռ��

	 //3.��������˿�listen
	if (listen(_sock, 5) == SOCKET_ERROR)//�ȴ����ӵĶ��еĳ���
	{
		cout << "��������˿�ʧ��..." << endl;
	}
	else {
		cout << "��������˿ڳɹ�..." << endl;
	}
	//4.�ȴ����ܿͻ�������accept
	sockaddr_in clientAddr = {};//�ͻ��˵ĵ�ַ�Ͷ˿�
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;
	
	_cSock = accept(_sock, (sockaddr *)&clientAddr, &nAddrLen);
	if (_cSock == INVALID_SOCKET)
	{
		cout << "���󣬽��յ���Ч�ͻ���SOCKET..." << endl;
	}
	else
	{
		cout << "�������ݿͻ�����Ч..." << endl;
	}
	cout << "�¿ͻ��˼���: socket = " << _cSock << ", IP = " << inet_ntoa(clientAddr.sin_addr) /*ת��Ϊ �ַ��� �̶ȵ��ַ�������*/ << endl;
	
	while (true)
	{
		DataHeader header = {};
		//5.���տͻ��˵���������
		int nLen = recv(_cSock,(char*)&header, sizeof(DataHeader), 0);
		if (nLen <= 0)
		{
			cout << "�ͻ������˳����������" << endl;
			break;
		}
		
		//6.��������
		switch (header.cmd) {
			case CMD_LOGIN: 
			{
				Login login = {};
				//�����ݵ�ƫ��
				recv(_cSock, (char*)&login + sizeof(DataHeader), sizeof(Login) - sizeof(DataHeader), 0);
				cout << "�յ�����������" << login.cmd << "�����ݳ��ȣ�" << login.dataLength << "��userName = " 
					<< login.userName << ",PassWord = " << login.PassWord << endl;
				LoginResult ret;
				//�����ж��û����������Ƿ���ȷ
				send(_cSock, (char*)&ret, sizeof(LoginResult), 0);
			}
			break;
			case CMD_LOGOUT: 
			{
				LogOut logOut = {};
				recv(_cSock, (char*)&logOut + sizeof(DataHeader), sizeof(LogOut) - sizeof(DataHeader), 0);
				cout << "�յ�����������" << logOut.cmd << "�����ݳ��ȣ�" << logOut.dataLength << "��userName = "
					<< logOut.userName << endl;
				LogOutResult ret;
				//�����ж��û����������Ƿ���ȷ
				send(_cSock, (char*)&ret, sizeof(LogOutResult), 0);
			}
			 break;
			default:
			{
				header.cmd = LOG_ERROR;
				header.dataLength = 0;
				send(_cSock, (char *)&header, sizeof(header), 0);//strlen(msgBuf) + 1 ����β��һ�����͹�ȥ
				break;
			}
		}	
	}
	//8.�ر�socket closesocket
	closesocket(_sock);

	//----------------------------
	//���windows socket����
	WSACleanup();
	getchar();
	return 0;
}