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
    };
}

