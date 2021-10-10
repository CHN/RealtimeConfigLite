#pragma once

struct GLFWwindow;

class WindowManager
{
public:
	void Init();
	void Update();
	bool DoesWindowWantClose();
	void Shutdown();
	void AddWindow(); // TODO


	~WindowManager();
private:
	
	GLFWwindow* window = nullptr;

	void DrawWindows();
	void DrawMainWindow();
};

extern WindowManager g_WindowManager;