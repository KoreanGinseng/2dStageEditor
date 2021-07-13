#pragma once
#include <vector>
#include <string>
#include <memory>
#include <Mof.h>
#include "../EditorParam/ParameterKey.h"
#include "InResourceTexture.h"

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
    using InResourceTexturePtr   = std::shared_ptr<CInResourceTexture>;
    using InResourceTextureArray = std::vector<InResourceTexturePtr>;

    static bool Load(void);

    static void* GetTexture(const ParamKey::Type& key);

    static void Release(void);

    static std::vector<ParamKey::Type>& GetKeyArray(void);
    static InResourceTextureArray& GetTextureArray(void);
};

