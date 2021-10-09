#pragma once

#include "ConfigServer.h"

#include <memory>

class ConfigServer;

class IServerCallbacks
{
public:

	void SetConfigServer(std::weak_ptr<ConfigServer> configServer);

	virtual void OnClientConnected() {}
	virtual void OnDataReceived(const char* buffer, size_t bufferLength) {}
	virtual void OnServerClosed() {}
	virtual void OnClientDisconnected() {}
	virtual void OnErrorOccurred(int errorCode) {}

protected:

	std::weak_ptr<ConfigServer> configServer;
};