#include "LayerData.h"

bool LayerData::ChangeMapSize(int x, int y)
{
    return false;
}

unsigned int LayerData::GetTextureNo() const
{
    return 0;
}

int* LayerData::GetMapChipData()
{
    return nullptr;
}

int LayerData::GetMapChipData(int x, int y) const
{
    return 0;
}

unsigned int LayerData::GetMapChipXCount() const
{
    return 0;
}

unsigned int LayerData::GetMapChipYCount() const
{
    return 0;
}

bool LayerData::GetShowFlag() const
{
    return false;
}

std::string LayerData::GetName() const
{
    return std::string();
}

void LayerData::SetTextureNo(int no)
{
}

void LayerData::SetMapChip(int x, int y, int n)
{
}

void LayerData::SetShowFlag(bool b)
{
}

void LayerData::SetName(const std::string& name)
{
}
