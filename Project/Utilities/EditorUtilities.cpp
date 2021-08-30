#include <Mof.h>
#include "EditorUtilities.h"
#include "../EditorCommon.h"
#include "FileDialog.h"
#include <atlstr.h>
#include <functional>
#include <filesystem>
#include <sstream>
#pragma comment(lib, "version.lib")

// ********************************************************************************
/// <summary>
/// �E�B���h�E�̋�`�擾
/// </summary>
/// <returns>�E�B���h�E�̋�`</returns>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
RECT& EditorUtilities::GetWindowRect(void) {
    static RECT rect;
    return rect;
}

// ********************************************************************************
/// <summary>
/// Version.rc�̃f�[�^�擾
/// </summary>
/// <returns>Version.rc�̃f�[�^, nullopt : ���s</returns>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
std::optional<VersionResourceData> EditorUtilities::GetVersionResourceData(void) {
    static std::optional<VersionResourceData> instance = std::nullopt;
    if (instance) {
        return instance;
    }
    instance = VersionResourceData();
    TCHAR        fileName[MAX_PATH + 1];
    DWORD        size;
    BYTE*        pVersion;
    UINT         queryLen;
    ATL::CString fileVersion;
    ATL::CString subBlock;
    WORD*        pLangCode;
    TCHAR*       pStrInfo;
    ::GetModuleFileName(NULL, fileName, sizeof(fileName));
    size     = ::GetFileVersionInfoSize(fileName, NULL);
    pVersion = new BYTE[size];
    if (::GetFileVersionInfo(fileName, NULL, size, pVersion)) {
        ::VerQueryValue(pVersion, _T("\\VarFileInfo\\Translation"), (void**)&pLangCode, &queryLen);
        std::function<std::string(LPCSTR)> GetFormatData = [&](LPCSTR str) {
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
    else {
        instance = std::nullopt;
    }
    delete[] pVersion;
    return instance;
}

// ********************************************************************************
/// <summary>
/// �E�B���h�E�̃T�C�Y�ύX��ʒm����
/// </summary>
/// <returns>RECT : �ύX��̃E�B���h�ERECT, nullopt : �ύX�Ȃ�</returns>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
std::optional<RECT> EditorUtilities::ChangeWindowSize(void) {
    RECT rect, pos;
    HWND hWnd = g_pMainWindow->GetWindowHandle();
    ::GetWindowRect(hWnd, &pos);
    ::GetClientRect(hWnd, &rect);
    if (rect.right <= 0) {
        rect.right = 1;
    }
    if (rect.bottom <= 0) {
        rect.bottom = 1;
    }
    int prev_window_width = g_pGraphics->GetTargetWidth();
    int prev_window_height = g_pGraphics->GetTargetHeight();
    int now_width = rect.right;
    int now_height = rect.bottom;
    if (pos.left != EditorUtilities::GetWindowRect().left ||
        pos.top != EditorUtilities::GetWindowRect().top) {
        EditorUtilities::GetWindowRect().left = pos.left;
        EditorUtilities::GetWindowRect().top  = pos.top;
    }
    if (prev_window_width != now_width ||
        prev_window_height != now_height) {
        g_pGraphics->SetScreenSize(now_width, now_height);
        g_pGraphics->SetBackBufferSize(now_width, now_height);
        return rect;
    }
    return std::nullopt;
}

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
std::string EditorUtilities::GetFilePath(const std::string& dir, const std::string& file) {
    std::function<std::string(std::filesystem::path, const std::string&)> f =
        [&](std::filesystem::path s, const std::string& n) {
        for (auto& it : std::filesystem::recursive_directory_iterator(s)) {
            if (std::filesystem::is_directory(it)) {
                f(it, n);
            }
            else {
                std::string filename = GetFileName(it.path().u8string()) + GetExt(it.path().u8string());
                if (file == filename) {
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
/// �t�@�C���g���q�̎擾
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
/// <returns>.���܂ފg���q</returns>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
std::string EditorUtilities::GetExt(const std::string& path) {
    auto ext    = path;
    int  length = ext.find_last_of('.');
    ext = ext.substr(length);
    return ext;
}

// ********************************************************************************
/// <summary>
/// �t�@�C�����̎擾
/// </summary>
/// <param name="path">�t�@�C���p�X</param>
/// <returns>�t�@�C����</returns>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
std::string EditorUtilities::GetFileName(const std::string& path) {
    auto file   = path;
    int  length = file.find_last_of('.');
    int  start  = file.find_last_of('/') + 1;
    if (start <= 0) {
        start = file.find_last_of('\\') + 1;
    }
    file = file.substr(start, length - start);
    return file;
}

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
std::string EditorUtilities::SerchDuplicateName(const std::string& name, const std::vector<std::string>& names) {
    std::function<bool(std::string)> checkhit =
        [&](std::string s) {
        for (auto& it : names) {
            if (it != s) {
                continue;
            }
            return true;
        }
        return false;
    };
    if (!checkhit(name)) {
        return name;
    }
    // ���O�����Ԃ����ꍇ�̔ԍ�
    int count = 1;
    std::string str;
    do {
        str = name + std::to_string(count++);
    } while (checkhit(str));
    return str;
}

// ********************************************************************************
/// <summary>
/// Default�t�H���_�̃p�X�擾
/// </summary>
/// <returns>Resource�t�H���_�̃p�X</returns>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
std::string EditorUtilities::GetDefaultPath(void) {
    return *theBB_string.GetData(BB_string::DefaultPath);
}

// ********************************************************************************
/// <summary>
/// �J�����g�p�X��Default�t�H���_�ɐݒ�
/// </summary>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
void EditorUtilities::SetCurrentPathDefault(void) {
    CUtilities::SetCurrentDirectory(GetDefaultPath().c_str());
}

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
bool EditorUtilities::OpenTextureFileDialog(const std::string& title, char* path, bool* array_flag) {
    bool b = false;
    return FileDialog::Open(g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Open,
        title.c_str(),
        "�摜 �t�@�C��\0*.png;*.bmp;*.dds\0all file(*.*)\0*.*\0\0",
        "png\0bmp\0dds", path, (array_flag == nullptr ? b : *array_flag));
}

// ********************************************************************************
/// <summary>
/// ���݂̎����𕶎���Ŏ擾
/// </summary>
/// <returns>���݂̎����̕�����</returns>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
std::string EditorUtilities::GetDatetimeStr(void) {
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
