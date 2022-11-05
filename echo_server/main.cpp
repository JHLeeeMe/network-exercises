#include <iostream>
#ifdef __linux__
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#elif _WIN32
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
#endif

int main()
{
#ifdef _WIN32
	// Init winsock
	WSADATA ws_data;
	WORD version = MAKEWORD(2, 2);  // using WinSock 2.2

	int ws_ok = WSAStartup(version, &ws_data);
	if (ws_ok != 0)
	{
		std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
		return -1;
	}
#endif

	// Create a socket
#ifdef __linux__
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
#elif _WIN32
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET)
#endif
	{
		std::cerr << "Can't create a socket! Quitting" << std::endl;
		return -1;
	}

	// Bind the ip_addr & port to a socket
	sockaddr_in s_sockaddr_info;
	s_sockaddr_info.sin_family = AF_INET;
	s_sockaddr_info.sin_port = htons(1201);
#ifdef __linux__
	s_sockaddr_info.sin_addr.s_addr = htonl(INADDR_ANY);
#elif _WIN32
	s_sockaddr_info.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
#endif

	if (bind(server_socket, (sockaddr*)&s_sockaddr_info, sizeof(s_sockaddr_info)) < 0)
    {
		std::cerr << "Can't bind a socket! Quitting" << std::endl;
        return -1;
    }

	// Tell winsock the socket is for server_socket
	listen(server_socket, SOMAXCONN);

	// Wait for a connection
	sockaddr_in c_sockaddr_info;
#ifdef __linux__
	socklen_t c_sockaddr_info_len = sizeof(c_sockaddr_info);
	int client_socket = accept(server_socket, (sockaddr*)&c_sockaddr_info, &c_sockaddr_info_len);
	//if (client_socket == -1)
#elif _WIN32
	int c_sockaddr_info_len = sizeof(c_sockaddr_info);
	SOCKET client_socket = accept(server_socket, (sockaddr*)&c_sockaddr_info, &c_sockaddr_info_len);
	//if (client_socket == INVALID_SOCKET)
#endif
	//{
	//	//Do something...
	//}

	char host[NI_MAXHOST];				// Client's remote name
	char service[NI_MAXHOST];			// Service (i.e. port) the client is connect on

	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXHOST);
	//ZeroMemory(host, NI_MAXHOST);		// same as memset(host, 0, NI_MAXHOST);
	//ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo(
            (sockaddr*)&c_sockaddr_info, c_sockaddr_info_len,
            host, NI_MAXHOST,
            service, NI_MAXHOST, 0
        ) == 0)
	{
		std::cout << host << " connected on port " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &c_sockaddr_info.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port "
                  << ntohs(c_sockaddr_info.sin_port) << std::endl;
	}

	// Close  server socket
#ifdef __linux__
	close(server_socket);
#elif _WIN32
	closesocket(server_socket);
#endif

	// While loop: accept & echo msg back to client
	char buf[4096];
	while (true)
	{
		memset(buf, 0, 4096);
		//ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytes_received = recv(client_socket, buf, 4096, 0);
		if (bytes_received == -1)
		{
			std::cerr << "Error in recv(). Quitting" << std::endl;
			break;
		}

		if (bytes_received == 0)
		{
			std::cout << "Clinet disconnected " << std::endl;
			break;
		}

		// Echo message back to client
		send(client_socket, buf, bytes_received + 1, 0);  // +1: null
	}

	// Close the socket
#ifdef __linux__
	close(client_socket);
#elif _WIN32
	closesocket(client_socket);

	// Shutdown winsock
	WSACleanup();
#endif

	return 0;
}

