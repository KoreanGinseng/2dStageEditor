#pragma once
#include "../EditorCommon.h"

// ********************************************************************************
/// <summary>
/// メインメニュークラス
/// </summary>
// ********************************************************************************
class MainMenu final {
private:

    ImVec2 _pos{ 0.0f, 0.0f };  //!<座標
    ImVec2 _size{ 0.0f, 0.0f }; //!<サイズ

    // ********************************************************************************
    /// <summary>
    /// ファイルメニューの更新
    /// </summary>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    void UpdateFileMenu();

    // ********************************************************************************
    /// <summary>
    /// エディットメニューの更新
    /// </summary>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    void UpdateEditMenu();

    // ********************************************************************************
    /// <summary>
    /// ヴューメニューの更新
    /// </summary>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    void UpdateViewMenu();

public:
    // ********************************************************************************
    /// <summary>
    /// コンストラクタ
    /// </summary>
    // ********************************************************************************
    MainMenu();

    // ********************************************************************************
    /// <summary>
    /// デストラクタ
    /// </summary>
    // ********************************************************************************
    ~MainMenu();

    // ********************************************************************************
    /// <summary>
    /// Menuの更新
    /// </summary>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    void UpdateGui();

    // ********************************************************************************
    /// <summary>
    /// Getter
    /// </summary>
    // ********************************************************************************

    // ********************************************************************************
    /// <summary>
    /// 座標の取得
    /// </summary>
    /// <returns>Menu表示座標</returns>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    const ImVec2 GetPos() const;

    // ********************************************************************************
    /// <summary>
    /// サイズの取得
    /// </summary>
    /// <returns>Menu表示サイズ</returns>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    const ImVec2 GetSize() const;
};

