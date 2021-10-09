#pragma once

#include <memory>

#include <winsock2.h>

#include <thread>

class IServerCallbacks;

class ConfigServer
{
public:
	bool Init(const char* port, std::weak_ptr<IServerCallbacks> serverCallbacks, uint16_t bufferSize = 512);

	void AcceptClient();
	void ListenClient();
	void StopListenClient();
	void CloseServer();

	void SetServerCallbacks(std::weak_ptr<IServerCallbacks> serverCallbacks);
	void ResizeBuffer(uint16_t newBufferSize);

	~ConfigServer();
	
private:

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	std::weak_ptr<IServerCallbacks> serverCallbacks;

	char* buffer = nullptr;
	uint16_t bufferSize = 0;

	std::thread clientAcceptWaitingThread;
	bool isClientAcceptWaitingThreadActive = false;
	
	std::thread listeningThread;
	bool isListeningThreadActive = false;
};