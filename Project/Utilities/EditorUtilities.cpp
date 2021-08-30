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
/// ウィンドウの矩形取得
/// </summary>
/// <returns>ウィンドウの矩形</returns>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
RECT& EditorUtilities::GetWindowRect(void) {
    static RECT rect;
    return rect;
}

// ********************************************************************************
/// <summary>
/// Version.rcのデータ取得
/// </summary>
/// <returns>Version.rcのデータ, nullopt : 失敗</returns>
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
/// ウィンドウのサイズ変更を通知する
/// </summary>
/// <returns>RECT : 変更後のウィンドウRECT, nullopt : 変更なし</returns>
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
/// ファイルパスの取得
/// </summary>
/// <param name="dir">指定ディレクトリ</param>
/// <param name="file">ファイル名</param>
/// <returns>ファイルパス, "" : 検索失敗</returns>
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
/// ファイル拡張子の取得
/// </summary>
/// <param name="path">ファイルパス</param>
/// <returns>.を含む拡張子</returns>
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
/// ファイル名の取得
/// </summary>
/// <param name="path">ファイルパス</param>
/// <returns>ファイル名</returns>
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
/// 名前の重複検索
/// </summary>
/// <param name="name">検索する名前</param>
/// <param name="names">確認する名前配列</param>
/// <returns>重複しない名前</returns>
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
    // 名前がかぶった場合の番号
    int count = 1;
    std::string str;
    do {
        str = name + std::to_string(count++);
    } while (checkhit(str));
    return str;
}

// ********************************************************************************
/// <summary>
/// Defaultフォルダのパス取得
/// </summary>
/// <returns>Resourceフォルダのパス</returns>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
std::string EditorUtilities::GetDefaultPath(void) {
    return *theBB_string.GetData(BB_string::DefaultPath);
}

// ********************************************************************************
/// <summary>
/// カレントパスをDefaultフォルダに設定
/// </summary>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
void EditorUtilities::SetCurrentPathDefault(void) {
    CUtilities::SetCurrentDirectory(GetDefaultPath().c_str());
}

// ********************************************************************************
/// <summary>
/// Textureファイルを開くダイアログを開く
/// </summary>
/// <param name="title">ダイアログタイトル</param>
/// <param name="path">読み込んだファイルパス</param>
/// <param name="array_flag">複数ファイル読み込んだかのフラグ</param>
/// <returns>true : 読み込み成功, false : 読み込み失敗</returns>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
bool EditorUtilities::OpenTextureFileDialog(const std::string& title, char* path, bool* array_flag) {
    bool b = false;
    return FileDialog::Open(g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Open,
        title.c_str(),
        "画像 ファイル\0*.png;*.bmp;*.dds\0all file(*.*)\0*.*\0\0",
        "png\0bmp\0dds", path, (array_flag == nullptr ? b : *array_flag));
}

// ********************************************************************************
/// <summary>
/// 現在の時刻を文字列で取得
/// </summary>
/// <returns>現在の時刻の文字列</returns>
/// <created>inoue,2021/08/27</created>
/// <changed>inoue,2021/08/27</changed>
// ********************************************************************************
std::string EditorUtilities::GetDatetimeStr(void) {
    time_t t = time(nullptr);
    const tm* localTime = localtime(&t);
    std::stringstream s;
    s << "20" << localTime->tm_year - 100;
    // setw(),setfill()で0詰め
    s << std::setw(2) << std::setfill('0') << localTime->tm_mon + 1;
    s << std::setw(2) << std::setfill('0') << localTime->tm_mday;
    s << std::setw(2) << std::setfill('0') << localTime->tm_hour;
    s << std::setw(2) << std::setfill('0') << localTime->tm_min;
    s << std::setw(2) << std::setfill('0') << localTime->tm_sec;
    // std::stringにして値を返す
    return s.str();
}
