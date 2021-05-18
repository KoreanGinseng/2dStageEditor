#include <MofImGui/MofImGui.h>
#include "EditWindow.h"
#include "WindowDefine.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"
#include "../Manager/ImGuiWindowManager.h"

/// /////////////////////////////////////////////////////////////
/// <summary>
/// ÉyÉìÉÇÅ[Éh
/// </summary>
void EditWindow::UpdateWriteMode(MapChip* mapchip) {
    if (!g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON) && !g_pInput->IsMouseKeyHold(MOFMOUSE_RBUTTON)) {
        return;
    }
    if (EditorUtilities::IsNoEditAreaHold()) {
        return;
    }
    Vector2 select_pos = GetEditPos();
    if (select_pos == Vector2(-1, -1)) {
        return;
    }
    int        tex_no       = mapchip->GetTextureNo();
    Vector2    chip_size    = mapchip->GetChipSize();
    CTexture*  texture      = nullptr;
    Vector2    tex_size     = chip_size;
    if (tex_no >= 0) {
        if (!mapchip->IsTextureArray()) {
            texture      = &(*_mapchip_texture_array)[tex_no];
            tex_size = Vector2((float)(texture->GetWidth()), (float)(texture->GetHeight()));
        }
    }
    Vector2    array_size   = mapchip->GetArraySize();
    const int  xcnt         = (int)(tex_size.x / chip_size.x);
    auto       select_chip  = *theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
    auto       selects      = EditorUtilities::GetSelectChips(select_chip.first, select_chip.second, xcnt);
    CRectangle select_rect  = EditorUtilities::CalcSelectRect(select_chip.first, select_chip.second, chip_size, tex_size);
    const int  cnt          = selects.size();
    const int  select_x_cnt = select_rect.GetWidth()  / chip_size.x;
    const int  select_y_cnt = select_rect.GetHeight() / chip_size.y;
    for (int i = 0; i < cnt; i++) {
        int x = i % select_x_cnt;
        int y = i / select_x_cnt;
        if (select_pos.x + x >= array_size.x ||
            select_pos.y + y >= array_size.y) {
            continue;
        }
        int chip_no = 0;
        if (g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON)) {
            chip_no = selects[i] + 1;
        }
        mapchip->SetMapChip(select_pos.x + x, select_pos.y + y, chip_no);
    }
}

void EditWindow::UpdateEraseMode(MapChip* mapchip) {
    Vector2 mp;
    g_pInput->GetMousePos(mp);
    auto edit_area = EditorUtilities::GetEditArea();
    if (!edit_area.CollisionPoint(mp)) {
        return;
    }
    auto select_chips = theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
    if (g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON)) {

        Vector2    array_size   = mapchip->GetArraySize();
        Vector2    chip_size_def= mapchip->GetChipSize();
        Vector2    tex_size_def = chip_size_def * array_size;
        Vector2    select_pos   = GetEditPos();
        const int  xcnt         = (int)(array_size.x);
        auto       selects      = EditorUtilities::GetSelectChips(select_chips->first, select_chips->second, xcnt);
        CRectangle select_rect  = EditorUtilities::CalcSelectRect(select_chips->first, select_chips->second, chip_size_def, tex_size_def);
        const int  cnt          = selects.size();
        const int  select_x_cnt = select_rect.GetWidth()  / chip_size_def.x;
        const int  select_y_cnt = select_rect.GetHeight() / chip_size_def.y;
        for (int i = 0; i < cnt; i++) {
            int x = i % select_x_cnt;
            int y = i / select_x_cnt;
            if (select_pos.x + x >= array_size.x ||
                select_pos.y + y >= array_size.y) {
                continue;
            }
            mapchip->SetMapChip(select_pos.x + x, select_pos.y + y, 0);
        }
    }
    else if (g_pInput->IsMouseKeyHold(MOFMOUSE_RBUTTON)) {
        int xcnt   = (int)(mapchip->GetArraySize().x);
        int ycnt   = (int)(mapchip->GetArraySize().y);
        int selx   = (int)((mp.x - edit_area.Left + _scroll.x) / (int)(mapchip->GetChipSize().x * _scale));
        int sely   = (int)((mp.y - edit_area.Top  + _scroll.y) / (int)(mapchip->GetChipSize().y * _scale));
        selx       = std::clamp(selx, 0, xcnt - 1);
        sely       = std::clamp(sely, 0, ycnt - 1);
        int select = sely * xcnt + selx;
        if (g_pInput->IsMouseKeyPush(MOFMOUSE_RBUTTON)) {
            select_chips->first = select;
        }
        select_chips->second = select;
    }
}

