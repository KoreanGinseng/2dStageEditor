#include "FileDialog.h"
#include <filesystem>

// ********************************************************************************
/// <summary>
/// ファイルダイアログの開始
/// </summary>
/// <param name="hWnd">ハンドル</param>
/// <param name="mode">ダイアログのモード</param>
/// <param name="Title">ダイアログのタイトル</param>
/// <param name="Filter">開くファイルのフィルタ</param>
/// <param name="DefExt">開くファイルの拡張子</param>
/// <param name="path">パスの出力先</param>
/// <param name="bOutArray">複数ファイルが選択されているかのフラグ出力先</param>
/// <returns></returns>
/// <created>いのうえ,2020/11/11</created>
/// <changed>いのうえ,2020/11/11</changed>
// ********************************************************************************
bool FileDialog::Open(HWND hWnd, Mode mode, const char* Title, const char* Filter, const char* DefExt, char* path, bool& bOutArray) {
    //ファイルダイアログ用構造体
    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner   = hWnd;
    ofn.lpstrFile   = path;
    ofn.lpstrFilter = Filter;
    ofn.lpstrDefExt = DefExt;
    ofn.nMaxFile    = MAX_PATH;
    ofn.lpstrTitle  = Title;

    memset(path, 0, MAX_PATH);

    if (mode == Mode::Open) {
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
        if (!GetOpenFileName(&ofn)) {
            return false;
        }
        if (ofn.nFileExtension == 0) {
            bOutArray = true;
        }
    }
    else if (mode == Mode::Save) {
        ofn.Flags = OFN_OVERWRITEPROMPT;
        if (!GetSaveFileName(&ofn)) {
            return false;
        }
    }

    return true;
}

// ********************************************************************************
/// <summary>
/// カレントディレクトリからの相対パスへ変換
/// </summary>
/// <param name="path">変換したい絶対パス</param>
/// <param name="base">指定位置：ここからの相対パスになる</param>
/// <returns>ベースからパスまでの相対パス</returns>
/// <created>いのうえ,2020/11/11</created>
/// <changed>いのうえ,2020/11/11</changed>
// ********************************************************************************
std::string FileDialog::ChangeRelativePath(LPCMofChar path, LPCMofChar base) {
    std::filesystem::path sfp = path;
    std::filesystem::path bfp = base;
    bool path_absolute = sfp.is_absolute();
    bool base_absolute = bfp.is_absolute();
    if (!path_absolute) {
        sfp = ChangeFullPath(sfp.string().c_str());
    }
    if (!base_absolute) {
        bfp = ChangeFullPath(bfp.string().c_str());
    }
    return std::filesystem::relative(sfp, bfp).string();
}

// ********************************************************************************
/// <summary>
/// 相対パスから絶対パスへの変換
/// </summary>
/// <param name="path">変換したい相対パス</param>
/// <returns>絶対パス</returns>
/// <created>いのうえ,2020/11/11</created>
/// <changed>いのうえ,2020/11/11</changed>
// ********************************************************************************
std::string FileDialog::ChangeFullPath(LPCMofChar path) {
    return std::filesystem::absolute(path).string();
}

// ********************************************************************************
/// <summary>
/// 最初の\0以降の\0区切りの文字列配列を\0\0まで分解する。
/// </summary>
/// <param name="pPath">文字列配列</param>
/// <param name="outArray">分解後の文字列を格納する配列</param>
/// <param name="outCurrentDirPath">最初の\0までの文字列の格納場所</param>
/// <created>いのうえ,2020/11/11</created>
/// <changed>いのうえ,2020/11/11</changed>
// ********************************************************************************
void FileDialog::SeparatePath(LPCMofChar pPath, std::vector<std::string>& outArray, std::string* outCurrentDirPath) {
    // 文字の長さを保存する用の配列
    std::vector<int> lengthArray;
    int pathLength = strlen(pPath);

    // カレントディレクトリの出力先がある場合、出力する。
    if (outCurrentDirPath) {
        (*outCurrentDirPath) = pPath;
    }

    int old = 0;
    while (true) {
        lengthArray.push_back(pathLength);
        old = pathLength;
        pathLength += strlen(&pPath[pathLength + 1]);

        // \0\0が来たらループをぬける。
        if ((pathLength - old) == 0) {
            // 最後の文字列データ(\0\0)の長さは必要ないので消しておく。
            lengthArray.pop_back();
            break;
        }
        pathLength++;
    }

    // 文字列を分解して保存していく。
    for (const auto& it : lengthArray) {
        outArray.push_back(&pPath[it + 1]);
    }
}
