#include "Stage.h"

Stage::Stage(void) {
}

Stage::~Stage(void) {
}

bool Stage::Load(const std::string& file) {
    MapChip tmp;
    tmp.SetTextureNo(0);
    //tmp.LoadTexture("mapchip/01Background.png");
    tmp.SetChipSize(Vector2(16, 16));
    tmp.Create(100, 100);
    for (int y = 0; y < 12; y++) {
        for (int i = 0; i < 14; i++) {
            tmp.SetMapChip(i, y, y * 14 + i + 1);
        }
    }
    _mapchip_array.push_back(std::move(tmp));
    CTexture texture;
    texture.Load("mapchip\\Tileset.png");
    _mapchip_texture_array.push_back(std::move(texture));
    return true;
}

void Stage::Initialize(void) {
}

void Stage::Update(void) {
}

void Stage::Render(void) {
    for (auto& mapchip_data : _mapchip_array) {
        int tex_no = mapchip_data.GetTextureNo();
        if (tex_no < 0) {
            continue;
        }
        CTexture* texture      = &_mapchip_texture_array[tex_no];
        int       chip_size_x  = mapchip_data.GetChipSize().x;
        int       chip_size_y  = mapchip_data.GetChipSize().y;
        int       tmp_chip_x   = texture->GetWidth()  / chip_size_x;
        int       tmp_chip_y   = texture->GetHeight() / chip_size_y;
        int       tmp_chip_cnt = tmp_chip_x * tmp_chip_y;
        for (int y = 0; y < mapchip_data.GetArraySize().y; y++) {
            for (int x = 0; x < mapchip_data.GetArraySize().x; x++) {
                int chip_no = mapchip_data.GetMapChip(x, y) - 1;
                if (chip_no < 0 || chip_no >= tmp_chip_cnt) {
                    continue;
                }
                Vector2    pos;
                CRectangle rect(
                    chip_size_x * (chip_no % tmp_chip_x),
                    chip_size_y * (chip_no / tmp_chip_x),
                    chip_size_x * (chip_no % tmp_chip_x + 1),
                    chip_size_y * (chip_no / tmp_chip_x + 1)
                    );
                pos.x = x * chip_size_x - _scroll.x;
                pos.y = y * chip_size_y - _scroll.y;
                texture->Render(pos.x, pos.y, rect);
            }
        }
    }
}

void Stage::Release(void) {
    for (auto& chipdata : _mapchip_array) {
        chipdata.Release();
    }
    _mapchip_array.clear();
    for (auto& texture : _mapchip_texture_array) {
        texture.Release();
    }
    _mapchip_texture_array.clear();
    for (auto& background : _background_array) {
        background._texture.Release();
    }
    _background_array.clear();
    for (auto& texture_array : _texture_arrays) {
        for (auto& texture : texture_array) {
            texture.Release();
        }
        texture_array.clear();
    }
    _texture_arrays.clear();
    _collisionrect_array.clear();
}

Vector2 Stage::GetScroll(void) const {
    return _scroll;
}

void Stage::SetScroll(const Vector2& scroll) {
    _scroll = scroll;
}

MapChipArray* Stage::GetChipArrayPointer(void) {
    return &_mapchip_array;
}

std::vector<TextureArray>* Stage::GetTextureArraysPointer(void) {
    return &_texture_arrays;
}

TextureArray* Stage::GetChipTextureArrayPointer(void) {
    return &_mapchip_texture_array;
}

BackGroundArray* Stage::GetBackgroundArrayPointer(void) {
    return &_background_array;
}

CollisionDataArray* Stage::GetCollisionArrayPointer(void) {
    return &_collisionrect_array;
}
