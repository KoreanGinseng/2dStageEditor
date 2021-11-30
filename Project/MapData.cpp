#include "MapData.h"
#include "LayerWindow.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// データ配列の再設定
/// </summary>
/// <returns>TRUE : 成功, それ以外 : 失敗</returns>
// ********************************************************************************
MofBool CMapData::ResetData()
{
	const auto& layerWindow = CSingletonBlackboard<CLayerWindow>::GetInstance()
		.Get<CLayerWindow>("LayerWindow");
	const auto& layerDataArray = layerWindow.GetLayerDataArray();
	const auto& selectLayer    = layerWindow.GetSelectLayer();
	if (layerDataArray.size() <= 0)
	{
		return TRUE;
	}
	const MofS32& chipSize = layerDataArray[selectLayer].chipSize;
	if (chipSize <= 0)
	{
		return TRUE;
	}
	const auto& mapSize = CSingletonBlackboard<Vector2>::GetInstance()
		.Get<Vector2>("MapSize");
	m_MapData.resize((MofS32)mapSize.y / chipSize);
	for (auto& itr : m_MapData)
	{
		itr.resize((MofS32)mapSize.x / chipSize);
	}
	return TRUE;
}
