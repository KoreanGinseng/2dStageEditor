#pragma once
#include <Mof.h>
#include "../Stage/Stage.h"

class EditWindow {
private:

    std::vector<MapChip>*       _mapchip_array;
    std::vector<CTexture>*      _mapchip_texture_array;
    std::vector<BackGround>*    _background_array;
    std::vector<CollisionData>* _collision_array;
    std::vector<TextureArray>*  _texture_arrays;
    int*                        _select_chip_layer;
    int*                        _select_back_layer;
    Vector2                     _scroll;
    float                       _scale;
    bool                        _preview_flag;

    void UpdateWriteMode(MapChip* mapchip);
    void UpdateEraseMode(MapChip* mapchip);

    Vector2 GetEditPos(void);

    void ShowDummyArea(void);

    void RenderbackGround(float px, float py, const Vector2& max_area);

    void RenderGrid(float px, float py, const Vector2& max_size, const Vector2& chip_size);

    void RenderChips(float px, float py);

    void RenderSelectRect(float px, float py, const std::pair<int, int>& select_chips, const Vector2& select_pos, const Vector2& chip_size, const Vector2& tex_size);

public:

    EditWindow(void);
    ~EditWindow(void);

    void Initialize(void);

    void Update(void);

    void Show(void);
    void Render(void);
};

