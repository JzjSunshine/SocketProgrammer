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
	//----------------------------
	//TCP�ͻ���
	//1.����һ��socket
	//2.���ӷ�����connect
	//3.���ܷ�������Ϣrecv
	//4.�ر�socket closesocket
	//---------------------------

	//TCP �����
	//����windows scoket��̻���
	//1.����һ��socket
	//2.�󶨽��ܿͻ������ӵĶ˿ں�bind
	//3.��������˿�listen
	//4.�ȴ����ܿͻ�������accept
	//6.�ر�socket closesocket
	//----------------------------
	//���windows socket����
	WSACleanup();
	return 0;
}