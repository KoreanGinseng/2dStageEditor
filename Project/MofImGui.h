#pragma once
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_dx11.h"
#include "ImGui/backends/imgui_impl_win32.h"

#include <Mof.h>

class CMofImGui
{
private:

    CMofImGui()  = delete;
    ~CMofImGui() = delete;

public:
    static void Setup(bool input_keyboard = true, bool input_controller = true);

    static void Refresh();

    static void RenderSetup();

    static void RenderGui();

    static void Cleanup();
};

class CMofImGuiProc : public CDefWindowProc
{
public:
    virtual MofProcResult WindowProc(MofWindowHandle hWnd, MofUInt msg, MofProcParamW wpar, MofProcParamL lpar) override;
};