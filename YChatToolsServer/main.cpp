// main.cpp : ����Ӧ�ó������ڵ㡣

#include "YChatToolsServer.h"

using namespace std;

int main()
{
	ChatToolsServer* pServer = new ChatToolsServer();
	cout << "Hello CMake." << endl;
	for (size_t i = 1; i <= 10; i++)
	{
		cout << "cycle test��" << i  << endl;
	}
	return 0;
}