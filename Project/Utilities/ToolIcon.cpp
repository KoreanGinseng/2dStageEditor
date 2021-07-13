#include "ToolIcon.h"
#include "../EditorParam/EditorParameter.h"
#include "../Resource.h"

bool ToolIcon::Load(void) {
    LPCMofChar file[] = {
        "icons/back-arrow.png",
        "icons/diskette.png",
        "icons/edit.png",
        "icons/eraser-tool.png",
        "icons/folder.png",
        "icons/new-document.png",
        "icons/redo-arrow.png",
        "icons/selection.png",
    };
    DWORD ids[] = {
        IDB_PNG1,
        IDB_PNG2,
        IDB_PNG3,
        IDB_PNG4,
        IDB_PNG5,
        IDB_PNG6,
        IDB_PNG7,
        IDB_PNG8,
    };
    int i = 0;
    auto& tex_array = GetTextureArray();
    for (const auto& it : GetKeyArray()) {
        tex_array.push_back(std::make_shared<CInResourceTexture>());
        if (!tex_array[i]->Load(ids[i], file[i])) {
            return false;
        }
        i++;
    }
    return true;
}

void* ToolIcon::GetTexture(const ParamKey::Type& key) {
    auto& tex_array = GetTextureArray();
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
        ParamKey::IconUndo,
        ParamKey::IconSaveFile,
        ParamKey::IconModeWrite,
        ParamKey::IconModeErase,
        ParamKey::IconOpenFile,
        ParamKey::IconNewFile,
        ParamKey::IconRedo,
        ParamKey::IconModeSelect,
    };
    return key;
}

ToolIcon::InResourceTextureArray& ToolIcon::GetTextureArray(void) {
    static InResourceTextureArray tex_array;
    return tex_array;
}
