#include <MofImGui/MofImGui.h>

#include "WindowDefine.h"
#include "MapChipWindow.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"
#include "../Utilities/FileDialog.h"
#include "../Manager/ImGuiWindowManager.h"

void MapChipWindow::ShowTextureData(MapChip* mapchip) {
    if (mapchip->IsTextureArray()) {
        std::string file_name = "no textures";
        int tex_no = mapchip->GetTextureNo();
        if (tex_no >= 0) {
            TextureArray* textures = &(*_texture_arrays)[tex_no];
            if (!(*theParam.GetDataPointer<bool>(ParamKey::WriteMode))) {
                file_name = "erase mode now";
            }
            else if (textures->size() > 0) {
                file_name = (*textures)[_select_chips.first].GetName()->GetString();
            }
        }
        ImGui::InputText("textures", &file_name[0], file_name.length(), ImGuiInputTextFlags_ReadOnly);
        if (ImGui::Button("add texture")) {
            char path[PATH_MAX];
            bool array_flag = false;
            bool open = FileDialog::Open(g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Open,
                "画像の選択",
                "画像 ファイル\0*.png;*.bmp;*.dds\0all file(*.*)\0*.*\0\0",
                "png\0bmp\0dds", path, array_flag);
            if (open) {
                int  tex_no = mapchip->GetTextureNo();
                auto select_array = &(*_texture_arrays)[tex_no];
                std::string resource_path = EditorUtilities::GetResourcePath();
                if (array_flag) {
                    std::string current;
                    std::vector<std::string> files;
                    FileDialog::SeparatePath(path, files, &current);
                    std::string relative_path = FileDialog::ChangeRelativePath(current.c_str(), resource_path.c_str());
                    for (const auto& file : files) {
                        CTexture texture;
                        if (!texture.Load((current + "\\" + file).c_str())) {
                            continue;
                        }
                        texture.SetName((relative_path + "\\" + file).c_str());
                        select_array->push_back(std::move(texture));
                    }
                }
                else {
                    CTexture texture;
                    if (texture.Load(path)) {
                        std::string relative_path = FileDialog::ChangeRelativePath(path, resource_path.c_str());
                        texture.SetName(relative_path.c_str());
                        select_array->push_back(std::move(texture));
                    }
                }
            }
        } ImGui::SameLine();
        if (ImGui::Button("remove texture")) {
            int  tex_no = mapchip->GetTextureNo();
            auto select_array = &(*_texture_arrays)[tex_no];
            auto select_tex_no = *theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
            if (select_array->size() > select_tex_no.first) {
                (*select_array)[select_tex_no.first].Release();
                select_array->erase(select_array->begin() + select_tex_no.first);
            }
            EditorUtilities::ResetSelectPair();
        }
    }
    else {
        std::string file_name = "no texture";
        int tex_no = mapchip->GetTextureNo();
        if (tex_no >= 0) {
            CTexture* texture = &(*_mapchip_texture_array)[tex_no];
            file_name = texture->GetName()->GetString();
        }
        ImGui::InputText("chip file", &file_name[0], file_name.length(), ImGuiInputTextFlags_ReadOnly);
        if (ImGui::Button("load")) {
            char path[PATH_MAX];
            bool array_flag = false;
            bool open = FileDialog::Open(g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Open,
                "マップチップ画像の読み込み",
                "画像 ファイル\0*.png;*.bmp;*.dds\0all file(*.*)\0*.*\0\0",
                "png\0dds\0bmp", path, array_flag);
            if (open) {
                std::string resource_path = EditorUtilities::GetResourcePath();
                if (_mapchip_texture_array->size() <= 0) {
                    CTexture tmp;
                    tmp.Load(path);
                    tmp.SetName(FileDialog::ChangeRelativePath(path, resource_path.c_str()).c_str());
                    _mapchip_texture_array->push_back(std::move(tmp));
                    mapchip->SetTextureNo(0);
                }
                else {
                    auto tmp = &(*_mapchip_texture_array)[0];
                    tmp->Release();
                    tmp->Load(path);
                    tmp->SetName(FileDialog::ChangeRelativePath(path, resource_path.c_str()).c_str());
                }
            }
        } ImGui::SameLine();
        if (ImGui::Button("remove")) {
            if (_mapchip_texture_array->size() > 0) {
                (*_mapchip_texture_array)[0].Release();
                _mapchip_texture_array->clear();
                mapchip->SetTextureNo(-1);
                EditorUtilities::ResetSelectPair();
            }
        }
    }
}

