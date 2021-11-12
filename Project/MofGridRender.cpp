#include "MofGridRender.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// �O���b�h�̕`��
/// </summary>
/// <param name="s">�O���b�h�T�C�Y</param>
/// <param name="w">����</param>
/// <param name="h">����</param>
/// <returns>TRUE ����, ����ȊO ���s</returns>
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
/// ��`�̎擾
/// </summary>
/// <param name="no">�I��ԍ�</param>
/// <param name="chipSize">�`�b�v�T�C�Y</param>
/// <param name="w">����</param>
/// <returns>�I����`</returns>
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
