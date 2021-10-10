#include "WindowManager.h"

int main()
{
	g_WindowManager.Init();

	while (!g_WindowManager.DoesWindowWantClose())
	{
		g_WindowManager.Update();
	}

	g_WindowManager.Shutdown();

	return 0;
}