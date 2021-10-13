#include "WindowManager.h"

int main()
{
	g_WindowManager.Init();

	while (g_WindowManager.IsWindowOpen())
	{
		g_WindowManager.Update();
	}

	g_WindowManager.Shutdown();

	return 0;
}