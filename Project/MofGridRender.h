#pragma once
#include "EditorDefine.h"

namespace Editor
{

    // ********************************************************************************
    /// <summary>
    /// グリッド描画用クラス
    /// </summary>
    // ********************************************************************************
    class CMofGridRender
    {
    private:

    public:
        // ********************************************************************************
        /// <summary>
        /// グリッドの描画
        /// </summary>
        /// <param name="s">グリッドサイズ</param>
        /// <param name="w">横幅</param>
        /// <param name="h">高さ</param>
        /// <returns>TRUE 成功, それ以外 失敗</returns>
        // ********************************************************************************
        static MofBool RenderGrid(MofS32 s, MofFloat w, MofFloat h);

        // ********************************************************************************
        /// <summary>
        /// 矩形の取得
        /// </summary>
        /// <param name="no">選択番号</param>
        /// <param name="chipSize">チップサイズ</param>
        /// <param name="w">横幅</param>
        /// <returns>選択矩形</returns>
        // ********************************************************************************
        static CRectangle GetRect(MofS32 no, MofS32 chipSize, MofS32 w);
    };
}

