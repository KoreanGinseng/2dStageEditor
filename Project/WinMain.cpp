#include	"GameApp.h"
#include    "resource.h"
#include    "MofImGui.h"
#include    "EditorDefine.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//_CrtSetBreakAlloc(9370);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Mof::LPFramework pFrame = new Mof::CDX11GameFramework();
	Mof::WINDOWSGAMEFRAMEWORKINITIALIZEINFO Info;
	Info.WindowCreateInfo.hIcon  = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	Info.WindowCreateInfo.pProc  = NEW CMofImGuiProc();
	Info.WindowCreateInfo.Title = "Editor";
	Info.WindowCreateInfo.Width  = Editor::k_SceneW;
	Info.WindowCreateInfo.Height = Editor::k_SceneH;
	Info.pApplication = new CGameApp();
	if (pFrame->Initialize(&Info))
	{
		pFrame->Run();
	}
	MOF_SAFE_DELETE(pFrame);
	return 0;
}