Vector2 EditWindow::GetEditPos(void) {
    float      w         = g_pGraphics->GetTargetWidth();
    float      h         = g_pGraphics->GetTargetHeight();
    float      size_max  = min(w * k_layer_width_ratio, k_layer_width_max);
    CRectangle edit_area = EditorUtilities::GetEditArea();
    Vector2    mp;
    g_pInput->GetMousePos(mp);
    if (!edit_area.CollisionPoint(mp)) {
        return Vector2(-1, -1);
    }
    int     layer      = *_select_chip_layer;
    auto    mapchip    = &(*_mapchip_array)[layer];
    Vector2 chip_size  = Vector2((mapchip->GetChipSize().x * _scale), (mapchip->GetChipSize().y * _scale));
    int     selx       = (int)((mp.x + _scroll.x - edit_area.Left) / chip_size.x);
    int     sely       = (int)((mp.y + _scroll.y - edit_area.Top)  / chip_size.y);
    return Vector2(selx, sely);
}

void EditWindow::ShowDummyArea(void) {
    float  h          = g_pGraphics->GetTargetHeight();
    ImVec2 window_pos = ImGui::GetWindowPos();
    if (h - window_pos.y < k_safe_edit_height) {
        return;
    }
    int flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove          |
        ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysVerticalScrollbar |
        ImGuiWindowFlags_AlwaysHorizontalScrollbar;
    if (ImGui::BeginChild("edit window child", ImVec2(), true, flags)) {
        theImGuiWindowManager.Register(ParamKey::EditWindowChild);
        int    layer     = *_select_chip_layer;
        ImVec2 dummyarea = ImVec2(0, 0);
        if (_mapchip_array->size() > 0 && layer >= 0) {
            auto    mapchip    = &(*_mapchip_array)[layer];
            Vector2 chip_size  = Vector2((mapchip->GetChipSize().x * _scale), (mapchip->GetChipSize().y * _scale));
            Vector2 array_size = mapchip->GetArraySize();
            dummyarea = ImVec2(
                chip_size.x * array_size.x,
                chip_size.y * array_size.y
            );
        }
        ImGui::Dummy(dummyarea);
        _scroll.x = ImGui::GetScrollX();
        _scroll.y = ImGui::GetScrollY();

        ImGui::EndChild();
    }
}

void EditWindow::RenderbackGround(float px, float py, const Vector2& max_area) {
    for (auto& background : *_background_array) {
        if (!background._show_flag) {
            continue;
        }
        CTexture* texture = &background._texture;
        float texture_w = texture->GetWidth()  * _scale;
        float texture_h = texture->GetHeight() * _scale;
        int tmp_w = (int)texture_w;
        int tmp_h = (int)texture_h;
        if (tmp_w == 0 || tmp_h == 0) {
            continue;
        }
        for (float y = ((int)(py - _scroll.y) % tmp_h) - texture_h; y < max_area.y; y += texture_h) {
            for (float x = ((int)(px - _scroll.x) % tmp_w) - texture_w; x < max_area.x; x += texture_w) {
                texture->RenderScale(x, y, _scale);
            }
        }
    }
}

