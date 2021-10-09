#include "ConfigServer.h"

#include "IServerCallbacks.h"

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <cassert>
#include <thread>

#pragma comment (lib, "Ws2_32.lib")

bool ConfigServer::Init(const char* const port, const std::weak_ptr<IServerCallbacks> serverCallbacks, const uint16_t bufferSize)
{
	this->serverCallbacks = serverCallbacks;
	ResizeBuffer(bufferSize);

	WSADATA wsaData;
	int iResult;

	struct addrinfo* result = nullptr;
	struct addrinfo hints;

	int iSendResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	assert(iResult == 0 && "An error occurred");

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(nullptr, port, &hints, &result);
	
	assert(iResult == 0 && "An error occurred");

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		assert(false && "An error occurred");

		freeaddrinfo(result);
		WSACleanup();

		serverCallbacks.lock()->OnErrorOccurred(this, INVALID_SOCKET);

		return false;
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		assert(false && "An error occurred");

		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();

		serverCallbacks.lock()->OnErrorOccurred(this, iResult);

		return false;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		assert(false && "An error occurred");

		closesocket(ListenSocket);
		WSACleanup();

		serverCallbacks.lock()->OnErrorOccurred(this, iResult);

		return false;
	}

	return true;
}

void ConfigServer::AcceptClient()
{
	assert(!isClientAcceptWaitingThreadActive && "Already client accept waiting");

	isClientAcceptWaitingThreadActive = true;

	clientAcceptWaitingThread = std::thread([=] 
	{
		ClientSocket = accept(ListenSocket, nullptr, nullptr);

		isClientAcceptWaitingThreadActive = false;

		if (ClientSocket == INVALID_SOCKET)
		{
			closesocket(ListenSocket);
			WSACleanup();

			serverCallbacks.lock()->OnErrorOccurred(this, INVALID_SOCKET);
		}
		else
		{
			serverCallbacks.lock()->OnClientConnected(this);
		}
	});

	clientAcceptWaitingThread.detach();
}

void ConfigServer::ListenClient()
{
	assert(ListenSocket != INVALID_SOCKET && "Socket is not valid");
	assert(ClientSocket != INVALID_SOCKET && "No client is connected");

	assert(!isClientAcceptWaitingThreadActive && "Still waiting for a client");
	assert(!isListeningThreadActive && "Already listening client");

	isListeningThreadActive = true;

	listeningThread = std::thread([=]
	{
		while (isListeningThreadActive)
		{
			int result = recv(ClientSocket, buffer, bufferSize, 0);

			if (result <= 0)
			{
				serverCallbacks.lock()->OnClientDisconnected(this);
				StopListenClient();
				return;
			}

			serverCallbacks.lock()->OnDataReceived(this, buffer, result);
		}
	});

	listeningThread.detach();
}

void ConfigServer::StopListenClient()
{
	assert(isListeningThreadActive && "Listening already stopped");

	int result = shutdown(ClientSocket, SD_RECEIVE);

	if (result == SOCKET_ERROR)
	{
		serverCallbacks.lock()->OnErrorOccurred(this, result);
	}

	isListeningThreadActive = false;
	
	if (listeningThread.joinable())
	{
		listeningThread.join();
	}
}

void ConfigServer::CloseServer()
{
	assert(ListenSocket != INVALID_SOCKET && "Socket is not valid");
	assert(ClientSocket != INVALID_SOCKET && "No client is connected");

	int result = shutdown(ClientSocket, SD_BOTH);

	if (result == SOCKET_ERROR)
	{
		serverCallbacks.lock()->OnErrorOccurred(this, result);
	}

	closesocket(ClientSocket);
	WSACleanup();

	if (clientAcceptWaitingThread.joinable())
	{
		clientAcceptWaitingThread.join();
	}

	StopListenClient();
}

void ConfigServer::SetServerCallbacks(const std::weak_ptr<IServerCallbacks> serverCallbacks)
{
	this->serverCallbacks = serverCallbacks;
}

void ConfigServer::ResizeBuffer(const uint16_t newBufferSize)
{
	assert(!isListeningThreadActive && "Listening client should be stopped to change buffer size");

	if (buffer)
	{
		delete[] buffer;
	}

	buffer = new char[newBufferSize];
	bufferSize = newBufferSize;
}

ConfigServer::~ConfigServer()
{
	if (ListenSocket != INVALID_SOCKET && 
		ClientSocket != INVALID_SOCKET)
	{
		serverCallbacks.lock()->OnServerClosed(this);

		CloseServer();
	}

	delete[] buffer;
}