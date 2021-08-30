#include <Mof.h>
#include "AddTextureArrayCommand.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"
#include "../Utilities/FileDialog.h"
#include "../Stage/Stage.h"

AddTextureArrayCommand::AddTextureArrayCommand(char* path, int tex_no, bool array_flag) 
    : _path_array()
    , _tex_no(tex_no)
    , _array_flag(array_flag)
    , _insert_point(-1)
    , _set_flag(false) {
    const auto& resource_path = EditorUtilities::GetResourcePath();
    if (_array_flag) {
        std::string current;
        std::vector<std::string> files;
        FileDialog::SeparatePath(path, _path_array, &current);
        const auto& relative_path = FileDialog::ChangeRelativePath(current.c_str(), resource_path.c_str());
        _path_array.push_back(current);
        _path_array.push_back(relative_path);
    }
    else {
        const auto& relative_path = FileDialog::ChangeRelativePath(path, resource_path.c_str());
        _path_array.push_back(path);
        _path_array.push_back(relative_path);
    }
}

AddTextureArrayCommand::~AddTextureArrayCommand(void) {
}

void AddTextureArrayCommand::Execute(void) {
    const auto  texture_arrays = theParam.GetDataPointer<std::vector<TextureArray>>(ParamKey::TextureArrays);
    const auto  select_array   = &(*texture_arrays)[_tex_no];
    const auto& resource_path  = EditorUtilities::GetResourcePath();
    if (_array_flag) {
        const int   size          = _path_array.size();
        const auto& current       = _path_array[size - 2];
        const auto& relative_path = _path_array[size - 1];
        for (int i = 0; i < size - 2; i++) {
            CTexture texture;
            if (!texture.Load((current + "\\" + _path_array[i]).c_str())) {
                continue;
            }
            texture.SetName((relative_path + "\\" + _path_array[i]).c_str());
            if (!_set_flag) {
                select_array->push_back(std::move(texture));
            }
            else {
                select_array->insert(select_array->begin() + (_insert_point + i), std::move(texture));
            }
        }
    }
    else {
        CTexture texture;
        if (texture.Load(_path_array[0].c_str())) {
            texture.SetName(_path_array[1].c_str());

            if (!_set_flag) {
                select_array->push_back(std::move(texture));
            }
            else {
                select_array->insert(select_array->begin() + _insert_point, std::move(texture));
            }
        }
    }
    _set_flag = true;
}

void AddTextureArrayCommand::Undo(void) {
    const auto  texture_arrays = theParam.GetDataPointer<std::vector<TextureArray>>(ParamKey::TextureArrays);
    const auto  select_array   = &(*texture_arrays)[_tex_no];
    const auto& resource_path  = EditorUtilities::GetResourcePath();
    std::string name           = "";
    int         array_cnt      =  1;
    if (_array_flag) {
        const int   size          = _path_array.size();
        const auto& current       = _path_array[size - 2];
        const auto& relative_path = _path_array[size - 1];
        name      = (relative_path + "\\" + _path_array[0]);
        array_cnt = size - 2;
    }
    else {
        name = _path_array[1];
    }
    for (int i = 0; i < select_array->size(); i++) {
        const auto texture = &(*select_array)[i];
        if (strcmp(texture->GetName()->GetString(), name.c_str()) == 0) {
            _insert_point = i;
            break;
        }
    }
    for (int i = 0; i < array_cnt; i++) {
        auto& tmp_texture = select_array->begin() + (_insert_point);
        tmp_texture->Release();
        select_array->erase(tmp_texture);
    }
    EditorUtilities::ResetSelectPair();
}

void AddTextureArrayCommand::Register(void) {
}

std::string AddTextureArrayCommand::GetLog(void) const {
    std::stringstream log;
    log << "AddTextureArrayCommand\n";
    log << "tex_no : "       << _tex_no                          << "\n";
    log << "array_flag : "   << (_array_flag ? "true" : "false") << "\n";
    log << "insert_point : " << _insert_point                    << "\n";
    for (auto& it : _path_array) {
        log << "path : " << it << "\n";
    }
    return log.str();
}
