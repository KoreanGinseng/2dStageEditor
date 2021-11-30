#pragma once
#include "EditorDefine.h"
#include "MapData.h"

namespace Editor
{

    // ********************************************************************************
    /// <summary>
    /// レイヤー用ウィンドウ
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
            CMapData    mapData  {    };

            MofBool ResetData()
            {
                return mapData.ResetData();
            }
        };

        using LayerDataArray = std::vector<LayerData>;

    private:

        MofS32 m_SelectLayer;

        LayerDataArray m_LayerDataArray;

    public:

        // ********************************************************************************
        /// <summary>
        /// コンストラクタ
        /// </summary>
        // ********************************************************************************
        CLayerWindow();
        
        // ********************************************************************************
        /// <summary>
        /// デストラクタ
        /// </summary>
        // ********************************************************************************
        ~CLayerWindow();

        // ********************************************************************************
        /// <summary>
        /// 更新
        /// </summary>
        /// <returns>TRUE 成功, それ以外 失敗</returns>
        // ********************************************************************************
        MofBool Update();

        // ********************************************************************************
        /// <summary>
        /// 選択中のレイヤー番号の取得
        /// </summary>
        /// <returns>選択中のレイヤー番号</returns>
        // ********************************************************************************
        const MofS32 GetSelectLayer() const noexcept
        {
            return m_SelectLayer;
        }

        // ********************************************************************************
        /// <summary>
        /// レイヤーデータ配列の取得
        /// </summary>
        /// <returns>レイヤーデータ配列</returns>
        // ********************************************************************************
        const LayerDataArray& GetLayerDataArray() const noexcept
        {
            return m_LayerDataArray;
        }
    };
}

