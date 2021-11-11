/*************************************************************************//*!

                    @file    GameApp.cpp
                    @brief    ��{�Q�[���A�v���B

                                                            @author    �_�c�@��
                                                            @date    2014.05.14
*//**************************************************************************/

//INCLUDE
#include    "GameApp.h"
#include    "ImGui/MofImGui.h"
#include    "Resource.h"
#include    <filesystem>

//MENU
#include    "Menu/MainMenu.h"
#include    "Menu/ToolBar.h"


bool show_flags[static_cast<int>(ShowFlag::count)];

//GLOBAL
MainMenu main_menu;
ToolBar  tool_bar;
EditorMode editor_mode = EditorMode::write;

/*************************************************************************//*!
        @brief            �A�v���P�[�V�����̏�����
        @param            None

        @return            TRUE        ����<br>
                        ����ȊO    ���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {
    struct stat s;
    if (stat("log", &s) != 0) {
        std::filesystem::create_directory("log");
    }

    CMofImGui::Setup(false, false);
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    //SHOWFLAG
    memset(show_flags, true, sizeof(show_flags));

    //LOAD
    for (int i = 0; i < (int)BB_InResourceTexture::count; i++) {
        auto texture = new CInResourceTexture();
        texture->Load(in_resource_texture_ids[i], in_resource_texture_names[i]);
        theBB_InResourceTexture.Register((BB_InResourceTexture)i, texture);
    }

    return TRUE;
}
/*************************************************************************//*!
        @brief            �A�v���P�[�V�����̍X�V
        @param            None

        @return            TRUE        ����<br>
                        ����ȊO    ���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void) {
    //�L�[�̍X�V
    g_pInput->RefreshKey();
    CMofImGui::Update();

    if (auto rect = EditorUtilities::ChangeWindowSize()) {
        //ChangeWindow
    }

    //DOCKING BACKGROUNDS
    {
        auto tool_pos = theBB_ImVec2.GetData(BB_ImVec2::ToolBarPos);
        auto tool_size = theBB_ImVec2.GetData(BB_ImVec2::ToolBarSize);
        ImGuiWindowFlags dockspace_window_flag = ImGuiWindowFlags_NoDocking;
        dockspace_window_flag |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        dockspace_window_flag |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGui::SetNextWindowPos(ImVec2(0, tool_pos->y + tool_size->y));
        ImGui::SetNextWindowSize(ImVec2(g_pGraphics->GetTargetWidth(), g_pGraphics->GetTargetHeight() - (tool_pos->y + tool_size->y)));
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
    main_menu.UpdateGui();
    
    //TOOLBAR
    tool_bar.UpdateGui();

    MOF_PRINTLOG("%s\n", (ImGui::IsAnyItemHovered() ? "TRUE" : "FALSE"));

    //LAYER
    if (show_flags[(int)ShowFlag::layer]) {
        ImGui::Begin("Layer", &show_flags[(int)ShowFlag::layer], EditorUtilities::GetGuiWindowFlag()); {

        }
        ImGui::End();
    }

    //LAYER DATA
    if (show_flags[(int)ShowFlag::layer_data]) {
        ImGui::Begin("LayerData", &show_flags[(int)ShowFlag::layer_data], EditorUtilities::GetGuiWindowFlag()); {

        }
        ImGui::End();
    }

    //BACKGROUND
    if (show_flags[(int)ShowFlag::background]) {
        ImGui::Begin("BackGround", &show_flags[(int)ShowFlag::background], EditorUtilities::GetGuiWindowFlag()); {

        }
        ImGui::End();
    }

    //COLOR
    if (show_flags[(int)ShowFlag::color]) {
        ImGui::Begin("Color", &show_flags[(int)ShowFlag::color], EditorUtilities::GetGuiWindowFlag()); {

        }
        ImGui::End();
    }

    //Textures
    if (show_flags[(int)ShowFlag::textures]) {
        ImGui::Begin("Textures", &show_flags[(int)ShowFlag::textures], EditorUtilities::GetGuiWindowFlag()); {

        }
        ImGui::End();
    }

    //MapChipTexture
    if (show_flags[(int)ShowFlag::maphcip_texture]) {
        ImGui::Begin("MapChipTexture", &show_flags[(int)ShowFlag::maphcip_texture], EditorUtilities::GetGuiWindowFlag()); {

        }
        ImGui::End();
    }

    //RECT EDIT
    if (show_flags[(int)ShowFlag::rect_edit]) {
        ImGui::Begin("RectEdit", &show_flags[(int)ShowFlag::rect_edit], EditorUtilities::GetGuiWindowFlag()); {

        }
        ImGui::End();
    }

    //MAP EDIT
    if (show_flags[(int)ShowFlag::map_edit]) {
        ImGui::Begin("MapEdit", &show_flags[(int)ShowFlag::map_edit], EditorUtilities::GetGuiWindowFlag()); {
            if (ImGui::BeginTabBar("##MapEdit")) {

                //EDIT
                if (show_flags[(int)ShowFlag::map_edit_tab]) {
                    if (ImGui::BeginTabItem("Edit", &show_flags[(int)ShowFlag::map_edit_tab])) {
                        ImGui::EndTabItem();
                    }
                }

                //PREVIEW
                if (show_flags[(int)ShowFlag::map_preview_tab]) {
                    if (ImGui::BeginTabItem("Preview", &show_flags[(int)ShowFlag::map_preview_tab])) {
                        ImGui::EndTabItem();
                    }
                }
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    return TRUE;
}
/*************************************************************************//*!
        @brief            �A�v���P�[�V�����̕`��
        @param            None

        @return            TRUE        ����<br>
                        ����ȊO    ���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void) {
    //�`��J�n
    g_pGraphics->RenderStart();
    //��ʂ̃N���A
    g_pGraphics->ClearTarget(0.06f, 0.06f, 0.06f, 0.0f, 1.0f, 0);

    CMofImGui::RenderSetup();
    CMofImGui::RenderImGui();
    
    //�`��̏I��
    g_pGraphics->RenderEnd();
    return TRUE;
}
/*************************************************************************//*!
        @brief            �A�v���P�[�V�����̉��
        @param            None

        @return            TRUE        ����<br>
                        ����ȊO    ���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void) {

    CUtilities::SetCurrentDirectory("log");
    //SAVELOGS
    CUtilities::SetCurrentDirectory("../");

    CMofImGui::Cleanup();

    theBB_string.Clear();
    theBB_ImVec2.Clear();
    theBB_InResourceTexture.AllDelete();

    return TRUE;
}