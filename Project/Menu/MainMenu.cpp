#include "MainMenu.h"

// ********************************************************************************
/// <summary>
/// ファイルメニューの更新
/// </summary>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
void MainMenu::UpdateFileMenu() {
    //Menuの開始
    if (!ImGui::BeginMenu("File")) {
        return;
    }

    //Menuの終了
    ImGui::EndMenu();
}

// ********************************************************************************
/// <summary>
/// エディットメニューの更新
/// </summary>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
void MainMenu::UpdateEditMenu() {
    //Menuの開始
    if (!ImGui::BeginMenu("Edit")) {
        return;
    }

    //Menuの終了
    ImGui::EndMenu();
}

// ********************************************************************************
/// <summary>
/// ヴューメニューの更新
/// </summary>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
void MainMenu::UpdateViewMenu() {
    //Menuの開始
    if (!ImGui::BeginMenu("View")) {
        return;
    }

    if (ImGui::BeginMenu("shows")) {
        //SHOW FLAGS
        for (int i = 0; i < (int)ShowFlag::count; i++) {
            ImGui::Checkbox(show_flag_names[i], &show_flags[i]);
        }
        ImGui::EndMenu();
    }

    //Menuの終了
    ImGui::EndMenu();
}

// ********************************************************************************
/// <summary>
/// コンストラクタ
/// </summary>
// ********************************************************************************
MainMenu::MainMenu() {
    theBB_ImVec2.Register(BB_ImVec2::MainMenuPos , &_pos );
    theBB_ImVec2.Register(BB_ImVec2::MainMenuSize, &_size);
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
// ********************************************************************************
MainMenu::~MainMenu() {
}

// ********************************************************************************
/// <summary>
/// Menuの更新
/// </summary>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
void MainMenu::UpdateGui() {

    //Menuの開始
    if (!ImGui::BeginMainMenuBar()) {
        return;
    }
    //Menu座標、サイズの更新
    _pos  = ImGui::GetWindowPos();
    _size = ImGui::GetWindowSize();

    //各Menu処理
    UpdateFileMenu();
    UpdateEditMenu();
    UpdateViewMenu();

    //Menuの終了
    ImGui::EndMainMenuBar();
}

// ********************************************************************************
/// <summary>
/// Getter
/// </summary>
// ********************************************************************************

// ********************************************************************************
/// <summary>
/// 座標の取得
/// </summary>
/// <returns>Menu表示座標</returns>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
const ImVec2 MainMenu::GetPos() const {
    return _pos;
}

// ********************************************************************************
/// <summary>
/// サイズの取得
/// </summary>
/// <returns>Menu表示サイズ</returns>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
const ImVec2 MainMenu::GetSize() const {
    return _size;
}
