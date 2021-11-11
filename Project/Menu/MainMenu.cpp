#include "MainMenu.h"

// ********************************************************************************
/// <summary>
/// �t�@�C�����j���[�̍X�V
/// </summary>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
void MainMenu::UpdateFileMenu() {
    //Menu�̊J�n
    if (!ImGui::BeginMenu("File")) {
        return;
    }

    //Menu�̏I��
    ImGui::EndMenu();
}

// ********************************************************************************
/// <summary>
/// �G�f�B�b�g���j���[�̍X�V
/// </summary>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
void MainMenu::UpdateEditMenu() {
    //Menu�̊J�n
    if (!ImGui::BeginMenu("Edit")) {
        return;
    }

    //Menu�̏I��
    ImGui::EndMenu();
}

// ********************************************************************************
/// <summary>
/// �����[���j���[�̍X�V
/// </summary>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
void MainMenu::UpdateViewMenu() {
    //Menu�̊J�n
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

    //Menu�̏I��
    ImGui::EndMenu();
}

// ********************************************************************************
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ********************************************************************************
MainMenu::MainMenu() {
    theBB_ImVec2.Register(BB_ImVec2::MainMenuPos , &_pos );
    theBB_ImVec2.Register(BB_ImVec2::MainMenuSize, &_size);
}

// ********************************************************************************
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ********************************************************************************
MainMenu::~MainMenu() {
}

// ********************************************************************************
/// <summary>
/// Menu�̍X�V
/// </summary>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
void MainMenu::UpdateGui() {

    //Menu�̊J�n
    if (!ImGui::BeginMainMenuBar()) {
        return;
    }
    //Menu���W�A�T�C�Y�̍X�V
    _pos  = ImGui::GetWindowPos();
    _size = ImGui::GetWindowSize();

    //�eMenu����
    UpdateFileMenu();
    UpdateEditMenu();
    UpdateViewMenu();

    //Menu�̏I��
    ImGui::EndMainMenuBar();
}

// ********************************************************************************
/// <summary>
/// Getter
/// </summary>
// ********************************************************************************

// ********************************************************************************
/// <summary>
/// ���W�̎擾
/// </summary>
/// <returns>Menu�\�����W</returns>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
const ImVec2 MainMenu::GetPos() const {
    return _pos;
}

// ********************************************************************************
/// <summary>
/// �T�C�Y�̎擾
/// </summary>
/// <returns>Menu�\���T�C�Y</returns>
/// <created>inoue,2021/08/26</created>
/// <changed>inoue,2021/08/26</changed>
// ********************************************************************************
const ImVec2 MainMenu::GetSize() const {
    return _size;
}
