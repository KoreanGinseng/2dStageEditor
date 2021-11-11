#pragma once
#include <Mof.h>

#define  PATH_MAX 1000

namespace Editor
{
    // ********************************************************************************
    /// <summary>
    /// ファイルダイアログ汎用クラス
    /// </summary>
    // ********************************************************************************
    class CFileDialog 
    {
    private:

        //static
        CFileDialog() = delete;
        ~CFileDialog() = delete;

    public:

        // ********************************************************************************
        /// <summary>
        /// ダイアログのモード
        /// </summary>
        // ********************************************************************************
        enum class Mode
        {
            Open,
            Save,
        };

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
        static bool Open(HWND hWnd, Mode mode, const char* Title, const char* Filter, const char* DefExt, char* path, bool& bOutArray);

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
        static std::string ChangeRelativePath(LPCMofChar path, LPCMofChar base = CUtilities::GetCurrentDirectory());

        // ********************************************************************************
        /// <summary>
        /// 相対パスから絶対パスへの変換
        /// </summary>
        /// <param name="path">変換したい相対パス</param>
        /// <returns>絶対パス</returns>
        /// <created>いのうえ,2020/11/11</created>
        /// <changed>いのうえ,2020/11/11</changed>
        // ********************************************************************************
        static std::string ChangeFullPath(LPCMofChar path);

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
        static void SeparatePath(LPCMofChar pPath, std::vector<std::string>& outArray, std::string* outCurrentDirPath = nullptr);

        // ********************************************************************************
        /// <summary>
        /// フォルダ選択ダイアログの開始
        /// </summary>
        /// <param name="title">ダイアログのタイトル</param>
        /// <param name="folderName">フォルダ名出力先</param>
        /// <param name="fullPath">フォルダ古パス出力先</param>
        /// <returns></returns>
        /// <created>いのうえ,2020/11/11</created>
        /// <changed>いのうえ,2020/11/11</changed>
        // ********************************************************************************
        static bool OpenFolderDialog(LPCMofChar title, char* folderName, char* fullPath = nullptr);
    };
}

