#include "ToolIcon.h"
#include "../EditorParam/EditorParameter.h"

bool ToolIcon::Load(void) {
    LPCMofChar file[] = {
        "icons/new-document.png",
        "icons/folder.png",
        "icons/diskette.png",
        "icons/back-arrow.png",
        "icons/redo-arrow.png",
        "icons/edit.png",
        "icons/eraser-tool.png",
    };
    int i = 0;
    TextureArray& tex_array = GetTextureArray();
    for (const auto& it : GetKeyArray()) {
        tex_array.push_back(std::make_shared<CTexture>());
        if (!tex_array[i]->Load(file[i])) {
            return false;
        }
        i++;
    }
    return true;
}

void* ToolIcon::GetTexture(const ParamKey::Type& key) {
    TextureArray& tex_array = GetTextureArray();
    int no = 0;
    for (const auto& it : GetKeyArray()) {
        if (it == key) {
            break;
        }
        no++;
    }
    if (tex_array.size() <= no) {
        return nullptr;
    }
    return static_cast<void*>((tex_array[no])->GetTexture());
}

void ToolIcon::Release(void) {
    for (auto& it : GetTextureArray()) {
        it->Release();
        it = nullptr;
    }
    GetTextureArray().clear();
}

std::vector<ParamKey::Type>& ToolIcon::GetKeyArray(void) {
    static std::vector<ParamKey::Type> key = {
        ParamKey::IconNewFile,
        ParamKey::IconOpenFile,
        ParamKey::IconSaveFile,
        ParamKey::IconUndo,
        ParamKey::IconRedo,
        ParamKey::IconModeWrite,
        ParamKey::IconModeErase,
    };
    return key;
}

ToolIcon::TextureArray& ToolIcon::GetTextureArray(void) {
    static TextureArray tex_array;
    return tex_array;
}
