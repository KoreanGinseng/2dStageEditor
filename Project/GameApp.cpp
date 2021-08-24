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
#include    "Stage/Stage.h"

//UTIL
#include    "Utilities/EditorUtilities.h"
#include    "Utilities/ToolIcon.h"
#include    "EditorParam/EditorParameter.h"
#include    "EditorParam/ParameterKey.h"

//MNG
#include    "Manager/ImGuiWindowManager.h"
#include    "Manager/CommandManager.h"

//WINDOW
#include    "Window/LogWindow.h"
#include    "Window/EditWindow.h"
#include    "Window/MapChipWindow.h"
#include    "Window/LayerWindow.h"

//MENU
#include    "Menu/MainMenu.h"
#include    "Menu/ToolMenu.h"

//GLOBAL
std::string              resource_path;
std::string              open_file = ".txt";
EditMode                 edit_mode = EditMode::Write;
bool                     show_layer_window       = true;
bool                     show_chip_window        = true;
bool                     show_chip_window_child  = true;
bool                     show_edit_window        = true;
bool                     show_edit_window_child  = true;
bool                     show_main_menu          = true;
bool                     show_tool_menu          = true;
bool                     show_log_window         = false;
bool                     show_x_memory           = false;
bool                     show_y_memory           = false;
bool                     edit_grid_flag          = false;
bool                     chip_grid_flag          = false;
MofU32                   edit_background_color = MOF_COLOR_CBLACK;
MofU32                   edit_font_color       = MOF_COLOR_WHITE;
EditWindow               edit_window;
MapChipWindow            mapchip_window;
LayerWindow              layer_window;
Stage                    stage;

std::function<void(void)> def_create;

