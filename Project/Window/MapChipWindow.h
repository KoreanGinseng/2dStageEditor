#pragma once
#include <Mof.h>
#include "../Stage/Stage.h"

class MapChipWindow {
private:

    std::vector<MapChip>*      _mapchip_array;
    TextureArray*              _mapchip_texture_array;
    std::vector<TextureArray>* _texture_arrays;
    int*                       _select_layer;
    Vector2                    _scroll;
    float                      _scale;
    std::pair<int, int>        _select_chips;

    void ShowTextureData(MapChip* mapchip);
    void ShowDummyArea(MapChip* mapchip);

    void RenderTexture(float px, float py, CTexture* texture);
    void RenderGrid(float px, float py, const Vector2& max_size, const Vector2& chip_size);
    void RenderSelectRect(float px, float py, const Vector2& tex_size_def, const Vector2& chip_size_def);

public:
    
    MapChipWindow(void);
    
    ~MapChipWindow(void);
    
    void Initialize(void);

    void Update(void);

    void Show(void);
    void Render(void);
};

