#pragma once
#include <Windows.h>
#include <string>
#include <optional>
#include <vector>

// ********************************************************************************
/// <summary>
/// Version.rc�̓����f�[�^�擾�p�\����
/// </summary>
// ********************************************************************************
struct VersionResourceData {
    std::string CompanyName;
    std::string FileDescription;
    std::string FileVersion;
    std::string InternalName;
    std::string LegalCopyright;
    std::string OriginalFileName;
    std::string ProductName;
    std::string ProductVersion;
};

// ********************************************************************************
/// <summary>
/// �G�f�B�^�ėp�N���X
/// </summary>
// ********************************************************************************
class EditorUtilities {
private:

    //static class
    EditorUtilities() = delete;
    ~EditorUtilities() = delete;

    // ********************************************************************************
    /// <summary>
    /// �E�B���h�E�̋�`�擾
    /// </summary>
    /// <returns>�E�B���h�E�̋�`</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static RECT& GetWindowRect(void);

public:

    // ********************************************************************************
    /// <summary>
    /// Version.rc�̃f�[�^�擾
    /// </summary>
    /// <returns>Version.rc�̃f�[�^, nullopt : ���s</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::optional<VersionResourceData> GetVersionResourceData(void);

    // ********************************************************************************
    /// <summary>
    /// �E�B���h�E�̃T�C�Y�ύX��ʒm����
    /// </summary>
    /// <returns>RECT : �ύX��̃E�B���h�ERECT, nullopt : �ύX�Ȃ�</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::optional<RECT> ChangeWindowSize(void);

    // ********************************************************************************
    /// <summary>
    /// �t�@�C���p�X�̎擾
    /// </summary>
    /// <param name="dir">�w��f�B���N�g��</param>
    /// <param name="file">�t�@�C����</param>
    /// <returns>�t�@�C���p�X, "" : �������s</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::string GetFilePath(const std::string& dir, const std::string& file);

    // ********************************************************************************
    /// <summary>
    /// �t�@�C���g���q�̎擾
    /// </summary>
    /// <param name="path">�t�@�C���p�X</param>
    /// <returns>.���܂ފg���q</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::string GetExt(const std::string& path);

    // ********************************************************************************
    /// <summary>
    /// �t�@�C�����̎擾
    /// </summary>
    /// <param name="path">�t�@�C���p�X</param>
    /// <returns>�t�@�C����</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::string GetFileName(const std::string& path);

    // ********************************************************************************
    /// <summary>
    /// ���O�̏d������
    /// </summary>
    /// <param name="name">�������閼�O</param>
    /// <param name="names">�m�F���閼�O�z��</param>
    /// <returns>�d�����Ȃ����O</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::string SerchDuplicateName(const std::string& name, const std::vector<std::string>& names);

    // ********************************************************************************
    /// <summary>
    /// Default�t�H���_�̃p�X�擾
    /// </summary>
    /// <returns>Default�t�H���_�̃p�X</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::string GetDefaultPath(void);

    // ********************************************************************************
    /// <summary>
    /// �J�����g�p�X��Default�t�H���_�ɐݒ�
    /// </summary>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static void SetCurrentPathDefault(void);

    // ********************************************************************************
    /// <summary>
    /// Texture�t�@�C�����J���_�C�A���O���J��
    /// </summary>
    /// <param name="title">�_�C�A���O�^�C�g��</param>
    /// <param name="path">�ǂݍ��񂾃t�@�C���p�X</param>
    /// <param name="array_flag">�����t�@�C���ǂݍ��񂾂��̃t���O</param>
    /// <returns>true : �ǂݍ��ݐ���, false : �ǂݍ��ݎ��s</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static bool OpenTextureFileDialog(const std::string& title, char* path, bool* array_flag = nullptr);

    // ********************************************************************************
    /// <summary>
    /// ���݂̎����𕶎���Ŏ擾
    /// </summary>
    /// <returns>���݂̎����̕�����</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::string GetDatetimeStr(void);
};

