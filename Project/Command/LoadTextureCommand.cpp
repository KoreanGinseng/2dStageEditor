#include "LoadTextureCommand.h"
#include "../Utilities/EditorUtilities.h"
#include "../Utilities/FileDialog.h"

LoadTextureCommand::LoadTextureCommand(char* path, CTexture* texture) 
    : _path(path) 
    , _path_prev("")
    , _texture(texture) {
}

LoadTextureCommand::~LoadTextureCommand(void) {
}

void LoadTextureCommand::Execute(void) {
    if (_texture->GetName()->GetLength() > 0) {
        _path_prev = _texture->GetName()->GetString();
        _texture->Release();
    }
    std::string resource_path = EditorUtilities::GetResourcePath();
    std::string name          = FileDialog::ChangeRelativePath(_path.c_str(), resource_path.c_str());
    _texture->Load(_path.c_str());
    _texture->SetName(name.c_str());

}

void LoadTextureCommand::Undo(void) {
    _texture->Release();
    _texture->Load(_path_prev.c_str());
    _texture->SetName(_path_prev.c_str());
}

void LoadTextureCommand::Register(void) {
}

std::string LoadTextureCommand::GetLog(void) const {
    std::stringstream log;
    log << "LoadTextureCommand\n";
    log << "path : " << _path << "\n";
    log << "prev_path : " << _path_prev << "\n";
    return log.str();
}
