#pragma once
#include "EditorDefine.h"

namespace Editor
{

    // ********************************************************************************
    /// <summary>
    /// ���C���[�p�E�B���h�E
    /// </summary>
    // ********************************************************************************
    class CLayerWindow
    {
    public:

        struct LayerData
        {
            std::string name     { "" };
            MofS32      chipSize {  0 };
            MofS32      textureNo{ -1 };
            MapData     mapData  {    };

            MofBool ResetData()
            {
                if (chipSize <= 0)
                {
                    return TRUE;
                }
                const auto& mapSize = CSingletonBlackboard<Vector2>::GetInstance()
                    .Get<Vector2>("MapSize");
                mapData.resize((MofS32)mapSize.y / chipSize);
                for (auto& itr : mapData)
                {
                    itr.resize((MofS32)mapSize.x / chipSize);
                }
                return TRUE;
            }
        };

        using LayerDataArray = std::vector<LayerData>;

    private:

        MofS32 m_SelectLayer;

        LayerDataArray m_LayerDataArray;

    public:

        // ********************************************************************************
        /// <summary>
        /// �R���X�g���N�^
        /// </summary>
        // ********************************************************************************
        CLayerWindow();
        
        // ********************************************************************************
        /// <summary>
        /// �f�X�g���N�^
        /// </summary>
        // ********************************************************************************
        ~CLayerWindow();

        // ********************************************************************************
        /// <summary>
        /// �X�V
        /// </summary>
        /// <returns>TRUE ����, ����ȊO ���s</returns>
        // ********************************************************************************
        MofBool Update();

        // ********************************************************************************
        /// <summary>
        /// �I�𒆂̃��C���[�ԍ��̎擾
        /// </summary>
        /// <returns>�I�𒆂̃��C���[�ԍ�</returns>
        // ********************************************************************************
        const MofS32 GetSelectLayer() const noexcept
        {
            return m_SelectLayer;
        }

        // ********************************************************************************
        /// <summary>
        /// ���C���[�f�[�^�z��̎擾
        /// </summary>
        /// <returns>���C���[�f�[�^�z��</returns>
        // ********************************************************************************
        const LayerDataArray& GetLayerDataArray() const noexcept
        {
            return m_LayerDataArray;
        }
    };
}