void EditWindow::RenderGrid(float px, float py, const Vector2& max_size, const Vector2& chip_size) {
    for (float y = py - _scroll.y; y < max_size.y + py - _scroll.y; y += chip_size.y) {
        CGraphicsUtilities::RenderLine(px, y, max_size.x + px - _scroll.x, y, MOF_COLOR_WHITE);
    }
    CGraphicsUtilities::RenderLine(px, max_size.y + py - _scroll.y, max_size.x + px - _scroll.x, max_size.y + py - _scroll.y, MOF_COLOR_WHITE);
    for (float x = px - _scroll.x; x < max_size.x + px - _scroll.x; x += chip_size.x) {
        CGraphicsUtilities::RenderLine(x, py, x, max_size.y + py - _scroll.y, MOF_COLOR_WHITE);
    }
    CGraphicsUtilities::RenderLine(max_size.x + px - _scroll.x, py, max_size.x + px - _scroll.x, max_size.y + py - _scroll.y, MOF_COLOR_WHITE);
}

void EditWindow::RenderChips(float px, float py) {
    auto font = theParam.GetDataPointer<CFont>(ParamKey::NumFont);
    for (int i = 0; i < _mapchip_array->size(); i++) {
        auto mapchip = &(*_mapchip_array)[i];
        int tex_no = mapchip->GetTextureNo();
        if (tex_no < 0 || !mapchip->IsShow()) {
            continue;
        }
        CTexture* texture       = nullptr;
        Vector2   chip_size     = Vector2((mapchip->GetChipSize().x * _scale), (mapchip->GetChipSize().y * _scale));
        Vector2   chip_size_def = mapchip->GetChipSize();
        Vector2   tex_size      = chip_size;
        Vector2   tex_size_def  = chip_size_def;
        if (!mapchip->IsTextureArray()) {
            texture      = &(*_mapchip_texture_array)[tex_no];
            tex_size     = Vector2((texture->GetWidth() * _scale), (texture->GetHeight() * _scale));
            tex_size_def = Vector2((float)(texture->GetWidth()), (float)(texture->GetHeight()));
        } 
        MofU32    color        = ((_preview_flag || i == *_select_chip_layer) ? MOF_COLOR_WHITE : MOF_COLOR_HWHITE);
        Vector2   array_size   = mapchip->GetArraySize();
        int       tmp_chip_x   = (int)(tex_size_def.x / chip_size_def.x);
        int       tmp_chip_y   = (int)(tex_size_def.y / chip_size_def.y);
        int       tmp_chip_cnt = tmp_chip_x * tmp_chip_y;
        if (mapchip->IsTextureArray()) {
            tmp_chip_cnt = (*_texture_arrays)[tex_no].size();
        }
        for (int y = 0; y < array_size.y; y++) {
            for (int x = 0; x < array_size.x; x++) {
                Vector2    pos;
                CRectangle rect(0, 0, chip_size_def.x, chip_size_def.y);
                pos.x = x * chip_size.x - _scroll.x + px;
                pos.y = y * chip_size.y - _scroll.y + py;
                int chip_no = mapchip->GetMapChip(x, y) - 1;
                if (chip_no >= tmp_chip_cnt) {
                    CRectangle error_rect = rect * _scale;
                    error_rect.Translation(pos);
                    CGraphicsUtilities::RenderFillRect(error_rect, MOF_COLOR_RED);
                    continue;
                }
                if (chip_no < 0) {
                    continue;
                }
                if (mapchip->IsTextureArray()) {
                    auto texture_array = &(*_texture_arrays)[tex_no];
                    texture = &(*texture_array)[chip_no];
                }
                if (texture == nullptr) {
                    continue;
                }
                if (!mapchip->IsTextureArray()) {
                    rect.SetValue(
                        mapchip->GetChipSize().x * (chip_no % tmp_chip_x),
                        mapchip->GetChipSize().y * (chip_no / tmp_chip_x),
                        mapchip->GetChipSize().x * (chip_no % tmp_chip_x + 1),
                        mapchip->GetChipSize().y * (chip_no / tmp_chip_x + 1)
                    );
                }
                texture->RenderScale(pos.x, pos.y, _scale, rect, color);
                if (mapchip->IsShowNo()) {
                    std::string num_str = std::to_string(chip_no + 1);
                    CRectangle out;
                    CRectangle no_rect = rect;
                    no_rect.SetPosition(Vector2(0, 0));
                    no_rect.SetWidth(rect.GetWidth() * _scale);
                    no_rect.SetHeight(rect.GetHeight() * _scale);
                    no_rect.Translation(pos);
                    MofU32 col = *theParam.GetDataPointer<MofU32>(ParamKey::EditFontColor);
                    CGraphicsUtilities::CalculateStringRect(0, 0, num_str.c_str(), out);
                    CGraphicsUtilities::RenderString(no_rect, col, num_str.c_str());
                    CGraphicsUtilities::RenderRect(no_rect, col);
                }
            }
        }
    }
}

