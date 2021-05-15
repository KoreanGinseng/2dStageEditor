#include "MofImGui.h"

void CMofImGui::Setup(bool input_keyboard, bool input_controller)
{

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (input_keyboard)   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	if (input_controller) io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_pMainWindow->GetWindowHandle());
	ImGui_ImplDX11_Init(g_pGraphics->GetDevice(), g_pGraphics->GetDeviceContext());
}

void CMofImGui::Refresh(void)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CMofImGui::RenderSetup(void)
{
	ImGui::Render();
}

void CMofImGui::RenderGui(void)
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CMofImGui::Cleanup(void)
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

MofProcResult CMofImGuiProc::WindowProc(MofWindowHandle hWnd, MofUInt msg, MofProcParamW wpar, MofProcParamL lpar)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wpar, lpar))
		return true;
	return CDefWindowProc::WindowProc(hWnd, msg, wpar, lpar);
}
