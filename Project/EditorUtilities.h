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
    /// バージョンデータ情報
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
    /// エディター用汎用クラス
    /// </summary>
    // ********************************************************************************
    class CEditorUtilities : public CSingleton<CEditorUtilities>
    {
        friend class CSingleton<CEditorUtilities>;

    public:

        // ********************************************************************************
        /// <summary>
        /// コンストラクタ
        /// </summary>
        // ********************************************************************************
        CEditorUtilities(void);

        // ********************************************************************************
        /// <summary>
        /// デストラクタ
        /// </summary>
        // ********************************************************************************
        ~CEditorUtilities(void);

        // ********************************************************************************
        /// <summary>
        /// バージョン情報取得
        /// </summary>
        /// <returns>バージョン情報, none : 失敗</returns>
        // ********************************************************************************
        std::optional<VersionResourceData> GetVersionResourceData(void);

        // ********************************************************************************
        /// <summary>
        /// 名前重複検索し、かぶらない名前を返す
        /// </summary>
        /// <param name="name">重複しているか検索する値</param>
        /// <param name="names">検索したいリスト</param>
        /// <returns>かぶらない名前</returns>
        // ********************************************************************************
        std::string SerchDuplicateName(const std::string& name, const std::vector<std::string>& names);

        // ********************************************************************************
        /// <summary>
        /// 指定ディレクトリ以下からファイルを探す
        /// </summary>
        /// <param name="dir">検索ディレクトリ</param>
        /// <param name="file">検索ファイル名</param>
        /// <returns>ファイルパス, "" : 見つからなかった</returns>
        // ********************************************************************************
        std::string GetFilePath(const std::string& dir, const std::string& file);

        // ********************************************************************************
        /// <summary>
        /// 拡張子の取得
        /// </summary>
        /// <param name="path">ファイルパス</param>
        /// <returns>拡張子</returns>
        // ********************************************************************************
        std::string GetExt(const std::string& path);

        // ********************************************************************************
        /// <summary>
        /// ファイル名の取得
        /// </summary>
        /// <param name="path">ファイルパス</param>
        /// <returns>ファイル名</returns>
        // ********************************************************************************
        std::string GetFileName(const std::string& path);

        // ********************************************************************************
        /// <summary>
        /// ImGuiのカラーピッカーでMofU32カラーを編集する
        /// </summary>
        /// <param name="color">編集するカラー</param>
        /// <returns>true : 変更あり, false : 変更なし</returns>
        // ********************************************************************************
        bool ColorPicker(MofU32& color);

        // ********************************************************************************
        /// <summary>
        /// 現在の時間を文字列で取得する
        /// </summary>
        /// <returns>現在の時間</returns>
        // ********************************************************************************
        std::string GetDatetimeStr(void);
    };
}
