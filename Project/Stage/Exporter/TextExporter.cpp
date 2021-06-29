#include "TextExporter.h"
#include "../Stage.h"
#include "../../EditorParam/EditorParameter.h"

bool TextExporter::Export(const std::string& file) {
    FILE* file_pointer = fopen(file.c_str(), "wt");
    if (file_pointer == NULL) {
        return false;
    }
    auto background_array      = theParam.GetDataPointer<std::vector<BackGround>>(ParamKey::BackgroundArray);
    auto mapchip_array         = theParam.GetDataPointer<std::vector<MapChip>>(ParamKey::MapChipArray);
    auto mapchip_texture_array = theParam.GetDataPointer<std::vector<CTexture>>(ParamKey::MapChipTextureArray);
    auto texture_arrays        = theParam.GetDataPointer<std::vector<TextureArray>>(ParamKey::TextureArrays);
    if (background_array->size() > 0) {
        fprintf(file_pointer, "%s,", (*background_array)[0]._texture.GetName()->GetString());
    }
    else {
        fprintf(file_pointer, " ,");
    }
    if (mapchip_array->size() > 0) {
        int tex_no = (*mapchip_array)[0].GetTextureNo();
        if (tex_no >= 0) {
            fprintf(file_pointer, "%s,\n", (*mapchip_texture_array)[tex_no].GetName()->GetString());
        }
        else {
            fprintf(file_pointer, " ,\n");
        }
    }
    if (mapchip_array->size() > 0) {
        auto mapchip = &(*mapchip_array)[0];
        fprintf(file_pointer, "%d,\n"   , (int)mapchip->GetChipSize().x);
        fprintf(file_pointer, "%d,%d,\n", (int)mapchip->GetArraySize().x, (int)mapchip->GetArraySize().y);
        for (int y = 0; y < mapchip->GetArraySize().y; y++) {
            for (int x = 0; x < mapchip->GetArraySize().x; x++) {
                fprintf(file_pointer, "%4d,", mapchip->GetMapChip(x, y));
            }
            fprintf(file_pointer, "\n");
        }
        fprintf(file_pointer, "\n");
        for (int i = 1; i < mapchip_array->size(); i++) {
            mapchip = &(*mapchip_array)[i];
            int  tex_no = mapchip->GetTextureNo();
            if (tex_no < 0) {
                continue;
            }
            auto texture_array = &(*texture_arrays)[tex_no];
            fprintf(file_pointer, "%d,", texture_array->size());
            for (int i = 0; i < texture_array->size(); i++) {
                fprintf(file_pointer, "%s,", (*texture_array)[i].GetName()->GetString());
            }
            fprintf(file_pointer, "\n");
            for (int y = 0; y < mapchip->GetArraySize().y; y++) {
                for (int x = 0; x < mapchip->GetArraySize().x; x++) {
                    fprintf(file_pointer, "%4d,", mapchip->GetMapChip(x, y));
                }
                fprintf(file_pointer, "\n");
            }
            fprintf(file_pointer, "\n");
        }
    }
    fclose(file_pointer);
    return true;
}
