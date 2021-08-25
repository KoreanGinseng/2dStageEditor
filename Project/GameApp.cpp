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
    
//POS,SIZE
ImVec2 menu_pos, menu_size;
ImVec2 tool_pos, tool_size;

//SHOW
enum class ShowFlag {
    layer,
    layer_data,
    background,
    color,
    textures,
    maphcip_texture,
    rect_edit,
    map_edit,
    map_edit_tab,
    map_preview_tab,
    count,
};
constexpr char* show_flag_names[] = {
    "layer",
    "layer_data",
    "background",
    "color",
    "textures",
    "maphcip_texture",
    "rect_edit",
    "map_edit",
    "map_edit_tab",
    "map_preview_tab",
    "count",
};
bool show_flags[static_cast<int>(ShowFlag::count)];

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

    CUtilities::SetCurrentDirectory("Resource");

    CMofImGui::Setup(false, false);
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    //SHOWFLAG
    memset(show_flags, true, sizeof(show_flags));

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
            //SHOW FLAGS
            for (int i = 0; i < (int)ShowFlag::count; i++) {
                ImGui::Checkbox(show_flag_names[i], &show_flags[i]);
            }
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
    if (show_flags[(int)ShowFlag::layer]) {
        ImGui::Begin("Layer", &show_flags[(int)ShowFlag::layer]); {

        }
        ImGui::End();
    }

    //LAYER Detail
    if (show_flags[(int)ShowFlag::layer_data]) {
        ImGui::Begin("LayerData", &show_flags[(int)ShowFlag::layer_data]); {

        }
        ImGui::End();
    }

    //BACKGROUND
    if (show_flags[(int)ShowFlag::background]) {
        ImGui::Begin("BackGround", &show_flags[(int)ShowFlag::background]); {

        }
        ImGui::End();
    }

    //COLOR
    if (show_flags[(int)ShowFlag::color]) {
        ImGui::Begin("Color", &show_flags[(int)ShowFlag::color]); {

        }
        ImGui::End();
    }

    //Textures
    if (show_flags[(int)ShowFlag::textures]) {
        ImGui::Begin("Textures", &show_flags[(int)ShowFlag::textures]); {

        }
        ImGui::End();
    }

    //MapChipTexture
    if (show_flags[(int)ShowFlag::maphcip_texture]) {
        ImGui::Begin("MapChipTexture", &show_flags[(int)ShowFlag::maphcip_texture]); {

        }
        ImGui::End();
    }

    //RECT EDIT
    if (show_flags[(int)ShowFlag::rect_edit]) {
        ImGui::Begin("RectEdit", &show_flags[(int)ShowFlag::rect_edit]); {

        }
        ImGui::End();
    }

    //MAP EDIT
    if (show_flags[(int)ShowFlag::map_edit]) {
        ImGui::Begin("MapEdit", &show_flags[(int)ShowFlag::map_edit]); {
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

    CUtilities::SetCurrentDirectory("../log");
    //SAVELOGS
    CUtilities::SetCurrentDirectory("../Resource");

    CMofImGui::Cleanup();

    return TRUE;
}