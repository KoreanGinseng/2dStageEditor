#pragma once
#include "../EditorCommon.h"

// ********************************************************************************
/// <summary>
/// �c�[���o�[�N���X
/// </summary>
// ********************************************************************************
class ToolBar {
private:

    ImVec2 _pos{ 0.0f, 0.0f };  //!<���W
    ImVec2 _size{ 0.0f, 0.0f }; //!<�T�C�Y

public:

    // ********************************************************************************
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    ToolBar();
    
    // ********************************************************************************
    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    ~ToolBar();

    // ********************************************************************************
    /// <summary>
    /// �c�[���o�[�̍X�V
    /// </summary>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    void UpdateGui();
};
