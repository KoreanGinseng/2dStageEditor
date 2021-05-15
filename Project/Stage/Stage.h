#pragma once
#include <Mof.h>

#include <string>
#include <vector>

#include "MapChip.h"

struct CollisionData {
    bool                    _show_flag;
    std::string             _name;
    std::vector<CRectangle> _rect_array;
};

struct BackGround {
    bool        _show_flag{ true };
    CTexture    _texture;
};

using TextureArray       = std::vector<CTexture>;
using BackGroundArray    = std::vector<BackGround>;
using CollisionDataArray = std::vector<CollisionData>;
using MapChipArray       = std::vector<MapChip>;

class Stage {
private:

    TextureArray              _mapchip_texture_array;
    std::vector<TextureArray> _texture_arrays;
    BackGroundArray           _background_array;
    MapChipArray              _mapchip_array;
    CollisionDataArray        _collisionrect_array;
    Vector2                   _scroll;

public:

    Stage(void);
    ~Stage(void);

    bool Load(const std::string& file);

    void Initialize(void);

    void Update(void);

    void Render(void);

    void Release(void);

    Vector2 GetScroll(void) const;
    void SetScroll(const Vector2& scroll);

    TextureArray* GetChipTextureArrayPointer(void);
    std::vector<TextureArray>* GetTextureArraysPointer(void);
    MapChipArray* GetChipArrayPointer(void);
    BackGroundArray* GetBackgroundArrayPointer(void);
    CollisionDataArray* GetCollisionArrayPointer(void);
};
