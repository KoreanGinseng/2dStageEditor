#include    "GameApp.h"
#include    "Resource.h"
#include    "Utilities/EditorUtilities.h"
#include    <MofImGui/MofImGui.h>

int WINAPI _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    Mof::LPFramework pFrame = new Mof::CDX11GameFramework(); 
    Mof::WINDOWSGAMEFRAMEWORKINITIALIZEINFO Info;
    Info.WindowCreateInfo.pProc  = new CMofImGuiProc();
    Info.WindowCreateInfo.hIcon  = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    Info.WindowCreateInfo.Style  = WS_OVERLAPPEDWINDOW;
    Info.WindowCreateInfo.Title  = EditorUtilities::GetVersionResourceData()->ProductName.c_str();
    Info.WindowCreateInfo.Width  = 1280;
    Info.WindowCreateInfo.Height =  720;
    Info.GraphicsCreateInfo.bMSAA = FALSE;
    Info.pApplication = new CGameApp();
    if ( pFrame->Initialize(&Info) )
    {
        pFrame->Run();
    }
    MOF_SAFE_DELETE(pFrame);
    return 0;
}