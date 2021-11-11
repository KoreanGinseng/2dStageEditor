#pragma once
#include <vector>
#include <string>

// ********************************************************************************
/// <summary>
/// レイヤーデータクラス
/// </summary>
// ********************************************************************************
class LayerData {
private:

    unsigned int _texture_no{ 0 };

    std::vector<std::vector<int>> _mapchip_data{  };

    bool _show_flag{ true };

    std::string _name{ "new_layer" };

public:

    LayerData() = default;
    
    ~LayerData() = default;

    bool ChangeMapSize(int x, int y);

    unsigned int GetTextureNo() const;

    std::vector<std::vector<int>>& GetMapChipData();

    int GetMapChipData(int x, int y) const;

    unsigned int GetMapChipXCount() const;

    unsigned int GetMapChipYCount() const;

    bool GetShowFlag() const;

    std::string GetName() const;

    
    void SetTextureNo(int no);

    void SetMapChip(int x, int y, int n);

    void SetShowFlag(bool b);

    void SetName(const std::string& name);
};

using LayerDataArray = std::vector<LayerData>;