void MapChipWindow::ShowDummyArea(MapChip* mapchip) {
    ImVec2 dummy_area(0, 0);
    int tex_no = -1;
    if (mapchip) {
        tex_no = mapchip->GetTextureNo();
    }
    if (tex_no >= 0) {
        if (mapchip->IsTextureArray()) {
            for (auto& texture : (*_texture_arrays)[tex_no]) {
                dummy_area.x += (texture.GetWidth() * _scale);
                dummy_area.y  = (max(texture.GetHeight(), dummy_area.y) * _scale);
            }
        }
        else {
            CTexture* texture = &(*_mapchip_texture_array)[tex_no];
            dummy_area.x = (texture->GetWidth()  * _scale);
            dummy_area.y = (texture->GetHeight() * _scale);
        }
    }
    int flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove          |
        ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysVerticalScrollbar |
        ImGuiWindowFlags_AlwaysHorizontalScrollbar;
    if (ImGui::BeginChild("mapchip area", ImVec2(), true, flags)) {
        theImGuiWindowManager.Register(ParamKey::ChipWindowChild);
        ImGui::Dummy(dummy_area);
        _scroll.x = ImGui::GetScrollX();
        _scroll.y = ImGui::GetScrollY();
        ImGui::EndChild();
    }
}

void MapChipWindow::RenderTexture(float px, float py, CTexture* texture) {
    texture->RenderScale(px - _scroll.x, py - _scroll.y, _scale);
}

void MapChipWindow::RenderGrid(float px, float py, const Vector2& max_size, const Vector2& chip_size) {
    for (float y = py - _scroll.y; y < max_size.y + py - _scroll.y; y += chip_size.y) {
        CGraphicsUtilities::RenderLine(px, y, max_size.x + px - _scroll.x, y, MOF_COLOR_WHITE);
    }
    CGraphicsUtilities::RenderLine(px, max_size.y + py - _scroll.y, max_size.x + px - _scroll.x, max_size.y + py - _scroll.y, MOF_COLOR_WHITE);
    for (float x = px - _scroll.x; x < max_size.x + px - _scroll.x; x += chip_size.x) {
        CGraphicsUtilities::RenderLine(x, py, x, max_size.y + py - _scroll.y, MOF_COLOR_WHITE);
    }
    CGraphicsUtilities::RenderLine(max_size.x + px - _scroll.x, py, max_size.x + px - _scroll.x, max_size.y + py - _scroll.y, MOF_COLOR_WHITE);
}

void MapChipWindow::RenderSelectRect(float px, float py, const Vector2& tex_size, const Vector2& chip_size) {
    auto select_rect = EditorUtilities::CalcSelectRect(_select_chips.first, _select_chips.second, chip_size, tex_size);
    select_rect.Translation(Vector2(px - _scroll.x, py - _scroll.y));
    CGraphicsUtilities::RenderRect(select_rect, MOF_COLOR_RED);
}

MapChipWindow::MapChipWindow(void)
    : _select_layer(nullptr)
    , _mapchip_array(nullptr)
    , _mapchip_texture_array(nullptr)
    , _scroll(0, 0)
    , _scale(1)
    , _select_chips() {
}

MapChipWindow::~MapChipWindow(void) {
}

void MapChipWindow::Initialize(void) {
    _select_layer          = theParam.GetDataPointer<int>(ParamKey::MapChipLayerSelect);
    _mapchip_array         = theParam.GetDataPointer<std::vector<MapChip>>(ParamKey::MapChipArray);
    _mapchip_texture_array = theParam.GetDataPointer<std::vector<CTexture>>(ParamKey::MapChipTextureArray);
    _texture_arrays        = theParam.GetDataPointer<std::vector<TextureArray>>(ParamKey::TextureArrays);
    _scale                 = 1.0f;
    theParam.Register(ParamKey::MapChipSelect     , &_select_chips);
    theParam.Register(ParamKey::ChipScroll        , &_scroll      );
    theParam.Register(ParamKey::ChipScale         , &_scale       );
}

void MapChipWindow::Update(void) {
    if (_mapchip_array->size() <= 0) {
        return;
    }
    MapChip*  mapchip = &(*_mapchip_array)[*_select_layer];
    int       tex_no  = mapchip->GetTextureNo();
    if (tex_no < 0) {
        return;
    }
    CRectangle mapchip_area = *theImGuiWindowManager.Find(ParamKey::ChipWindowChild);
    Vector2 mp;
    g_pInput->GetMousePos(mp);
    if (mapchip_area.CollisionPoint(mp)) {
        float mouse_wheel = g_pInput->GetMouseWheelMove();
        if (g_pInput->IsKeyHold(MOFKEY_LCONTROL) && mouse_wheel) {
            _scale += mouse_wheel * 0.001f;
            if (_scale <= 0) {
                _scale = 0.001f;
            }
        }
        if (EditorUtilities::IsNoMapChipAreaHold()) {
            return;
        }
        if (!g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON)) {
            return;
        }
        mapchip_area = EditorUtilities::GetChipArea();
        int xcnt, ycnt;
        int selx, sely;
        int select = 0;
        if (mapchip->IsTextureArray()) {
            TextureArray* tex_array = &(*_texture_arrays)[tex_no];
            float offset_x = 0;
            for (int i = 0; i < tex_array->size(); i++) {
                CTexture*  texture = &(*tex_array)[i];
                Vector2    tex_size((texture->GetWidth() * _scale), (texture->GetHeight() * _scale));
                CRectangle tex_rect(offset_x, 0, offset_x + tex_size.x, tex_size.y);
                if (tex_rect.CollisionPoint((mp.x - mapchip_area.Left + _scroll.x), (mp.y - mapchip_area.Top + _scroll.y))) {
                    select = i;
                    break;
                }
                offset_x += tex_size.x;
            }
        }
        else {
            CTexture* texture   = &(*_mapchip_texture_array)[tex_no];
            Vector2   tex_size  = Vector2((texture->GetWidth() * _scale), (texture->GetHeight() * _scale));
            Vector2   chip_size = Vector2((mapchip->GetChipSize().x * _scale), (mapchip->GetChipSize().y  * _scale));
            xcnt   = (int)(tex_size.x                             / chip_size.x);
            ycnt   = (int)(tex_size.y                             / chip_size.y);
            selx   = (int)((mp.x - mapchip_area.Left + _scroll.x) / chip_size.x);
            sely   = (int)((mp.y - mapchip_area.Top  + _scroll.y) / chip_size.y);
            selx   = std::clamp(selx, 0, xcnt - 1);
            sely   = std::clamp(sely, 0, ycnt - 1);
            select = sely * xcnt + selx;
        }
        if (g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON)) {
            _select_chips.first = select;
        }
        _select_chips.second = select;
    }
}

