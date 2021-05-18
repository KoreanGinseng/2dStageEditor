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

    void RenderbackGround(const Vector2& offset_pos, const Vector2& max_area);

    void RenderChips(const Vector2& offset_pos);
    void RenderMapChip(const Vector2& offset_pos, MapChip* mapchip, MofU32 color);
    void RenderItems(const Vector2& offset_pos, MapChip* mapchip, MofU32 color);
    void RenderErrorRect(MapChip* mapchip, const Vector2& render_pos);
    void RenderNumRect(int chip_no, const Vector2& render_pos, const Vector2& chip_size);

    void RenderSelectRect(const Vector2& offset_pos, const std::pair<int, int>& select_chips, const Vector2& select_pos, const Vector2& chip_size, const Vector2& tex_size);

public:

    EditWindow(void);
    ~EditWindow(void);

    void Initialize(void);

    void Update(void);

    void Show(void);
    void Render(void);
};

