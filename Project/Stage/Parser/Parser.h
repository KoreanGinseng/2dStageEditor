#pragma once
#include "../Stage.h"

struct ParseData {
    TextureArray*              mapchip_texture_array;
    std::vector<TextureArray>* texture_arrays;
    MapChipArray*              mapchip_array;
    CollisionDataArray*        collisionrect_array;
    BackGroundArray*           background_array;
};

class IParser {
public:
    virtual bool Parse(const std::string& buffer, ParseData* out) = 0;
};

using ParserPtr = std::shared_ptr<IParser>;