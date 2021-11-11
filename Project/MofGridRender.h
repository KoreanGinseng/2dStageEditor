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
    };
}

