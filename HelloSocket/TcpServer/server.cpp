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
	cout << "�¿ͻ��˼���:socket = " << _cSock << ", IP = " << inet_ntoa(clientAddr.sin_addr) /*ת��Ϊ �ַ��� �̶ȵ��ַ�������*/ << endl;
	char _recvBuf[128] = {};//���ջ�����
	while (true)
	{
		//5.���տͻ��˵���������
		int nLen = recv(_cSock, _recvBuf, 128, 0);
		if (nLen <= 0)
		{
			cout << "�ͻ������˳����������" << endl;
			break;
		}
		cout << "�յ����" << _recvBuf << endl;
		//6.��������
		if (strcmp(_recvBuf, "getName") == 0) {
			char msgBuf[] = "Xiao Qiang!";
			//7.��ͻ��˷���һ������send
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);//strlen(msgBuf) + 1 ����β��һ�����͹�ȥ
		}
		else if (strcmp(_recvBuf, "getAge") == 0) {
			char msgBuf[] = "18";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);//strlen(msgBuf) + 1 ����β��һ�����͹�ȥ
		}
		else {
			char msgBuf[] = "hello ,I'm server!";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);//strlen(msgBuf) + 1 ����β��һ�����͹�ȥ
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