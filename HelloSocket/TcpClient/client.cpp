#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>

using namespace std;
//���뾲̬���ӿ�
#pragma comment(lib,"ws2_32.lib")
//���Ŀ¼ $(SolutionDir)..bin/$(Platform)/$(Configuration)\
//�м�Ŀ¼ $(SolutionDir)../temp/$(Platform)/$(Configuration)/$(ProjectName)\

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
		if (strcmp(cmdBuf, "exit") == 0) {
			cout << "���˳��ͻ��ˣ����������" << endl;
			break;
		}
		else {
			//5.������͸������
			send(_sock, cmdBuf, strlen(cmdBuf) + 1, 0);
		}
		//6.���ܷ�������Ϣrecv
		char recBuf[256] = {};//�������ݻ�����
		int nlen = recv(_sock, recBuf, 256, 0);
		if (nlen > 0) {
			cout << "���ܵ�����Ϊ��" << recBuf << endl;
		}
		else {
			cout << "��������ʧ��" << endl;
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