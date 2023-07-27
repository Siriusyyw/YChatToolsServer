#include "YChatToolsServer.h"

ChatToolsServer::ChatToolsServer():m_port(10246)
{
	if (init())
	{
		start();
	}
	printf("服务启动！！！\t\n");
}

ChatToolsServer::ChatToolsServer(int port)
{
	printf("元神启动！！！ <==> %d\t\n", port);
}

ChatToolsServer::~ChatToolsServer()
{
	if (m_servscok != -1)
	{
		close(m_servscok);
	}
	printf("服务结束！！！\t\n");
}

bool ChatToolsServer::init()
{
	sockaddr_in server_adrr;
	m_servscok = socket(PF_INET, SOCK_STREAM, 0);
	if (m_servscok == -1)
	{
		printf("Socket creation failed.\t\n");
		return false;
	}
	server_adrr.sin_family = AF_INET;
	server_adrr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_adrr.sin_port = htons(m_port);
	if (bind(m_servscok, (sockaddr*)&server_adrr, sizeof(server_adrr)) == -1)
	{
		printf("bind error %d %s\t\n", errno, strerror(errno));
		close(m_servscok);
		return false;
	}
	if (listen(m_servscok, 5) == -1)
	{
		printf("listen error %d %s\t\n", errno, strerror(errno));
		close(m_servscok);
		return false;
	}
	printf("Socket initialized successfully.listen on %d\t\n",m_port);
	return true;
}

void ChatToolsServer::start()
{
	epoll_event event;
	int epfd, event_cnt;
	epfd = epoll_create(1);
	if (epfd == -1)
	{
		printf("epoll_create error %d %s\t\n", errno, strerror(errno));
		return;
	}
	epoll_event* all_events = new epoll_event[MAX_EVENTS];
	event.events = EPOLLIN;
	event.data.fd = m_servscok;
	epoll_ctl(epfd, EPOLL_CTL_ADD, m_servscok, &event);
	printf("start epoll wait...\t\n");

	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_sz = sizeof(clnt_addr);
	memset(&clnt_addr, 0, sizeof(clnt_addr));

	while (true)
	{
		event_cnt = epoll_wait(epfd, all_events, 100, 1000);
		if (event_cnt == -1)
		{
			printf("epoll_wait error %d %s\t\n", errno, strerror(errno));
		}
		if (event_cnt == 0) continue;
		for (int i = 0; i < event_cnt; i++)
		{
			memset(m_buffer, 0, sizeof(m_buffer));
			int fd_current = all_events[i].data.fd;
			if (fd_current == m_servscok)		// client connection
			{
				int client_fd = accept(m_servscok, (sockaddr*)&clnt_addr, &clnt_addr_sz);
				event.events = EPOLLIN;
				event.data.fd = client_fd;
				epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &event);
				printf("client is connected:%d\n", client_fd);
				m_clients[client_fd] = { client_fd , "Anonymous" };
			}
			else // read and write
			{
				printf("read and write messgae\n");
				ssize_t  len = read(fd_current, m_buffer, sizeof(m_buffer));
				if (len <= 0)
				{	// client disconnected
					printf("client disconnected\n");
					epoll_ctl(epfd, EPOLL_CTL_DEL, fd_current, NULL);
					closeClient(fd_current);
					close(fd_current);
				}
				else
				{
					printf("write\n");
					write(fd_current, m_buffer, len);
					handleClientMessage(fd_current, "xxx");
					std::cout << "m_buffer = " << m_buffer << std::endl;
					//printf("receive from client:%s\n", buf);
					// send(buf, len);
				}
			}
		}
	}

	delete[] all_events;
	close(epfd);
}

void ChatToolsServer::handleClientMessage(int cfd, const std::string& msg)
{

}

void ChatToolsServer::closeClient(int cfd)
{
	// clean up resources
	m_clients.erase(cfd);
	printf("client is closed:%d\n", cfd);
}
