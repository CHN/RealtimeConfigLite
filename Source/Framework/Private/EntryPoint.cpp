#include "WindowManager.h"

#include <memory>

int main()
{
	std::unique_ptr<WindowManager> windowManager = std::make_unique<WindowManager>();

	windowManager->Init();

	while (windowManager->IsWindowOpen())
	{
		windowManager->Update();
	}

	windowManager->Shutdown();

	return 0;
}