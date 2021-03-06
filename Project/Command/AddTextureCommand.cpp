#include "AddTextureCommand.h"
#include "../Utilities/EditorUtilities.h"
#include "../Utilities/FileDialog.h"
#include "../EditorParam/EditorParameter.h"
#include "../Stage/Stage.h"

AddTextureCommand::AddTextureCommand(char* path, TextureArray* texture_array, MapChip* mapchip)
    : _path(path)
    , _path_prev("")
    , _texture_array(texture_array)
    , _mapchip(mapchip) {
}

AddTextureCommand::~AddTextureCommand(void) {
}

void AddTextureCommand::Execute(void) {
    const auto& resource_path = EditorUtilities::GetResourcePath();
    if (_texture_array->size() <= 0) {
        CTexture texture;
        texture.Load(_path.c_str());
        texture.SetName(FileDialog::ChangeRelativePath(_path.c_str(), resource_path.c_str()).c_str());
        _texture_array->push_back(std::move(texture));
        if (_mapchip) _mapchip->SetTextureNo(0);
    }
    else {
        auto texture = &(*_texture_array)[0];
        _path_prev   = texture->GetName()->GetString();
        texture->Release();
        texture->Load(_path.c_str());
        texture->SetName(FileDialog::ChangeRelativePath(_path.c_str(), resource_path.c_str()).c_str());
        if (_mapchip) _mapchip->SetTextureNo(0);
    }
}

void AddTextureCommand::Undo(void) {
    auto        texture       = _texture_array->begin();
    const auto& resource_path = EditorUtilities::GetResourcePath();
    texture->Release();
    if (_path_prev == "") {
        _texture_array->erase(texture);
        if (_mapchip) _mapchip->SetTextureNo(-1);
    }
    else {
        texture->Load((resource_path + "\\" + _path_prev).c_str());
        texture->SetName(_path_prev.c_str());
    }
}

void AddTextureCommand::Register(void) {
}

std::string AddTextureCommand::GetLog(void) const {
    std::stringstream log;
    log << "AddTextureCommand\n";
    log << "path : "      << _path      << "\n";
    log << "prev_path : " << _path_prev << "\n";
    return log.str();
}
