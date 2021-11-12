#include "MofGridRender.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// グリッドの描画
/// </summary>
/// <param name="s">グリッドサイズ</param>
/// <param name="w">横幅</param>
/// <param name="h">高さ</param>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CMofGridRender::RenderGrid(MofS32 s, MofFloat w, MofFloat h)
{
    if (s <= 0)
    {
        return TRUE;
    }
    for (MofFloat y = 0.0f; y <= h; y += s)
    {
        CGraphicsUtilities::RenderLine(0.0f, y, w, y, MOF_COLOR_WHITE);
    }
    for (MofFloat x = 0.0f; x <= w; x += s)
    {
        CGraphicsUtilities::RenderLine(x, 0.0f, x, h, MOF_COLOR_WHITE);
    }
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// 矩形の取得
/// </summary>
/// <param name="no">選択番号</param>
/// <param name="chipSize">チップサイズ</param>
/// <param name="w">横幅</param>
/// <returns>選択矩形</returns>
// ********************************************************************************
CRectangle CMofGridRender::GetRect(MofS32 no, MofS32 chipSize, MofS32 w)
{
    if (chipSize <= 0)
    {
        return CRectangle();
    }
    MofS32 tx = w / chipSize;
    return CRectangle(
        (no % tx) * chipSize,
        (no / tx) * chipSize,
        (no % tx) * chipSize + chipSize,
        (no / tx) * chipSize + chipSize
    );
}
