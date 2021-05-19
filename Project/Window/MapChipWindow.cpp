#include <MofImGui/MofImGui.h>

#include "WindowDefine.h"
#include "MapChipWindow.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"
#include "../Utilities/FileDialog.h"
#include "../Manager/ImGuiWindowManager.h"
#include "../Manager/CommandManager.h"
#include "../Command/AddTextureArrayCommand.h"
#include "../Command/AddTextureCommand.h"
#include "../Command/RemoveTextureCommand.h"

void MapChipWindow::ShowTextureData(MapChip* mapchip) {
    const int tex_no = mapchip->GetTextureNo();
    if (mapchip->IsTextureArray()) {
        std::string file_name = "no textures";
        if (tex_no >= 0) {
            const auto textures = &(*_texture_arrays)[tex_no];
            if (!(*theParam.GetDataPointer<bool>(ParamKey::WriteMode))) {
                file_name = "erase mode now";
            }
            else if (textures->size() > 0) {
                file_name = (*textures)[_select_chips.first].GetName()->GetString();
            }
        }
        ImGui::InputText("textures", &file_name[0], file_name.length(), ImGuiInputTextFlags_ReadOnly);
        if (ImGui::Button("add texture")) {
            char       path[PATH_MAX];
            bool       array_flag = false;
            const bool open       = EditorUtilities::OpenTextureFileDialog("画像ファイルの選択", path, &array_flag);
            if (open) {
                theCommandManager.Register(std::make_shared<AddTextureArrayCommand>(path, tex_no, array_flag));
            }
        } ImGui::SameLine();
        if (ImGui::Button("remove texture")) {
            const auto select_array  = &(*_texture_arrays)[tex_no];
            const auto select_tex_no = *theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
            if (select_array->size() > select_tex_no.first) {
                theCommandManager.Register(std::make_shared<RemoveTextureCommand>(select_array, select_tex_no.first, mapchip));
            }
        }
    }
    else {
        std::string file_name = "no texture";
        if (tex_no >= 0) {
            const auto& texture = &(*_mapchip_texture_array)[tex_no];
            file_name = texture->GetName()->GetString();
        }
        ImGui::InputText("chip file", &file_name[0], file_name.length(), ImGuiInputTextFlags_ReadOnly);
        if (ImGui::Button("load")) {
            char       path[PATH_MAX];
            bool       array_flag = false;
            const bool open       = EditorUtilities::OpenTextureFileDialog("マップチップ画像の読み込み", path);
            if (open) {
                theCommandManager.Register(std::make_shared<AddTextureCommand>(path, mapchip));
            }
        } ImGui::SameLine();
        if (ImGui::Button("remove")) {
            if (_mapchip_texture_array->size() > 0) {
                theCommandManager.Register(std::make_shared<RemoveTextureCommand>(_mapchip_texture_array, mapchip->GetTextureNo(), mapchip));
            }
        }
    }
}

void MapChipWindow::ShowDummyArea(MapChip* mapchip) {
    ImVec2    dummy_area(0, 0);
    const int tex_no = mapchip->GetTextureNo();
    if (tex_no >= 0) {
        if (mapchip->IsTextureArray()) {
            for (auto& texture : (*_texture_arrays)[tex_no]) {
                dummy_area.x += (texture.GetWidth() * _scale);
                dummy_area.y  = (max(texture.GetHeight(), dummy_area.y) * _scale);
            }
        }
        else {
            const auto texture = &(*_mapchip_texture_array)[tex_no];
            dummy_area.x = (texture->GetWidth()  * _scale);
            dummy_area.y = (texture->GetHeight() * _scale);
        }
    }
    if (ImGui::BeginChild("mapchip area", ImVec2(), true, EditorUtilities::GetImGuiAlwaysScrollWindowFlag())) {
        theImGuiWindowManager.Register(ParamKey::ChipWindowChild);
        ImGui::Dummy(dummy_area);
        _scroll.x = ImGui::GetScrollX();
        _scroll.y = ImGui::GetScrollY();
        ImGui::EndChild();
    }
}