void MapChipWindow::Show(void) {
    float      h           = g_pGraphics->GetTargetHeight();
    CRectangle rect        = *theImGuiWindowManager.Find(ParamKey::LayerWindow);
    ImVec2     window_pos  = ImVec2(rect.Left, rect.Bottom);
    ImVec2     window_size = ImVec2(rect.GetWidth(), h - rect.Bottom);
    ImGui::SetNextWindowBgAlpha(0);
    ImGui::SetNextWindowPos (window_pos);
    ImGui::SetNextWindowSize(window_size);
    if (h - window_pos.y < k_safe_mapchip_height) {
        return;
    }
    int flags = 
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoSavedSettings;
    ImGui::Begin("mapchip", NULL, flags); {
        MapChip*  mapchip = nullptr;
        if (_mapchip_array->size() > 0) {
            mapchip = &(*_mapchip_array)[*_select_layer];
        }
        theImGuiWindowManager.Register(ParamKey::ChipWindow);
        ShowTextureData(mapchip);
        ShowDummyArea(mapchip);
    }
    ImGui::End();
}

void MapChipWindow::Render(void) {
    CGraphicsUtilities::RenderFillRect(*theImGuiWindowManager.Find(ParamKey::ChipWindow), MOF_XRGB(16, 16, 16));
    if (_mapchip_array->size() <= 0) {
        return;
    }
    auto* mapchip = &(*_mapchip_array)[*_select_layer];
    int   tex_no  = mapchip->GetTextureNo();
    if (tex_no < 0) {
        return;
    }
    g_pGraphics->SetStencilEnable(TRUE);
    g_pGraphics->SetStencilValue(255);
    g_pGraphics->SetStencilControl(COMPARISON_ALWAYS, STENCIL_REPLACE, STENCIL_REPLACE, STENCIL_REPLACE);

    auto chip_child_rect = EditorUtilities::GetChipArea();
    CGraphicsUtilities::RenderFillRect(chip_child_rect, mapchip->GetBackColor());

    g_pGraphics->SetStencilValue(254);
    g_pGraphics->SetStencilControl(COMPARISON_LESS_EQUAL, STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP);

    if (mapchip->IsTextureArray()) {
        float offset_x = 0;
        for (auto& texture : (*_texture_arrays)[tex_no]) {
            RenderTexture(chip_child_rect.Left + offset_x, chip_child_rect.Top, &texture);
            offset_x += (texture.GetWidth() * _scale);
        }
        if ((*theParam.GetDataPointer<bool>(ParamKey::WriteMode))) {
            CRectangle select_rect = EditorUtilities::CalcSelectTextureRect(tex_no);
            select_rect.Translation(Vector2(chip_child_rect.Left - _scroll.x, chip_child_rect.Top - _scroll.y));
            CGraphicsUtilities::RenderRect(select_rect, MOF_COLOR_RED);
        }
    }
    else {
        CTexture* texture = &(*_mapchip_texture_array)[tex_no];
        RenderTexture(chip_child_rect.Left, chip_child_rect.Top, texture);

        Vector2 tex_size  = Vector2((texture->GetWidth() * _scale), (texture->GetHeight() * _scale));
        Vector2 chip_size = Vector2((mapchip->GetChipSize().x * _scale), (mapchip->GetChipSize().y * _scale));
        if (*theParam.GetDataPointer<bool>(ParamKey::ChipGridFlag)) {
            RenderGrid(chip_child_rect.Left, chip_child_rect.Top, tex_size, chip_size);
        }
        if (EditorUtilities::IsWriteMode()) {
            RenderSelectRect(chip_child_rect.Left, chip_child_rect.Top, tex_size, chip_size);
        }
    }
    
    g_pGraphics->SetStencilEnable(FALSE);
}
