/*************************************************************************//*!

                    @file    GameApp.cpp
                    @brief    ��{�Q�[���A�v���B

                                                            @author    �_�c�@��
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

bool                     edit_grid_flag = false;
bool                     chip_grid_flag = false;
Stage                    stage;
MapChipWindow            map_chip_window;
EditWindow               edit_window;
LayerWindow              layer_window;
MofU32                   edit_background_color = MOF_COLOR_CBLACK;

bool                     write_mode_flag = true;

bool                     show_main_menu          = true;
bool                     show_tool_menu          = true;
bool                     show_layer_window       = true;
bool                     show_chip_window        = true;
bool                     show_chip_window_child  = true;
bool                     show_edit_window        = true;
bool                     show_edit_window_child  = true;

std::string              resource_path;
std::string              open_file;

#include "Stage/Parser/TextParser.h"

/*************************************************************************//*!
        @brief            �A�v���P�[�V�����̏�����
        @param            None

        @return            TRUE        ����<br>
                        ����ȊO    ���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {
    CUtilities::SetCurrentDirectory("Resource");

    ToolIcon::Load();

    resource_path = std::filesystem::current_path().string();

    CMofImGui::Setup();
    
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

    //open_file = "asaa";
    if (open_file.length()) {
        stage.Load(open_file);
    }

    layer_window.Initialize();   /*  first
  */map_chip_window.Initialize();/*  second
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

    //stage.Render();

    edit_window.Render();
    map_chip_window.Render();


    CMofImGui::RenderSetup();
    CMofImGui::RenderGui();

    //CGraphicsUtilities::RenderRect(EditorUtilities::GetChipArea(), MOF_COLOR_GREEN);
    //CGraphicsUtilities::RenderRect(EditorUtilities::GetEditArea(), MOF_COLOR_GREEN);
    //CGraphicsUtilities::RenderRect(*theImGuiWindowManager.Find(ParamKey::ChipWindow), MOF_COLOR_GREEN);

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

    CMofImGui::Cleanup();

    ToolIcon::Release();

    stage.Release();

    return TRUE;
}