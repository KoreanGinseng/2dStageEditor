#pragma once
#include "EditorDefine.h"

namespace Editor
{

	class CMapData
	{
	private:
		MapData m_MapData;

	public:

		const MofU32 Get(MofS32 x, MofS32 y) const 
		{
			return m_MapData[y][x];
		}
	};
}

