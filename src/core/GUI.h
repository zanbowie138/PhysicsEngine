#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GUI
{
public:
	bool MouseOver() const;
	void SetMouse(bool value);


	void Init(GLFWwindow* window);
	void NewFrame();
	void Draw();
	void Render();

	void Clean();
};

inline bool GUI::MouseOver() const
{
	return ImGui::GetIO().WantCaptureMouse;
}

inline void GUI::SetMouse(const bool value)
{
	ImGuiIO& io = ImGui::GetIO();

	if (!value)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	}
}

inline void GUI::Init(GLFWwindow* window)
{
	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

inline void GUI::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

inline void GUI::Draw()
{
	ImGui::Begin("FPS Counter");
	std::stringstream ss;
	//ss << "FPS: " << fps << "\nMSPF: " << mspf;
	std::string s = ss.str();
	ImGui::Text(s.c_str());
	ImGui::End();
}

inline void GUI::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

inline void GUI::Clean()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}