#include "MofImGui.h"
#include <filesystem>
#include <functional>
#include <Windows.h>
#include <atlstr.h>
#pragma comment(lib, "version.lib")

#include "FileDialog.h"
#include "EditorUtilities.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ********************************************************************************
CEditorUtilities::CEditorUtilities(void) 
{
}

// ********************************************************************************
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ********************************************************************************
CEditorUtilities::~CEditorUtilities(void) 
{
}

// ********************************************************************************
/// <summary>
/// �o�[�W�������擾
/// </summary>
/// <returns>�o�[�W�������, none : ���s</returns>
// ********************************************************************************
std::optional<VersionResourceData> CEditorUtilities::GetVersionResourceData(void)
{
    static std::optional<VersionResourceData> instance = std::nullopt;
    if (instance)
    {
        return instance;
    }
    instance = VersionResourceData();
    TCHAR             fileName[MAX_PATH + 1];
    DWORD             size;
    BYTE*             pVersion;
    UINT              queryLen;
    ATL::CString      fileVersion;
    ATL::CString      subBlock;
    WORD*             pLangCode;
    TCHAR*            pStrInfo;
    ::GetModuleFileName(NULL, fileName, sizeof(fileName));
    size     = ::GetFileVersionInfoSize(fileName, NULL);
    pVersion = new BYTE[size];
    if (::GetFileVersionInfo(fileName, NULL, size, pVersion))
    {
        ::VerQueryValue(pVersion, _T("\\VarFileInfo\\Translation"), (void**)&pLangCode, &queryLen);
        std::function<std::string(LPCSTR)> GetFormatData = [&](LPCSTR str)
        {
            subBlock.Format(str, pLangCode[0], pLangCode[1]);
            ::VerQueryValue(pVersion, (LPTSTR)(LPCTSTR)subBlock, (void**)&pStrInfo, &queryLen);
            if (0 < queryLen) return std::string(pStrInfo);
            return std::string("");
        };
        instance->CompanyName      = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\CompanyName"     ));
        instance->FileDescription  = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\FileDescription" ));
        instance->FileVersion      = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\FileVersion"     ));
        instance->InternalName     = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\InternalName"    ));
        instance->LegalCopyright   = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\LegalCopyright"  ));
        instance->OriginalFileName = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\OriginalFileName"));
        instance->ProductName      = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\ProductName"     ));
        instance->ProductVersion   = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\ProductVersion"  ));
    }
    else
    {
        instance = std::nullopt;
    }
    delete[] pVersion;
    return instance;
}

// ********************************************************************************
/// <summary>
/// ���O�d���������A���Ԃ�Ȃ����O��Ԃ�
/// </summary>
/// <param name="name">�d�����Ă��邩��������l</param>
/// <param name="names">�������������X�g</param>
/// <returns>���Ԃ�Ȃ����O</returns>
// ********************************************************************************
std::string CEditorUtilities::SerchDuplicateName(const std::string& name, const std::vector<std::string>& names)
{
    std::function<bool(std::string)> checkhit =
        [&](std::string s)
    {
        for (auto& it : names)
        {
            if (it != s) 
            {
                continue;
            }
            return true;
        }
        return false;
    };
    if (!checkhit(name))
    {
        return name;
    }
    // ���O�����Ԃ����ꍇ�̔ԍ�
    int count = 1;
    std::string str;
    do
    {
        str = name + std::to_string(count++);
    } while (checkhit(str));
    return str;
}

// ********************************************************************************
/// <summary>
/// �w��f�B���N�g���ȉ�����t�@�C����T��
/// </summary>
/// <param name="dir">�����f�B���N�g��</param>
/// <param name="file">�����t�@�C����</param>
/// <returns>�t�@�C���p�X, "" : ������Ȃ�����</returns>
// ********************************************************************************
std::string CEditorUtilities::GetFilePath(const std::string& dir, const std::string& file)
{
    std::function<std::string(std::filesystem::path, const std::string&)> f =
        [&](std::filesystem::path s, const std::string& n)
    {
        for (auto& it : std::filesystem::recursive_directory_iterator(s))
        {
            if (std::filesystem::is_directory(it))
            {
                f(it, n);
            }
            else
            {
                std::string filename = GetFileName(it.path().u8string()) + GetExt(it.path().u8string());
                if (file == filename)
                {
                    return it.path().u8string();
                }
            }
        }
        return std::string("");
    };

    return f(dir, file);
}

// ********************************************************************************
/// <summary>
/// �g���q�̎擾
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
/// <returns>�g���q</returns>
// ********************************************************************************
std::string CEditorUtilities::GetExt(const std::string& path)
{
    std::string ext    = path;
    int         length = ext.find_last_of('.');
    ext = ext.substr(length);
    return ext;
}

// ********************************************************************************
/// <summary>
/// �t�@�C�����̎擾
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
/// <returns>�t�@�C����</returns>
// ********************************************************************************
std::string CEditorUtilities::GetFileName(const std::string& path)
{
    std::string file   = path;
    int         length = file.find_last_of('.');
    int         start  = file.find_last_of('/') + 1;
    if (start <= 0)
    {
        start = file.find_last_of('\\') + 1;
    }
    file = file.substr(start, length - start);
    return file;
}

// ********************************************************************************
/// <summary>
/// ImGui�̃J���[�s�b�J�[��MofU32�J���[��ҏW����
/// </summary>
/// <param name="color">�ҏW����J���[</param>
/// <returns>true : �ύX����, false : �ύX�Ȃ�</returns>
// ********************************************************************************
bool CEditorUtilities::ColorPicker(MofU32& color)
{
    Vector4 col;
    CVector4Utilities::SetU32Color(color, col);
    if (ImGui::ColorPicker4("##EditorColorPicker", col.fv))
    {
        color = CVector4Utilities::ToU32Color(col);
        return true;
    }
    return false;
}

// ********************************************************************************
/// <summary>
/// ���݂̎��Ԃ𕶎���Ŏ擾����
/// </summary>
/// <returns>���݂̎���</returns>
// ********************************************************************************
std::string CEditorUtilities::GetDatetimeStr(void)
{
    time_t t = time(nullptr);
    const tm* localTime = localtime(&t);
    std::stringstream s;
    s << "20" << localTime->tm_year - 100;
    // setw(),setfill()��0�l��
    s << std::setw(2) << std::setfill('0') << localTime->tm_mon + 1;
    s << std::setw(2) << std::setfill('0') << localTime->tm_mday;
    s << std::setw(2) << std::setfill('0') << localTime->tm_hour;
    s << std::setw(2) << std::setfill('0') << localTime->tm_min;
    s << std::setw(2) << std::setfill('0') << localTime->tm_sec;
    // std::string�ɂ��Ēl��Ԃ�
    return s.str();
}