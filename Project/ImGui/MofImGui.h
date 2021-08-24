#pragma once
#include    <Mof.h>

//ImGui
#include    "imgui.h"
#include    "imgui_impl_dx11.h"
#include    "imgui_impl_win32.h"

class CMofImGui {
private:

    CMofImGui()  = delete;
    ~CMofImGui() = delete;

public:

    static void Setup(bool keyboard = true, bool controller = true);

    static void Update();

    static void RenderSetup();
    static void RenderImGui();

    static void Cleanup();
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class CMofImGuiProc : public CDefWindowProc {
public:
    MofProcResult CMofImGuiProc::WindowProc(MofWindowHandle hWnd, MofUInt msg, MofProcParamW wpar, MofProcParamL lpar) override;
};
