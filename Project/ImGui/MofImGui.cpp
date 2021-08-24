#include "MofImGui.h"

void CMofImGui::Setup(bool keyboard, bool controller)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	// Enable Keyboard Controls
	if (keyboard) io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Enable Gamepad Controls
	if (controller) io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_pMainWindow->GetWindowHandle());
	ImGui_ImplDX11_Init(g_pGraphics->GetDevice(), g_pGraphics->GetDeviceContext());
}

void CMofImGui::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CMofImGui::RenderSetup()
{
	ImGui::Render();
}

void CMofImGui::RenderImGui()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CMofImGui::Cleanup()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

MofProcResult CMofImGuiProc::WindowProc(MofWindowHandle hWnd, MofUInt msg, MofProcParamW wpar, MofProcParamL lpar)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wpar, lpar)) {
		return true;
	}
	return CDefWindowProc::WindowProc(hWnd, msg, wpar, lpar);
}
