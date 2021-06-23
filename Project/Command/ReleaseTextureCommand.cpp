#include "ReleaseTextureCommand.h"
#include "../Utilities/EditorUtilities.h"
#include "../Utilities/FileDialog.h"

ReleaseTextureCommand::ReleaseTextureCommand(CTexture* texture)
    : _texture(texture)
    , _path(texture->GetName()->GetString()) {
}

ReleaseTextureCommand::~ReleaseTextureCommand(void) {
}

void ReleaseTextureCommand::Execute(void) {
    _path = _texture->GetName()->GetString();
    _texture->Release();
}

void ReleaseTextureCommand::Undo(void) {
    auto resource_path = EditorUtilities::GetResourcePath();
    _texture->Load((resource_path + "\\" + _path).c_str());
    _texture->SetName(_path.c_str());
}

void ReleaseTextureCommand::Register(void) {
}

std::string ReleaseTextureCommand::GetLog(void) const {
    std::stringstream log;
    log << "ReleaseTextureCommand\n";
    log << "path : " << _path << "\n";
    return log.str();
}