void EditWindow::RenderSelectRect(float px, float py, const std::pair<int, int>& select_chips, const Vector2& select_pos, const Vector2& chip_size, const Vector2& tex_size) {
    CRectangle select_rect = EditorUtilities::CalcSelectRect(select_chips.first, select_chips.second, chip_size, tex_size);
    int tmp_chip_x = (int)(tex_size.x / chip_size.x);
    select_rect.Translation(
        Vector2(
            px - _scroll.x + (select_pos.x * chip_size.x) - (select_chips.first % tmp_chip_x) * chip_size.x,
            py - _scroll.y + (select_pos.y * chip_size.y) - (select_chips.first / tmp_chip_x) * chip_size.y
        )
    );
    CGraphicsUtilities::RenderRect(select_rect, MOF_COLOR_RED);
}

EditWindow::EditWindow(void) {
}

EditWindow::~EditWindow(void) {
}

void EditWindow::Initialize(void) {
    _mapchip_array         = theParam.GetDataPointer<std::vector<MapChip>>(ParamKey::MapChipArray);
    _mapchip_texture_array = theParam.GetDataPointer<std::vector<CTexture>>(ParamKey::MapChipTextureArray);
    _background_array      = theParam.GetDataPointer<std::vector<BackGround>>(ParamKey::BackgroundArray);
    _texture_arrays        = theParam.GetDataPointer<std::vector<TextureArray>>(ParamKey::TextureArrays);
    _select_chip_layer     = theParam.GetDataPointer<int>(ParamKey::MapChipLayerSelect);
    _scale                 = 1.0f;
    theParam.Register(ParamKey::EditScroll, &_scroll);
    theParam.Register(ParamKey::EditScale , &_scale );
}

void EditWindow::Update(void) {

    Vector2 mp;
    g_pInput->GetMousePos(mp);
    auto edit_area = *theImGuiWindowManager.Find(ParamKey::EditWindowChild);
    if (edit_area.CollisionPoint(mp)) {
        float wheel_move = g_pInput->GetMouseWheelMove();
        if (g_pInput->IsKeyHold(MOFKEY_LCONTROL) && wheel_move) {
            _scale += wheel_move * 0.001f;
            if (_scale <= 0) {
                _scale = 0.001f;
            }
        }
    }

    if (_preview_flag) {
        return;
    }

    if (_mapchip_array->size() <= 0) {
        return;
    }

    auto mapchip = &(*_mapchip_array)[*_select_chip_layer];
    if (EditorUtilities::IsWriteMode()) {
        UpdateWriteMode(mapchip);
    }
    else {
        UpdateEraseMode(mapchip);
    }
}

