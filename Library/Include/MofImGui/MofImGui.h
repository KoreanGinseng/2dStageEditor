#pragma once
#include "imgui.h"
#include "MofImGui/backends/imgui_impl_win32.h"
#include "MofImGui/backends/imgui_impl_dx11.h"

#include "Mof.h"

class CMofImGui
{
private:
	CMofImGui(void)  = delete;
	~CMofImGui(void) = delete;

public:
	
	static void Setup(bool input_keyboard = true, bool input_controller = true);

	static void Refresh(void);

	static void RenderSetup(void);

	static void RenderGui(void);

	static void Cleanup(void);
};

class CMofImGuiProc : public CDefWindowProc
{
public:
	virtual MofProcResult WindowProc(MofWindowHandle hWnd, MofUInt msg, MofProcParamW wpar, MofProcParamL lpar) override;
};
