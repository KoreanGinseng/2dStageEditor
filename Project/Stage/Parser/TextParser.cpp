#include "TextParser.h"

bool TextParser::Parse(const std::string& buffer, ParseData* out) {
    auto get_str = [](const std::string& buff, int& index) {
        std::string str    = &buff[index];
        int         length = str.find_first_of(',');
        index             += length + 1;
        return str.substr(0, length);
    };
    auto get_int = [&](const std::string& buff, int& index) {
        return std::atoi(get_str(buff, index).c_str());
    };
    int     index = 0;
    MapChip mapchip;
    std::string background_texture_name = get_str(buffer, index);
    if (background_texture_name.length()) {
        BackGround background;
        if (!background._texture.Load(background_texture_name.c_str())) {
            return false;
        }
        out->background_array->push_back(std::move(background));
    }
    std::string mapchip_texture_name = get_str(buffer, index);
    if (mapchip_texture_name.length()) {
        CTexture texture;
        if (!texture.Load(mapchip_texture_name.c_str())) {
            return false;
        }
        out->mapchip_texture_array->push_back(std::move(texture));
        mapchip.SetTextureNo(0);
    }
    int mapchip_size = get_int(buffer, index);
    int array_size_x = get_int(buffer, index);
    int array_size_y = get_int(buffer, index);
    mapchip.SetChipSize(Vector2(mapchip_size, mapchip_size));
    if (!mapchip.Create(array_size_x, array_size_y)) {
        return false;
    }
    for (int y = 0; y < array_size_y; y++) {
        for (int x = 0; x < array_size_x; x++) {
            mapchip.SetMapChip(x, y, get_int(buffer, index));
        }
    }
    mapchip.SetName("mapchip");
    out->mapchip_array->push_back(std::move(mapchip));

    if (buffer.length() <= index) {
        return true;
    }

    TextureArray enemy_textures;
    int enemy_cnt = get_int(buffer, index);
    for (int i = 0; i < enemy_cnt; i++) {
        CTexture texture;
        std::string enemy_texture_name = get_str(buffer, index);
        if (!texture.Load(enemy_texture_name.c_str())) {
            return false;
        }
        enemy_textures.push_back(std::move(texture));
    }
    MapChip enemy_layer;
    if (enemy_textures.size()) {
        enemy_layer.SetTextureNo(out->texture_arrays->size());
        out->texture_arrays->push_back(std::move(enemy_textures));
    }
    enemy_layer.SetTextureArray(true);
    enemy_layer.SetChipSize(Vector2(mapchip_size, mapchip_size));
    if (!enemy_layer.Create(array_size_x, array_size_y)) {
        return false;
    }
    for (int y = 0; y < array_size_y; y++) {
        for (int x = 0; x < array_size_x; x++) {
            enemy_layer.SetMapChip(x, y, get_int(buffer, index));
        }
    }
    enemy_layer.SetName("enemy");
    out->mapchip_array->push_back(std::move(enemy_layer));

    if (buffer.length() <= index) {
        return true;
    }

    TextureArray item_textures;
    int item_cnt = get_int(buffer, index);
    for (int i = 0; i < item_cnt; i++) {
        CTexture texture;
        std::string item_texture_name = get_str(buffer, index);
        if (!texture.Load(item_texture_name.c_str())) {
            return false;
        }
        item_textures.push_back(std::move(texture));
    }
    MapChip item_layer;
    if (item_textures.size()) {
        item_layer.SetTextureNo(out->texture_arrays->size());
        out->texture_arrays->push_back(std::move(item_textures));
    }
    item_layer.SetTextureArray(true);
    item_layer.SetChipSize(Vector2(mapchip_size, mapchip_size));
    if (!item_layer.Create(array_size_x, array_size_y)) {
        return false;
    }
    for (int y = 0; y < array_size_y; y++) {
        for (int x = 0; x < array_size_x; x++) {
            item_layer.SetMapChip(x, y, get_int(buffer, index));
        }
    }
    item_layer.SetName("item");
    out->mapchip_array->push_back(std::move(item_layer));
    return true;
}