void EditWindow::Show(void) {
    int w      = g_pGraphics->GetTargetWidth();
    int h      = g_pGraphics->GetTargetHeight();
    int size_w = min(w * k_layer_width_ratio, k_layer_width_max);
    CRectangle tool_rect = *theImGuiWindowManager.Find(ParamKey::ToolMenu);
    ImGui::SetNextWindowPos (ImVec2(    size_w,     tool_rect.Bottom));
    ImGui::SetNextWindowSize(ImVec2(w - size_w, h - tool_rect.Bottom));
    ImGui::SetNextWindowBgAlpha(0);
    int flags = 
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove          |
        ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoSavedSettings;
    ImGui::Begin("editwindow", NULL, flags); {
        theImGuiWindowManager.Register(ParamKey::EditWindow);
        if (ImGui::BeginTabBar("edit tabbar")) {
            if (ImGui::BeginTabItem("edit")) {
                _preview_flag = false;
                ShowDummyArea();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("preview")) {
                _preview_flag = true;
                ShowDummyArea();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

void EditWindow::Render(void) {
    if (_mapchip_array->size() <= 0) {
        return;
    }
    auto       mapchip    = &(*_mapchip_array)[*_select_chip_layer];
    int        tex_no     = mapchip->GetTextureNo();
    CTexture*  texture    = nullptr;
    Vector2    chip_size  = Vector2((mapchip->GetChipSize().x * _scale), (mapchip->GetChipSize().y * _scale));
    Vector2    tex_size   = chip_size;
    Vector2    array_size = mapchip->GetArraySize();
    Vector2    max_size   = array_size * chip_size;
    if (tex_no >= 0) {
        if (!mapchip->IsTextureArray()) {
            texture  = &(*_mapchip_texture_array)[tex_no];
            tex_size = Vector2((texture->GetWidth() * _scale), (texture->GetHeight() * _scale));
        }
    }
    Vector2    select_pos       = GetEditPos();
    auto       select_chips     = *theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
    CRectangle edit_window_rect = *theImGuiWindowManager.Find(ParamKey::EditWindow);
    CRectangle edit_child_rect  = EditorUtilities::GetEditArea();
    CGraphicsUtilities::RenderFillRect(edit_window_rect, MOF_XRGB(16, 16, 16));
    g_pGraphics->SetStencilEnable(TRUE);
    g_pGraphics->SetStencilValue(245);
    g_pGraphics->SetStencilControl(COMPARISON_ALWAYS, STENCIL_REPLACE, STENCIL_REPLACE, STENCIL_REPLACE);
    CGraphicsUtilities::RenderFillRect(edit_child_rect, *theParam.GetDataPointer<MofU32>(ParamKey::EditBackColor));
    g_pGraphics->SetStencilValue(244);
    g_pGraphics->SetStencilControl(COMPARISON_LESS_EQUAL, STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP);

    Vector2 max_area = Vector2(min(edit_child_rect.GetWidth(), max_size.x), min(edit_child_rect.GetHeight(), max_size.y));
    RenderbackGround(edit_child_rect.Left, edit_child_rect.Top, max_area + edit_child_rect.GetTopLeft());

    RenderChips(edit_child_rect.Left, edit_child_rect.Top);

    if (*theParam.GetDataPointer<bool>(ParamKey::EditGridFlag)) {
        RenderGrid(edit_child_rect.Left, edit_child_rect.Top, max_size, chip_size);
    }

    if (select_pos != Vector2(-1, -1) && !EditorUtilities::IsNoEditAreaHold() && !_preview_flag) {
        if (!EditorUtilities::IsWriteMode()) {
            tex_size = mapchip->GetArraySize() * chip_size;
            if (g_pInput->IsMouseKeyHold(MOFMOUSE_RBUTTON)) {
                select_pos.x = select_chips.first % (int)mapchip->GetArraySize().x;
                select_pos.y = select_chips.first / (int)mapchip->GetArraySize().x;
            }
        }
        RenderSelectRect(edit_child_rect.Left, edit_child_rect.Top, select_chips, select_pos, chip_size, tex_size);
    }

    g_pGraphics->SetStencilEnable(FALSE);
}
