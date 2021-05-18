/*************************************************************************//*!

                    @file    GameApp.cpp
                    @brief    基本ゲームアプリ。

                                                            @author    濱田　享
                                                            @date    2014.05.14
*//**************************************************************************/

//INCLUDE
#include    "GameApp.h"
#include    "Resource.h"
#include    "Utilities/EditorUtilities.h"
#include    "Utilities/ToolIcon.h"
#include    <filesystem>
#include    <MofImGui/MofImGui.h>

#include    "Stage/Stage.h"

#include    "EditorParam/EditorParameter.h"
#include    "Menu/MainMenu.h"
#include    "Menu/ToolMenu.h"
#include    "Window/MapChipWindow.h"
#include    "Window/EditWindow.h"
#include    "Window/LayerWindow.h"
#include    "Manager/ImGuiWindowManager.h"
#include    "Manager/CommandManager.h"

bool                     edit_grid_flag = false;
bool                     chip_grid_flag = false;
Stage                    stage;
MapChipWindow            map_chip_window;
EditWindow               edit_window;
LayerWindow              layer_window;
MofU32                   edit_background_color = MOF_COLOR_CBLACK;
MofU32                   edit_font_color       = MOF_COLOR_WHITE;

bool                     write_mode_flag = true;

bool                     show_main_menu          = true;
bool                     show_tool_menu          = true;
bool                     show_layer_window       = true;
bool                     show_chip_window        = true;
bool                     show_chip_window_child  = true;
bool                     show_edit_window        = true;
bool                     show_edit_window_child  = true;

std::string              resource_path;
std::string              open_file = ".txt";

#include "Stage/Parser/TextParser.h"
std::function<void(void)> def_create;
CFont                     num_font;

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
        @brief            アプリケーションの初期化
        @param            None

        @return            TRUE        成功<br>
                        それ以外    失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {
    CUtilities::SetCurrentDirectory("Resource");

    ToolIcon::Load();
    def_create = default_create;
    resource_path = std::filesystem::current_path().string();

    CMofImGui::Setup(false, false);
    
    theParam.Register(ParamKey::MapChipArray       , stage.GetChipArrayPointer()       );
    theParam.Register(ParamKey::ChipGridFlag       , &chip_grid_flag                   );
    theParam.Register(ParamKey::EditGridFlag       , &edit_grid_flag                   );
    theParam.Register(ParamKey::EditBackColor      , &edit_background_color            );
    theParam.Register(ParamKey::MainMenu           , &show_main_menu                   );
    theParam.Register(ParamKey::ToolMenu           , &show_tool_menu                   );
    theParam.Register(ParamKey::LayerWindow        , &show_layer_window                );
    theParam.Register(ParamKey::ChipWindow         , &show_chip_window                 );
    theParam.Register(ParamKey::EditWindow         , &show_edit_window                 );
    theParam.Register(ParamKey::ChipWindowChild    , &show_chip_window_child           );
    theParam.Register(ParamKey::EditWindowChild    , &show_edit_window_child           );
    theParam.Register(ParamKey::ResourcePath       , &resource_path                    );
    theParam.Register(ParamKey::WriteMode          , &write_mode_flag                  );
    theParam.Register(ParamKey::MapChipTextureArray, stage.GetChipTextureArrayPointer());
    theParam.Register(ParamKey::BackgroundArray    , stage.GetBackgroundArrayPointer() );
    theParam.Register(ParamKey::OpenFile           , &open_file                        );
    theParam.Register(ParamKey::Stage              , &stage                            );
    theParam.Register(ParamKey::TextureArrays      , stage.GetTextureArraysPointer()   );
    theParam.Register(ParamKey::DefaultCreate      , &def_create                       );
    theParam.Register(ParamKey::NumFont            , &num_font                         );
    theParam.Register(ParamKey::EditFontColor      , &edit_font_color                  );

    num_font.Create();
    
    //open_file = "asaa";
    if (open_file != ".txt" && open_file.length()) {
        stage.Load(open_file);
    }
    else {
        default_create();
    }

    layer_window.Initialize();   /*  first
  */map_chip_window.Initialize();/*  second
  */edit_window.Initialize();    /*  third
  */

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
    CMofImGui::Refresh();
    EditorUtilities::HitAreaRefresh();
    if (std::optional<RECT> rect = EditorUtilities::ChangeWindowSize()) {

    }

    MainMenu::Show();
    ToolMenu::Show();

    layer_window.Show();
    map_chip_window.Show();
    edit_window.Show();
    
    if (!EditorUtilities::IsPopupModalOpen()) {
        map_chip_window.Update();
        edit_window.Update();
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
    if (g_pInput->IsKeyPush(MOFKEY_W)) {
        write_mode_flag = true;
    }
    if (g_pInput->IsKeyPush(MOFKEY_E)) {
        write_mode_flag = false;
    }

    //ImGui::ShowDemoWindow();

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

    //stage.Render();

    edit_window.Render();
    map_chip_window.Render();


    CMofImGui::RenderSetup();
    CMofImGui::RenderGui();

    //CGraphicsUtilities::RenderRect(EditorUtilities::GetChipArea(), MOF_COLOR_GREEN);
    //CGraphicsUtilities::RenderRect(EditorUtilities::GetEditArea(), MOF_COLOR_GREEN);
    //CGraphicsUtilities::RenderRect(*theImGuiWindowManager.Find(ParamKey::EditWindowChild), MOF_COLOR_GREEN);
    //float scale = *theParam.GetDataPointer<float>(ParamKey::ChipScale);
    //CGraphicsUtilities::RenderString(500, 300, "%.3f, %.3f, %d, %.3f", ImGui::GetMousePos().x, ImGui::GetMousePos().y, (int)(32 * scale), scale);

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

    EditorUtilities::SetCurrentPathResource();

    CMofImGui::Cleanup();

    ToolIcon::Release();

    stage.Release();

    num_font.Release();

    return TRUE;
}