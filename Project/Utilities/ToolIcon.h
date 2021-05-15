#pragma once
#include <vector>
#include <string>
#include <memory>
#include <Mof.h>
#include "../EditorParam/ParameterKey.h"

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
class ToolIcon {
private:

    ToolIcon(void) = delete;
    ~ToolIcon(void) = delete;

public:
    using TexturePtr   = std::shared_ptr<CTexture>;
    using TextureArray = std::vector<TexturePtr>;

    static bool Load(void);

    static void* GetTexture(const ParamKey::Type& key);

    static void Release(void);

    static std::vector<ParamKey::Type>& GetKeyArray(void);
    static TextureArray& GetTextureArray(void);
};