void default_create(void) {
    MapChip mapchip_layer;
    MapChip enemy_layer;
    MapChip item_layer;
    mapchip_layer.SetName("mapchip");
    mapchip_layer.SetTextureNo(-1);
    mapchip_layer.SetChipSize(Vector2(32, 32));
    mapchip_layer.Create(70, 25);
    stage.GetChipArrayPointer()->push_back(std::move(mapchip_layer));
    enemy_layer.SetName("enemy");
    enemy_layer.SetTextureArray(true);
    enemy_layer.SetTextureNo(0);
    enemy_layer.SetChipSize(Vector2(32, 32));
    enemy_layer.Create(70, 25);
    stage.GetChipArrayPointer()->push_back(std::move(enemy_layer));
    item_layer.SetName("item");
    item_layer.SetTextureNo(1);
    item_layer.SetTextureArray(true);
    item_layer.SetChipSize(Vector2(32, 32));
    item_layer.Create(70, 25);
    stage.GetChipArrayPointer()->push_back(std::move(item_layer));

    stage.GetTextureArraysPointer()->push_back(TextureArray());
    stage.GetTextureArraysPointer()->push_back(TextureArray());
}

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
    ToolIcon::Load();
    CUtilities::SetCurrentDirectory("Resource");

    resource_path = std::filesystem::current_path().string();
    def_create = default_create;

    CMofImGui::Setup(false, false);
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    //PARAM REGISTERS
    theParam.Register(ParamKey::EditMode, &edit_mode);
    theParam.Register(ParamKey::OpenFile, &open_file);
    theParam.Register(ParamKey::MainMenu, &show_main_menu);
    theParam.Register(ParamKey::ToolMenu, &show_tool_menu);
    theParam.Register(ParamKey::ResourcePath, &resource_path);
    theParam.Register(ParamKey::ChipGridFlag, &chip_grid_flag);
    theParam.Register(ParamKey::EditGridFlag, &edit_grid_flag);
    theParam.Register(ParamKey::ShowMapChipWindow, &show_chip_window);
    theParam.Register(ParamKey::ShowMapChipWindowChild, &show_chip_window_child);
    theParam.Register(ParamKey::ShowEditWindow, &show_edit_window);
    theParam.Register(ParamKey::ShowEditWindowChild, &show_edit_window_child);
    theParam.Register(ParamKey::ShowLayerWindow, &show_layer_window);
    theParam.Register(ParamKey::ShowLogWindow, &show_log_window);
    theParam.Register(ParamKey::Stage, &stage);
    theParam.Register(ParamKey::TextureArrays, stage.GetTextureArraysPointer());
    theParam.Register(ParamKey::MapChipTextureArray, stage.GetChipTextureArrayPointer());
    theParam.Register(ParamKey::BackgroundArray, stage.GetBackgroundArrayPointer());
    theParam.Register(ParamKey::MapChipArray, stage.GetChipArrayPointer());
    theParam.Register(ParamKey::EditBackColor, &edit_background_color);
    theParam.Register(ParamKey::EditFontColor, &edit_font_color);
    theParam.Register(ParamKey::DefaultCreate, &def_create);
    theParam.Register(ParamKey::MemoryX, &show_x_memory);
    theParam.Register(ParamKey::MemoryY, &show_y_memory);

    if (open_file != ".txt" && open_file.length()) {
        stage.Load(open_file);
    }
    else {
        default_create();
    }

    layer_window.Initialize();   /*  first
  */mapchip_window.Initialize(); /*  second
  */edit_window.Initialize();    /*  third
  */

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
    
    if (std::optional<RECT> rect = EditorUtilities::ChangeWindowSize()) {

    }

    MainMenu::Show();
    ToolMenu::Show();

    layer_window.Show();
    mapchip_window.Show();
    edit_window.Show();

    if (!EditorUtilities::IsPopupModalOpen()) {
        mapchip_window.Update();
        edit_window.Update();
    }

    Vector2 mp;
    g_pInput->GetMousePos(mp);

    auto log_area = theImGuiWindowManager.Find(ParamKey::ShowLogWindow);
    if (log_area && log_area->CollisionPoint(mp) && g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON)) {
        ImGui::SetWindowFocus("log window");
    }
    if (show_log_window) {
        LogWindow::Show();
    }

    bool is_ctrl_hold  = g_pInput->IsKeyHold(MOFKEY_LCONTROL) || g_pInput->IsKeyHold(MOFKEY_RCONTROL);
    bool is_shift_hold = g_pInput->IsKeyHold(MOFKEY_LSHIFT)   || g_pInput->IsKeyHold(MOFKEY_RSHIFT);
    if (is_ctrl_hold && g_pInput->IsKeyPush(MOFKEY_N)) {
        MainMenu::NewProject();
    }
    if (is_ctrl_hold && g_pInput->IsKeyPush(MOFKEY_O)) {
        MainMenu::OpenProject();
    }
    if (is_ctrl_hold && g_pInput->IsKeyPush(MOFKEY_S)) {
        MainMenu::SaveProject();
    }
    if (is_ctrl_hold && is_shift_hold && g_pInput->IsKeyPush(MOFKEY_S)) {
        MainMenu::SaveAsProject();
    }
    if (is_ctrl_hold && g_pInput->IsKeyPush(MOFKEY_Z)) {
        theCommandManager.Undo();
    }
    if (is_ctrl_hold && g_pInput->IsKeyPush(MOFKEY_Y)) {
        theCommandManager.Redo();
    }
    if (is_ctrl_hold && g_pInput->IsKeyPush(MOFKEY_M)) {
        MainMenu::OpenManual();
    }
    if (is_ctrl_hold && g_pInput->IsKeyPush(MOFKEY_I)) {
        MainMenu::Version();
    }
    bool is_not_mouse_hold = (!g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON) && !g_pInput->IsMouseKeyHold(MOFMOUSE_RBUTTON));
    if (is_not_mouse_hold && g_pInput->IsKeyPush(MOFKEY_W)) {
        EditorUtilities::SetWriteMode();
    }
    if (is_not_mouse_hold && g_pInput->IsKeyPush(MOFKEY_E)) {
        EditorUtilities::SetDeleteMode();
    }
    auto select_mode_key = g_pInput->IsKeyPush(MOFKEY_R) ||
        ((EditorUtilities::IsWriteMode() &&
        g_pInput->IsKeyPush(MOFKEY_LSHIFT) ||
        g_pInput->IsKeyPush(MOFKEY_RSHIFT)));
    if (is_not_mouse_hold && select_mode_key) {
        EditorUtilities::SetSelectMode();
    }

    //ImGui::ShowDemoWindow();

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


    edit_window.Render();
    mapchip_window.Render();

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

    EditorUtilities::SetCurrentPathResource();

    CUtilities::SetCurrentDirectory("../log");
    LogWindow::Save();
    CUtilities::SetCurrentDirectory("../Resource");

    CMofImGui::Cleanup();

    ToolIcon::Release();

    stage.Release();

    return TRUE;
}