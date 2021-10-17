#include "WindowManager.h"

#include "glad/gl.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"

#include <cassert>

void WindowManager::Init()
{
	assert(glfwInit() && "GLFW could not init");

#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	glfwWindow = glfwCreateWindow(640, 480, "My Title", nullptr, nullptr);

	assert(glfwWindow && "GLFW Window could not create");

	glfwMakeContextCurrent(glfwWindow);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);
	glfwHideWindow(glfwWindow);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	bool isSuccessful = ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	assert(isSuccessful && "ImGUI could not initialize");

	isSuccessful = ImGui_ImplOpenGL3_Init(glsl_version);
}

void WindowManager::Update()
{
	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	DrawWindows();

	ImGui::Render();

	int display_w, display_h;
	glfwGetFramebufferSize(glfwWindow, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	glfwSwapBuffers(glfwWindow);
}

bool WindowManager::IsWindowOpen()
{
	return isMainWindowOpen;
}

void WindowManager::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(glfwWindow);

	glfwTerminate();

	glfwWindow = nullptr;
}

WindowManager::~WindowManager()
{
	assert(glfwWindow == nullptr && "Window manager must be shutdown before destroy");
}

void WindowManager::DrawWindows()
{
	DrawMainWindow();
}

void WindowManager::DrawMainWindow()
{
	ImGui::Begin("MainWindow", &isMainWindowOpen, ImGuiWindowFlags_NoCollapse);

	ImGui::End();
}

void WindowManager::DrawTestWindowContent()
{
	
}
