#pragma once

class ConfigServer;

class IServerCallbacks
{
public:

	virtual void OnClientConnected(ConfigServer* const configServer) {}
	virtual void OnDataReceived(ConfigServer* const configServer, const char* const buffer, const size_t bufferLength) {}
	virtual void OnServerClosed(ConfigServer* const configServer) {}
	virtual void OnClientDisconnected(ConfigServer* const configServer) {}
	virtual void OnErrorOccurred(ConfigServer* const configServer, const int errorCode) {}
};