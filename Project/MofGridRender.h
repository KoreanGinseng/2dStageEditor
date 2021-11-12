#pragma once
#include "EditorDefine.h"

namespace Editor
{

    // ********************************************************************************
    /// <summary>
    /// �O���b�h�`��p�N���X
    /// </summary>
    // ********************************************************************************
    class CMofGridRender
    {
    private:

    public:
        // ********************************************************************************
        /// <summary>
        /// �O���b�h�̕`��
        /// </summary>
        /// <param name="s">�O���b�h�T�C�Y</param>
        /// <param name="w">����</param>
        /// <param name="h">����</param>
        /// <returns>TRUE ����, ����ȊO ���s</returns>
        // ********************************************************************************
        static MofBool RenderGrid(MofS32 s, MofFloat w, MofFloat h);

        // ********************************************************************************
        /// <summary>
        /// ��`�̎擾
        /// </summary>
        /// <param name="no">�I��ԍ�</param>
        /// <param name="chipSize">�`�b�v�T�C�Y</param>
        /// <param name="w">����</param>
        /// <returns>�I����`</returns>
        // ********************************************************************************
        static CRectangle GetRect(MofS32 no, MofS32 chipSize, MofS32 w);
    };
}

