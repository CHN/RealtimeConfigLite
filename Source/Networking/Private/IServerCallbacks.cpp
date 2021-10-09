#include "IServerCallbacks.h"

void IServerCallbacks::SetConfigServer(std::weak_ptr<ConfigServer> configServer)
{
	this->configServer = configServer;
}
