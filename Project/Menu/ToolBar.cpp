#include "ToolBar.h"

// ********************************************************************************
/// <summary>
/// コンストラクタ
/// </summary>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
ToolBar::ToolBar() {
    theBB_ImVec2.Register(BB_ImVec2::ToolBarPos , &_pos );
    theBB_ImVec2.Register(BB_ImVec2::ToolBarSize, &_size);
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
ToolBar::~ToolBar() {
}

// ********************************************************************************
/// <summary>
/// ツールバーの更新
/// </summary>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
void ToolBar::UpdateGui() {
    //MainMenuの座標とサイズを取得
    auto main_menu_pos  = theBB_ImVec2.GetData(BB_ImVec2::MainMenuPos );
    auto main_menu_size = theBB_ImVec2.GetData(BB_ImVec2::MainMenuSize);
    if (!main_menu_pos || !main_menu_size) {
        MOF_PRINTLOG("mainmenu not found.\n");
        return;
    }

    //ToolBarの表示位置とサイズを固定
    int toolbar_flag = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;
    ImGui::SetNextWindowPos(ImVec2(0, main_menu_pos->y + main_menu_size->y));
    ImGui::SetNextWindowSize(ImVec2(g_pGraphics->GetTargetWidth() * 1.0f, g_pGraphics->GetTargetHeight() * 0.03f));
    //スタイルの変更
    ImGui::SetNextWindowBgAlpha(1);
    const ImVec2 def_window_padding = ImGui::GetStyle().WindowPadding;
    const ImVec2 def_item_spacing   = ImGui::GetStyle().ItemSpacing;
    ImGui::GetStyle().WindowPadding = ImVec2(2, 2);
    ImGui::GetStyle().ItemSpacing   = ImVec2(2, 2);
    //ToolBarの表示
    ImGui::Begin("##ToolBar", NULL, toolbar_flag); {
        //ToolBarの座標とサイズを更新
        _pos  = ImGui::GetWindowPos();
        _size = ImGui::GetWindowSize();


        ImVec2 size = ImVec2(22, 22);
        ImVec2 uv0  = ImVec2( 0,  0);
        ImVec2 uv1  = ImVec2( 1,  1);
        ImGui::Dummy(ImVec2(size.x * 0.5f, size.y)); ImGui::SameLine();
        if (ImGui::ImageButton(theBB_InResourceTexture.GetData(BB_InResourceTexture::new_document)->GetTexture(), size)) {
            //MainMenu::NewProject();
        }ImGui::SameLine();
        if (ImGui::ImageButton(theBB_InResourceTexture.GetData(BB_InResourceTexture::folder)->GetTexture(), size)) {
            //MainMenu::OpenProject();
        }ImGui::SameLine();
        if (ImGui::ImageButton(theBB_InResourceTexture.GetData(BB_InResourceTexture::diskette)->GetTexture(), size)) {
            //MainMenu::SaveProject();
        }ImGui::SameLine();
        ImGui::Dummy(ImVec2(size.x * 2.5f, size.y)); ImGui::SameLine();
        if (ImGui::ImageButton(theBB_InResourceTexture.GetData(BB_InResourceTexture::back_arrow)->GetTexture(), size)) {
            //theCommandManager.Undo();
        }ImGui::SameLine();
        if (ImGui::ImageButton(theBB_InResourceTexture.GetData(BB_InResourceTexture::redo_arrow)->GetTexture(), size)) {
            //theCommandManager.Redo();
        }ImGui::SameLine();
        ImGui::Dummy(ImVec2(size.x * 3.5f, size.y)); ImGui::SameLine();
        ImVec4 on_color     = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
        ImVec4 off_color    = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        ImVec4 def_color    = ImGui::GetStyle().Colors[ImGuiCol_Button];
        bool is_write       = (editor_mode == EditorMode::write);
        bool is_erase       = (editor_mode == EditorMode::erase);
        bool is_select      = (editor_mode == EditorMode::select);
        bool is_layout      = (editor_mode == EditorMode::layout);
        ImVec4 write_color  = (is_write  ? on_color : off_color);
        ImVec4 erase_color  = (is_erase  ? on_color : off_color);
        ImVec4 select_color = (is_select ? on_color : off_color);
        ImVec4 layout_color = (is_layout ? on_color : off_color);
        ImGui::GetStyle().Colors[ImGuiCol_Button] = (is_write ? write_color : def_color);
        if (ImGui::ImageButton(theBB_InResourceTexture.GetData(BB_InResourceTexture::edit)->GetTexture(), size, uv0, uv1, -1, write_color)) {
            editor_mode = EditorMode::write;
        }ImGui::SameLine();
        ImGui::GetStyle().Colors[ImGuiCol_Button] = (is_erase ? erase_color : def_color);
        if (ImGui::ImageButton(theBB_InResourceTexture.GetData(BB_InResourceTexture::eraser_tool)->GetTexture(), size, uv0, uv1, -1, erase_color)) {
            editor_mode = EditorMode::erase;
        }ImGui::SameLine();
        ImGui::GetStyle().Colors[ImGuiCol_Button] = (is_select ? select_color : def_color);
        if (ImGui::ImageButton(theBB_InResourceTexture.GetData(BB_InResourceTexture::selection)->GetTexture(), size, uv0, uv1, -1, select_color)) {
            editor_mode = EditorMode::select;
        }ImGui::SameLine();
        ImGui::GetStyle().Colors[ImGuiCol_Button] = (is_layout ? layout_color : def_color);
        if (ImGui::ImageButton(theBB_InResourceTexture.GetData(BB_InResourceTexture::layout)->GetTexture(), size, uv0, uv1, -1, layout_color)) {
            editor_mode = EditorMode::layout;
        }ImGui::SameLine();
        ImGui::GetStyle().Colors[ImGuiCol_Button] = def_color;
    }
    ImGui::GetStyle().WindowPadding = def_window_padding;
    ImGui::GetStyle().ItemSpacing   = def_item_spacing;
    ImGui::End();
}
