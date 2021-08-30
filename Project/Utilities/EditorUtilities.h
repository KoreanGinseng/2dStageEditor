#pragma once
#include <Windows.h>
#include <string>
#include <optional>
#include <vector>

// ********************************************************************************
/// <summary>
/// Version.rcの内部データ取得用構造体
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
/// エディタ汎用クラス
/// </summary>
// ********************************************************************************
class EditorUtilities {
private:

    //static class
    EditorUtilities() = delete;
    ~EditorUtilities() = delete;

    // ********************************************************************************
    /// <summary>
    /// ウィンドウの矩形取得
    /// </summary>
    /// <returns>ウィンドウの矩形</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static RECT& GetWindowRect(void);

public:

    // ********************************************************************************
    /// <summary>
    /// Version.rcのデータ取得
    /// </summary>
    /// <returns>Version.rcのデータ, nullopt : 失敗</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::optional<VersionResourceData> GetVersionResourceData(void);

    // ********************************************************************************
    /// <summary>
    /// ウィンドウのサイズ変更を通知する
    /// </summary>
    /// <returns>RECT : 変更後のウィンドウRECT, nullopt : 変更なし</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::optional<RECT> ChangeWindowSize(void);

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
    static std::string GetFilePath(const std::string& dir, const std::string& file);

    // ********************************************************************************
    /// <summary>
    /// ファイル拡張子の取得
    /// </summary>
    /// <param name="path">ファイルパス</param>
    /// <returns>.を含む拡張子</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::string GetExt(const std::string& path);

    // ********************************************************************************
    /// <summary>
    /// ファイル名の取得
    /// </summary>
    /// <param name="path">ファイルパス</param>
    /// <returns>ファイル名</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::string GetFileName(const std::string& path);

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
    static std::string SerchDuplicateName(const std::string& name, const std::vector<std::string>& names);

    // ********************************************************************************
    /// <summary>
    /// Defaultフォルダのパス取得
    /// </summary>
    /// <returns>Defaultフォルダのパス</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::string GetDefaultPath(void);

    // ********************************************************************************
    /// <summary>
    /// カレントパスをDefaultフォルダに設定
    /// </summary>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static void SetCurrentPathDefault(void);

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
    static bool OpenTextureFileDialog(const std::string& title, char* path, bool* array_flag = nullptr);

    // ********************************************************************************
    /// <summary>
    /// 現在の時刻を文字列で取得
    /// </summary>
    /// <returns>現在の時刻の文字列</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    static std::string GetDatetimeStr(void);
};

