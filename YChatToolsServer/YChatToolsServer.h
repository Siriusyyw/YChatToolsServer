// YChatToolsServer.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <wait.h>
#include <pthread.h>
#include <string.h>
#include <unordered_map>

const int MAX_EVENTS = 100;
const int MAX_BUFFER_SIZE = 4096;

// client information structure
struct ClientInfo {
	int fd;
	std::string username;
};

class ChatToolsServer
{
public:
	ChatToolsServer();
	ChatToolsServer(int port);
	~ChatToolsServer();
private:
	bool init();
	void start();
	void handleClientMessage(int cfd, const std::string& msg);
	void closeClient(int cfd);
private:
	int m_port;
	int m_servscok;
	int m_clntsock;

	char m_buffer[MAX_BUFFER_SIZE] = "";
	std::unordered_map<int, ClientInfo> m_clients;
};

// TODO: 在此处引用程序需要的其他标头。