void MapChipWindow::RenderSelectRect(const Vector2& offset_pos, const Vector2& tex_size_def, const Vector2& chip_size_def) {
    auto select_rect = EditorUtilities::CalcSelectRect(_select_chips.first, _select_chips.second, chip_size_def, tex_size_def, _scale);
    select_rect.Translation(Vector2(offset_pos.x - _scroll.x, offset_pos.y - _scroll.y));
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
    const auto mapchip = &(*_mapchip_array)[*_select_layer];
    const int  tex_no  = mapchip->GetTextureNo();
    if (tex_no < 0) {
        return;
    }
    const auto& child_rect = *theImGuiWindowManager.Find(ParamKey::ChipWindowChild);
    Vector2 mp;
    g_pInput->GetMousePos(mp);
    if (child_rect.CollisionPoint(mp)) {
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
        const auto& mapchip_area = EditorUtilities::GetChipArea();
        int xcnt, ycnt;
        int selx, sely;
        int select = 0;
        if (mapchip->IsTextureArray()) {
            TextureArray* tex_array = &(*_texture_arrays)[tex_no];
            float offset_x = 0;
            for (int i = 0; i < tex_array->size(); i++) {
                const auto texture  = &(*tex_array)[i];
                const auto tex_size = Vector2((float)texture->GetWidth(), (float)texture->GetHeight()) * _scale;
                const auto tex_rect = CRectangle(offset_x, 0, offset_x + tex_size.x, tex_size.y);
                if (tex_rect.CollisionPoint((mp.x - mapchip_area.Left + _scroll.x), (mp.y - mapchip_area.Top + _scroll.y))) {
                    select = i;
                    break;
                }
                offset_x += tex_size.x;
            }
        }
        else {
            const auto texture   = &(*_mapchip_texture_array)[tex_no];
            const auto tex_size  = Vector2((float)texture->GetWidth(), (float)texture->GetHeight());
            const auto chip_size = mapchip->GetChipSize();
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
    const float h           = g_pGraphics->GetTargetHeight();
    const auto& rect        = *theImGuiWindowManager.Find(ParamKey::LayerWindow);
    const auto  window_pos  = ImVec2(rect.Left, rect.Bottom);
    const auto  window_size = ImVec2(rect.GetWidth(), h - rect.Bottom);
    ImGui::SetNextWindowBgAlpha(0);
    ImGui::SetNextWindowPos (window_pos);
    ImGui::SetNextWindowSize(window_size);
    if (h - window_pos.y < k_safe_mapchip_height) {
        return;
    }
    ImGui::Begin("mapchip", NULL, EditorUtilities::GetImGuiDefWindowFlag()); {
        MapChip*  mapchip = nullptr;
        if (_mapchip_array->size() > 0) {
            mapchip = &(*_mapchip_array)[*_select_layer];
        }
        theImGuiWindowManager.Register(ParamKey::ChipWindow);
        if (mapchip) {
            ShowTextureData(mapchip);
            ShowDummyArea(mapchip);
        }
    }
    ImGui::End();
}

void MapChipWindow::Render(void) {
    CGraphicsUtilities::RenderFillRect(*theImGuiWindowManager.Find(ParamKey::ChipWindow), MOF_XRGB(16, 16, 16));
    if (_mapchip_array->size() <= 0) {
        return;
    }
    const auto mapchip = &(*_mapchip_array)[*_select_layer];
    const int  tex_no  = mapchip->GetTextureNo();
    if (tex_no < 0) {
        return;
    }
    g_pGraphics->SetStencilEnable(TRUE);
    g_pGraphics->SetStencilValue(255);
    g_pGraphics->SetStencilControl(COMPARISON_ALWAYS, STENCIL_REPLACE, STENCIL_REPLACE, STENCIL_REPLACE);

    const auto& chip_child_rect = EditorUtilities::GetChipArea();
    CGraphicsUtilities::RenderFillRect(chip_child_rect, mapchip->GetBackColor());

    g_pGraphics->SetStencilValue(254);
    g_pGraphics->SetStencilControl(COMPARISON_LESS_EQUAL, STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP);

    if (mapchip->IsTextureArray()) {
        float offset_x = 0;
        for (auto& texture : (*_texture_arrays)[tex_no]) {
            texture.RenderScale(chip_child_rect.Left + offset_x - _scroll.x, chip_child_rect.Top - _scroll.y, _scale);
            offset_x += (texture.GetWidth() * _scale);
        }
        if ((*theParam.GetDataPointer<bool>(ParamKey::WriteMode))) {
            auto select_rect = EditorUtilities::CalcSelectTextureRect(tex_no);
            select_rect.Translation(Vector2(chip_child_rect.Left - _scroll.x, chip_child_rect.Top - _scroll.y));
            CGraphicsUtilities::RenderRect(select_rect, MOF_COLOR_RED);
        }
    }
    else {
        const auto texture = &(*_mapchip_texture_array)[tex_no];
        texture->RenderScale(chip_child_rect.Left - _scroll.x, chip_child_rect.Top - _scroll.y, _scale);

        const auto  tex_size_def  = Vector2((float)texture->GetWidth(), (float)texture->GetHeight());
        const auto  tex_size      = tex_size_def * _scale;
        const auto& chip_size_def = mapchip->GetChipSize();
        const auto  chip_size     = chip_size_def * _scale;
        if (*theParam.GetDataPointer<bool>(ParamKey::ChipGridFlag)) {
            EditorUtilities::RenderGrid(chip_child_rect.GetTopLeft(), tex_size, chip_size, _scroll);
        }
        if (EditorUtilities::IsWriteMode()) {
            RenderSelectRect(chip_child_rect.GetTopLeft(), tex_size_def, chip_size_def);
        }
    }
    
    g_pGraphics->SetStencilEnable(FALSE);
}
