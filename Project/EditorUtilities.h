#pragma once
#include <Mof.h>

#include <string>
#include <optional>

#include <sstream>
#include <iomanip>

#include "Singleton.h"

namespace Editor
{
    // ********************************************************************************
    /// <summary>
    /// �o�[�W�����f�[�^���
    /// </summary>
    // ********************************************************************************
    struct VersionResourceData
    {
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
    /// �G�f�B�^�[�p�ėp�N���X
    /// </summary>
    // ********************************************************************************
    class CEditorUtilities : public CSingleton<CEditorUtilities>
    {
        friend class CSingleton<CEditorUtilities>;

    public:

        // ********************************************************************************
        /// <summary>
        /// �R���X�g���N�^
        /// </summary>
        // ********************************************************************************
        CEditorUtilities(void);

        // ********************************************************************************
        /// <summary>
        /// �f�X�g���N�^
        /// </summary>
        // ********************************************************************************
        ~CEditorUtilities(void);

        // ********************************************************************************
        /// <summary>
        /// �o�[�W�������擾
        /// </summary>
        /// <returns>�o�[�W�������, none : ���s</returns>
        // ********************************************************************************
        std::optional<VersionResourceData> GetVersionResourceData(void);

        // ********************************************************************************
        /// <summary>
        /// ���O�d���������A���Ԃ�Ȃ����O��Ԃ�
        /// </summary>
        /// <param name="name">�d�����Ă��邩��������l</param>
        /// <param name="names">�������������X�g</param>
        /// <returns>���Ԃ�Ȃ����O</returns>
        // ********************************************************************************
        std::string SerchDuplicateName(const std::string& name, const std::vector<std::string>& names);

        // ********************************************************************************
        /// <summary>
        /// �w��f�B���N�g���ȉ�����t�@�C����T��
        /// </summary>
        /// <param name="dir">�����f�B���N�g��</param>
        /// <param name="file">�����t�@�C����</param>
        /// <returns>�t�@�C���p�X, "" : ������Ȃ�����</returns>
        // ********************************************************************************
        std::string GetFilePath(const std::string& dir, const std::string& file);

        // ********************************************************************************
        /// <summary>
        /// �g���q�̎擾
        /// </summary>
        /// <param name="path">�t�@�C���p�X</param>
        /// <returns>�g���q</returns>
        // ********************************************************************************
        std::string GetExt(const std::string& path);

        // ********************************************************************************
        /// <summary>
        /// �t�@�C�����̎擾
        /// </summary>
        /// <param name="path">�t�@�C���p�X</param>
        /// <returns>�t�@�C����</returns>
        // ********************************************************************************
        std::string GetFileName(const std::string& path);

        // ********************************************************************************
        /// <summary>
        /// ImGui�̃J���[�s�b�J�[��MofU32�J���[��ҏW����
        /// </summary>
        /// <param name="color">�ҏW����J���[</param>
        /// <returns>true : �ύX����, false : �ύX�Ȃ�</returns>
        // ********************************************************************************
        bool ColorPicker(MofU32& color);

        // ********************************************************************************
        /// <summary>
        /// ���݂̎��Ԃ𕶎���Ŏ擾����
        /// </summary>
        /// <returns>���݂̎���</returns>
        // ********************************************************************************
        std::string GetDatetimeStr(void);
    };
}
