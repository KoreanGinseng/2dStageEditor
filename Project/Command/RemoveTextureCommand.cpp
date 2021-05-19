#include "RemoveTextureCommand.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"

RemoveTextureCommand::RemoveTextureCommand(TextureArray* tex_array, int select, MapChip* mapchip)
    : _texture_array(tex_array)
    , _tex_no(select)
    , _mapchip(mapchip)
    , _remove_name("") {
}

RemoveTextureCommand::~RemoveTextureCommand(void) {
}

void RemoveTextureCommand::Execute(void) {
    auto texture = &(*_texture_array)[_tex_no];
    _remove_name = texture->GetName()->GetString();
    texture->Release();
    _texture_array->erase(_texture_array->begin() + _tex_no);
    if (!_mapchip->IsTextureArray()) {
        _mapchip->SetTextureNo(_texture_array->size() - 1);
    }
    EditorUtilities::ResetSelectPair();
}

void RemoveTextureCommand::Undo(void) {
    const auto& resource_path = EditorUtilities::GetResourcePath();
    CTexture texture;
    texture.Load((resource_path + "\\" + _remove_name).c_str());
    texture.SetName(_remove_name.c_str());
    _texture_array->insert(_texture_array->begin() + _tex_no, std::move(texture));
    if (!_mapchip->IsTextureArray()) {
        _mapchip->SetTextureNo(0);
    }
}

void RemoveTextureCommand::Register(void) {
}
