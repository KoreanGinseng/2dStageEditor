/*************************************************************************//*!

                    @file    GameApp.cpp
                    @brief    基本ゲームアプリ。

                                                            @author    濱田　享
                                                            @date    2014.05.14
*//**************************************************************************/

//INCLUDE
#include    "GameApp.h"
#include    "ImGui/MofImGui.h"
#include    "Resource.h"
#include    <filesystem>
    
//POS,SIZE
ImVec2 menu_pos, menu_size;
ImVec2 tool_pos, tool_size;

/*************************************************************************//*!
        @brief            アプリケーションの初期化
        @param            None

        @return            TRUE        成功<br>
                        それ以外    失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {
    struct stat s;
    if (stat("log", &s) != 0) {
        std::filesystem::create_directory("log");
    }

    CUtilities::SetCurrentDirectory("Resource");

    CMofImGui::Setup(false, false);
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    return TRUE;
}
/*************************************************************************//*!
        @brief            アプリケーションの更新
        @param            None

        @return            TRUE        成功<br>
                        それ以外    失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void) {
    //キーの更新
    g_pInput->RefreshKey();
    CMofImGui::Update();

    //DOCKING BACKGROUNDS
    {
        ImGuiWindowFlags dockspace_window_flag = ImGuiWindowFlags_NoDocking;
        dockspace_window_flag |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        dockspace_window_flag |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGui::SetNextWindowPos(ImVec2(0, tool_pos.y + tool_size.y));
        ImGui::SetNextWindowSize(ImVec2(g_pGraphics->GetTargetWidth(), g_pGraphics->GetTargetHeight() - (tool_pos.y + tool_size.y)));
        ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("##Dockspace", NULL, dockspace_window_flag); {
            ImGui::PopStyleVar(3);
            ImGuiDockNodeFlags dockspace_flag = ImGuiDockNodeFlags_None;
            ImGui::DockSpace(ImGui::GetID("##DockingBackGrounds"), ImVec2(0.0f, 0.0f), dockspace_flag);
        }
        ImGui::End();
    } // FIRST GUI
    
    
    bool is_ctrl_hold  = g_pInput->IsKeyHold(MOFKEY_LCONTROL) || g_pInput->IsKeyHold(MOFKEY_RCONTROL);
    bool is_shift_hold = g_pInput->IsKeyHold(MOFKEY_LSHIFT)   || g_pInput->IsKeyHold(MOFKEY_RSHIFT);

    //ImGui::ShowDemoWindow();
    
    //MAINMENU
    if (ImGui::BeginMainMenuBar()) {
        menu_pos  = ImGui::GetWindowPos();
        menu_size = ImGui::GetWindowSize();
        if (ImGui::BeginMenu("File")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    //TOOLBAR
    int toolbar_flag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;
    ImGui::SetNextWindowPos(ImVec2(0, menu_pos.y + menu_size.y));
    ImGui::SetNextWindowSize(ImVec2(g_pGraphics->GetTargetWidth() * 1.0f, g_pGraphics->GetTargetHeight() * 0.03f));
    ImGui::Begin("##ToolBar", NULL, toolbar_flag); {
        tool_pos  = ImGui::GetWindowPos();
        tool_size = ImGui::GetWindowSize();
    }
    ImGui::End();

    //LAYER
    ImGui::Begin("Layer"); {

    }
    ImGui::End();

    //LAYER Detail
    ImGui::Begin("LayerData"); {

    }
    ImGui::End();

    //BACKGROUND
    ImGui::Begin("BackGround"); {

    }
    ImGui::End();

    //COLOR
    ImGui::Begin("Color"); {

    }
    ImGui::End();

    //Textures
    ImGui::Begin("Textures"); {

    }
    ImGui::End();

    //MapChipTexture
    ImGui::Begin("MapChipTexture"); {

    }
    ImGui::End();

    //RECT EDIT
    ImGui::Begin("RectEdit"); {

    }
    ImGui::End();

    //MAP EDIT
    ImGui::Begin("MapEdit"); {
        if (ImGui::BeginTabBar("##MapEdit")) {

            //EDIT
            if (ImGui::BeginTabItem("Edit")) {
                ImGui::EndTabItem();
            }

            //PREVIEW
            if (ImGui::BeginTabItem("Preview")) {
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
    ImGui::End();

    return TRUE;
}
/*************************************************************************//*!
        @brief            アプリケーションの描画
        @param            None

        @return            TRUE        成功<br>
                        それ以外    失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void) {
    //描画開始
    g_pGraphics->RenderStart();
    //画面のクリア
    g_pGraphics->ClearTarget(0.06f, 0.06f, 0.06f, 0.0f, 1.0f, 0);

    CMofImGui::RenderSetup();
    CMofImGui::RenderImGui();
    
    //描画の終了
    g_pGraphics->RenderEnd();
    return TRUE;
}
/*************************************************************************//*!
        @brief            アプリケーションの解放
        @param            None

        @return            TRUE        成功<br>
                        それ以外    失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void) {

    CUtilities::SetCurrentDirectory("../log");
    //SAVELOGS
    CUtilities::SetCurrentDirectory("../Resource");

    CMofImGui::Cleanup();

    return TRUE;
}