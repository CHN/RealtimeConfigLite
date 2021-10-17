#pragma once

struct GLFWwindow;

class WindowManager
{
public:

	void Init();
	void Update();
	bool IsWindowOpen();
	void Shutdown();
	static void AddWindow(); // TODO

	~WindowManager();

private:
	
	bool isMainWindowOpen = true;

	GLFWwindow* glfwWindow = nullptr;

	void DrawWindows();
	void DrawMainWindow();
	void DrawTestWindowContent();
};