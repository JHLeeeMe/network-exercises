#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

int main()
{
	// Init winsock
	WSADATA ws_data;
	WORD ver = MAKEWORD(2, 2);

	int ws_ok = WSAStartup(ver, &ws_data);
	if (ws_ok != 0)
	{
		std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
		return -1;
	}

	// Create a socket
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET)
	{
		std::cerr << "Can't create a socket! Quitting" << std::endl;
		return -1;
	}

	// Bind the ip_addr & port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(1201);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(server_socket, (sockaddr*)&hint, sizeof(hint));

	// Tell winsock the socket is for server_socket
	listen(server_socket, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int client_size = sizeof(client);

	SOCKET client_socket = accept(server_socket, (sockaddr*)&client, &client_size);
	//if (client_socket == INVALID_SOCKET)
	//{
	//	//Do something...
	//}

	char host[NI_MAXHOST];				// Client's remote name
	char service[NI_MAXHOST];			// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST);		// same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXHOST, 0) == 0)
	{
		std::cout << host << " connected on port " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
	}

	// Close  server socket
	closesocket(server_socket);

	// While loop: accept & echo msg back to client
	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytes_received = recv(client_socket, buf, 4096, 0);
		if (bytes_received == SOCKET_ERROR)
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
		send(client_socket, buf, bytes_received + 1, 0);
	}

	// Close the socket
	closesocket(client_socket);

	// Shutdown winsock
	WSACleanup();

	return 0;
}