#pragma once

struct GLFWwindow;

class WindowManager
{
public:
	void Init();
	void Update();
	bool IsWindowOpen();
	void Shutdown();
	void AddWindow(); // TODO


	~WindowManager();
private:
	
	bool isMainWindowOpen = true;

	GLFWwindow* window = nullptr;

	void DrawWindows();
	void DrawMainWindow();
	void DrawTestWindowContent();
};

extern WindowManager g_WindowManager;