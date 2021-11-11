#include "MenuBar.h"
#include "EditorFunc.h"
#include "AnimationData.h"
#include "EditorUtilities.h"
#include "resource.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// コンストラクタ
/// </summary>
// ********************************************************************************
CMenuBar::CMenuBar()
{
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
// ********************************************************************************
CMenuBar::~CMenuBar()
{
}

// ********************************************************************************
/// <summary>
/// 更新
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CMenuBar::Update()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {
                if (theEditorFunc.CloseFile())
                {
                    theEditorFunc.SaveFile();
                }
            }
            if (ImGui::MenuItem("Open"))
            {
                if (theEditorFunc.CloseFile())
                {
                    theEditorFunc.OpenFile();
                }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Close"))
            {
                theEditorFunc.CloseFile();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save As..."))
            {
                theEditorFunc.SaveFile();
            }
            if (ImGui::MenuItem("Save"))
            {
                const auto& saveFile = CSingletonBlackboard<std::string>::GetInstance()
                    .Get<std::string>("SaveFileName");
                theEditorFunc.SaveFile(saveFile.c_str());
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                if (theEditorFunc.CloseFile())
                {
                    PostQuitMessage(0);
                }
            }
            ImGui::EndMenu();
        }
        /*if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo"))
            {

            }
            if (ImGui::MenuItem("Redo"))
            {

            }
            ImGui::EndMenu();
        }*/
        if (ImGui::BeginMenu("Info"))
        {
            if (ImGui::MenuItem("Version"))
            {
                std::string appname = CEditorUtilities::GetInstance()
                    .GetVersionResourceData()->ProductName;
                std::string version = CEditorUtilities::GetInstance()
                    .GetVersionResourceData()->ProductVersion;
                std::string copyright = CEditorUtilities::GetInstance()
                    .GetVersionResourceData()->LegalCopyright;
                std::string detail =
                    appname + " ver." + version + "\n"
                    "Copyright (c) 2021 " + copyright;
                ShellAbout(g_pMainWindow->GetWindowHandle(), appname.c_str(), detail.c_str(), ::LoadIcon(g_pMainWindow->GetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1)));
            }
            /*if (ImGui::MenuItem("Manual"))
            {

            }*/
            ImGui::EndMenu();
        }
        // INFO
        {
            const MofBool& bSaveCheck = CSingletonBlackboard<MofBool>::GetInstance()
                .Get<MofBool>("SaveCheck");
            ImGui::Text("fps(%ld)", CUtilities::GetFPS());
            ImGui::Text("%s", (bSaveCheck == TRUE ? "*chenge*" : ""));
        }
        ImGui::EndMenuBar();
    }

    return TRUE;
}
