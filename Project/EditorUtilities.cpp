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
/// コンストラクタ
/// </summary>
// ********************************************************************************
CEditorUtilities::CEditorUtilities(void) 
{
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
// ********************************************************************************
CEditorUtilities::~CEditorUtilities(void) 
{
}

// ********************************************************************************
/// <summary>
/// バージョン情報取得
/// </summary>
/// <returns>バージョン情報, none : 失敗</returns>
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
/// 名前重複検索し、かぶらない名前を返す
/// </summary>
/// <param name="name">重複しているか検索する値</param>
/// <param name="names">検索したいリスト</param>
/// <returns>かぶらない名前</returns>
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
    // 名前がかぶった場合の番号
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
/// 指定ディレクトリ以下からファイルを探す
/// </summary>
/// <param name="dir">検索ディレクトリ</param>
/// <param name="file">検索ファイル名</param>
/// <returns>ファイルパス, "" : 見つからなかった</returns>
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
/// 拡張子の取得
/// </summary>
/// <param name="path">ファイルパス</param>
/// <returns>拡張子</returns>
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
/// ファイル名の取得
/// </summary>
/// <param name="path">ファイルパス</param>
/// <returns>ファイル名</returns>
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
/// ImGuiのカラーピッカーでMofU32カラーを編集する
/// </summary>
/// <param name="color">編集するカラー</param>
/// <returns>true : 変更あり, false : 変更なし</returns>
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
/// 現在の時間を文字列で取得する
/// </summary>
/// <returns>現在の時間</returns>
// ********************************************************************************
std::string CEditorUtilities::GetDatetimeStr(void)
{
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