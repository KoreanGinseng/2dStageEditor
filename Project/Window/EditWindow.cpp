#include "../ImGui/MofImGui.h"
#include "EditWindow.h"
#include "WindowDefine.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"
#include "../Manager/ImGuiWindowManager.h"
#include "../Manager/CommandManager.h"
#include "../Command/EditChipCommand.h"
#include "../Manager/ClipBoardManager.h"

/// /////////////////////////////////////////////////////////////
/// <summary>
/// ÉyÉìÉÇÅ[Éh
/// </summary>
void EditWindow::UpdateWriteMode(MapChip* mapchip) {
    if (EditorUtilities::IsNoEditAreaHold()) {
        return;
    }
    const auto& select_pos = GetEditPos();
    if (select_pos == Vector2(-1, -1)) {
        return;
    }
    if (_edit_chip_command == nullptr) {
        _edit_chip_command = std::make_shared<EditChipCommand>(mapchip);
    }
    const int   tex_no       = mapchip->GetTextureNo();
    const auto& chip_size    = mapchip->GetChipSize();
    auto        tex_size     = chip_size;
    if (tex_no < 0) {
        return;
    }
    if (!mapchip->IsTextureArray()) {
        const auto texture = &(*_mapchip_texture_array)[tex_no];
        tex_size = Vector2((float)(texture->GetWidth()), (float)(texture->GetHeight()));
    }
    const auto& array_size   = mapchip->GetArraySize();
    const int   xcnt         = (int)(tex_size.x / chip_size.x);
    const auto& select_chip  = *theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
    const auto& selects      = EditorUtilities::GetSelectChips(select_chip.first, select_chip.second, xcnt);
    const auto& select_rect  = EditorUtilities::CalcSelectRect(select_chip.first, select_chip.second, chip_size, tex_size, 1.0f);
    const int   cnt          = selects.size();
    const int   select_x_cnt = select_rect.GetWidth()  / chip_size.x;
    const int   select_y_cnt = select_rect.GetHeight() / chip_size.y;

    for (int i = 0; i < cnt; i++) {
        const int x = i % select_x_cnt;
        const int y = i / select_x_cnt;
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
    const auto& edit_area = EditorUtilities::GetEditArea();
    if (!edit_area.CollisionPoint(mp)) {
        return;
    }
    const auto& select_chips = theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
    if (g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON)) {
        if (_edit_chip_command == nullptr) {
            _edit_chip_command = std::make_shared<EditChipCommand>(mapchip);
        }

        const auto& array_size   = mapchip->GetArraySize();
        const auto& chip_size    = mapchip->GetChipSize();
        const auto& tex_size     = chip_size * array_size;
        const auto& select_pos   = GetEditPos();
        const int   xcnt         = (int)(array_size.x);
        const auto& selects      = EditorUtilities::GetSelectChips(select_chips->first, select_chips->second, xcnt);
        const auto& select_rect  = EditorUtilities::CalcSelectRect(select_chips->first, select_chips->second, chip_size, tex_size, 1.0f);
        const int   cnt          = selects.size();
        const int   select_x_cnt = select_rect.GetWidth()  / chip_size.x;
        const int   select_y_cnt = select_rect.GetHeight() / chip_size.y;
        for (int i = 0; i < cnt; i++) {
            const int x = i % select_x_cnt;
            const int y = i / select_x_cnt;
            if (select_pos.x + x >= array_size.x ||
                select_pos.y + y >= array_size.y) {
                continue;
            }
            mapchip->SetMapChip(select_pos.x + x, select_pos.y + y, 0);
        }
    }
    else if (g_pInput->IsMouseKeyHold(MOFMOUSE_RBUTTON)) {
        const int xcnt   = (int)(mapchip->GetArraySize().x);
        const int ycnt   = (int)(mapchip->GetArraySize().y);
        int       selx   = (int)((mp.x - edit_area.Left + _scroll.x) / (int)(mapchip->GetChipSize().x * _scale));
        int       sely   = (int)((mp.y - edit_area.Top  + _scroll.y) / (int)(mapchip->GetChipSize().y * _scale));
                  selx   = std::clamp(selx, 0, xcnt - 1);
                  sely   = std::clamp(sely, 0, ycnt - 1);
        const int select = sely * xcnt + selx;
        if (g_pInput->IsMouseKeyPush(MOFMOUSE_RBUTTON)) {
            select_chips->first = select;
        }
        select_chips->second = select;
    }
}

void EditWindow::UpdateSelectMode(MapChip* mapchip) {
    Vector2 mp;
    g_pInput->GetMousePos(mp);
    const auto& edit_area = EditorUtilities::GetEditArea();
    if (!edit_area.CollisionPoint(mp)) {
        return;
    }
    const auto& select_chips = theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
    if (g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON)) {
        const int xcnt = (int)(mapchip->GetArraySize().x);
        const int ycnt = (int)(mapchip->GetArraySize().y);
        int       selx = (int)((mp.x - edit_area.Left + _scroll.x) / (mapchip->GetChipSize().x * _scale));
        int       sely = (int)((mp.y - edit_area.Top  + _scroll.y) / (mapchip->GetChipSize().y * _scale));
        selx = std::clamp(selx, 0, xcnt - 1);
        sely = std::clamp(sely, 0, ycnt - 1);
        const int select = sely * xcnt + selx;
        if (g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON)) {
            select_chips->first = select;
        }
        select_chips->second = select;
    }
    auto is_ctrl = g_pInput->IsKeyHold(MOFKEY_LCONTROL) || g_pInput->IsKeyHold(MOFKEY_RCONTROL);
    if (is_ctrl && g_pInput->IsKeyPush(MOFKEY_C)) {
        const int xcnt = (int)(mapchip->GetArraySize().x);
        const int ycnt = (int)(mapchip->GetArraySize().y);
        const int offset_x = (select_chips->first % xcnt);
        const int offset_y = (select_chips->first / xcnt);
        CopyObject copy_object;
        copy_object._x_count = (select_chips->second % xcnt) - offset_x;
        copy_object._y_count = (select_chips->second / xcnt) - offset_y;
        copy_object._selects = *select_chips;
        for (int i = select_chips->first, j = 0; i <= select_chips->second;) {
            auto chip_array = mapchip->GetMapChip();
            copy_object._chips.push_back(chip_array[i]);
            if (j >= copy_object._x_count) {
                j = 0;
                i += offset_x + (mapchip->GetArraySize().x - (select_chips->second % xcnt));
            }
            else {
                i++;
                j++;
            }
        }
        theClipboardManager.Register(copy_object);
    }
    if (is_ctrl && g_pInput->IsKeyPush(MOFKEY_V)) {
        if (_edit_chip_command == nullptr) {
            _edit_chip_command = std::make_shared<EditChipCommand>(mapchip);
        }
        auto copy_object = theClipboardManager.GetCopyObject();
        if (copy_object->_chips.size() <= 0) {
            return;
        }
        const int xcnt     = mapchip->GetArraySize().x;
        const int offset_x = select_chips->first % xcnt;
        const int offset_y = select_chips->first / xcnt;
        const int x_range  = copy_object->_x_count + 1;
        const int y_range  = copy_object->_y_count + 1;
        int index = 0;
        for (int y = offset_y; y < offset_y + y_range; y++) {
            for (int x = offset_x; x < offset_x + x_range; x++) {
                mapchip->SetMapChip(x, y, copy_object->_chips[index++]);
            }
        }
    }
}

Vector2 EditWindow::GetEditPos(void) {
    const float w         = g_pGraphics->GetTargetWidth();
    const float h         = g_pGraphics->GetTargetHeight();
    const float size_max  = min(w * k_layer_width_ratio, k_layer_width_max);
    const auto& edit_area = EditorUtilities::GetEditArea();
    Vector2     mp;
    g_pInput->GetMousePos(mp);
    if (!edit_area.CollisionPoint(mp)) {
        return Vector2(-1, -1);
    }
    const int   layer      = *_select_chip_layer;
    const auto  mapchip    = &(*_mapchip_array)[layer];
    const auto& chip_size  = Vector2((mapchip->GetChipSize().x * _scale), (mapchip->GetChipSize().y * _scale));
    const int   selx       = (int)((mp.x + _scroll.x - edit_area.Left) / chip_size.x);
    const int   sely       = (int)((mp.y + _scroll.y - edit_area.Top)  / chip_size.y);
    return Vector2(selx, sely);
}

void EditWindow::ShowDummyArea(void) {
    const float h          = g_pGraphics->GetTargetHeight();
    const auto& window_pos = ImGui::GetWindowPos();
    if (h - window_pos.y < k_safe_edit_height) {
        return;
    }
    if (ImGui::BeginChild("edit window child", ImVec2(), true, EditorUtilities::GetImGuiAlwaysScrollWindowFlag())) {
        theImGuiWindowManager.Register(ParamKey::ShowEditWindowChild);
        const int   layer     = *_select_chip_layer;
        auto        dummyarea = ImVec2(0, 0);
        if (_mapchip_array->size() > 0 && layer >= 0) {
            const auto  mapchip    = &(*_mapchip_array)[layer];
            const auto& chip_size  = Vector2((mapchip->GetChipSize().x * _scale), (mapchip->GetChipSize().y * _scale));
            const auto& array_size = mapchip->GetArraySize();
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

void EditWindow::RenderbackGround(const Vector2& offset_pos, const Vector2& max_area) {
    for (auto& background : *_background_array) {
        if (!background._show_flag) {
            continue;
        }
        auto        texture = &background._texture;
        const float texture_w = texture->GetWidth()  * _scale;
        const float texture_h = texture->GetHeight() * _scale;
        const int   tmp_w     = (int)texture_w;
        const int   tmp_h     = (int)texture_h;
        if (tmp_w == 0 || tmp_h == 0) {
            continue;
        }
        for (float y = ((int)(offset_pos.y - _scroll.y) % tmp_h) - texture_h; y < max_area.y; y += texture_h) {
            for (float x = ((int)(offset_pos.x - _scroll.x) % tmp_w) - texture_w; x < max_area.x; x += texture_w) {
                texture->RenderScale(x, y, _scale);
            }
        }
    }
}

void EditWindow::RenderChips(const Vector2& offset_pos) {
    for (int i = 0; i < _mapchip_array->size(); i++) {
        const auto mapchip = &(*_mapchip_array)[i];
        const int  tex_no  = mapchip->GetTextureNo();
        if (tex_no < 0 || !mapchip->IsShow()) {
            continue;
        }
        const MofU32 color = ((_preview_flag || i == *_select_chip_layer) ? MOF_COLOR_WHITE : MOF_COLOR_HWHITE);
        if (mapchip->IsTextureArray()) {
            RenderItems(offset_pos, mapchip, color);
        }
        else {
            RenderMapChip(offset_pos, mapchip, color);
        }
    }
}

void EditWindow::RenderMapChip(const Vector2& offset_pos, MapChip* mapchip, MofU32 color) {
    auto        texture       = &(*_mapchip_texture_array)[mapchip->GetTextureNo()];
    const auto& chip_size_def = mapchip->GetChipSize();
    const auto  tex_size      = Vector2((float)(texture->GetWidth()), (float)(texture->GetHeight()));
    const auto  chip_size     = chip_size_def * _scale;
    const int   chip_x        = (int)(tex_size.x / chip_size_def.x);
    const int   chip_y        = (int)(tex_size.y / chip_size_def.y);
    const int   chip_cnt      = chip_x * chip_y;

    const auto& array_size = mapchip->GetArraySize();
    for (int y = 0; y < array_size.y; y++) {
        for (int x = 0; x < array_size.x; x++) {
            Vector2 pos;
            pos.x = x * chip_size.x - _scroll.x + offset_pos.x;
            pos.y = y * chip_size.y - _scroll.y + offset_pos.y;
            const int chip_no = mapchip->GetMapChip(x, y) - 1;
            if (chip_no >= chip_cnt) {
                RenderErrorRect(mapchip, pos);
                continue;
            }
            if (chip_no < 0) {
                continue;
            }
            const int  chip_x = (int)(tex_size.x / chip_size_def.x);
            auto       rect   = mapchip->GetChipViewRect(chip_no, chip_x);
            texture->RenderScale(pos.x, pos.y, _scale, rect, color);
            if (mapchip->IsShowNo()) {
                RenderNumRect(chip_no, pos, chip_size);
            }
        }
    }
}

void EditWindow::RenderItems(const Vector2& offset_pos, MapChip* mapchip, MofU32 color) {
    auto        tex_array      = &(*_texture_arrays)[mapchip->GetTextureNo()];
    const auto& chip_size_def  = mapchip->GetChipSize();
    const auto& chip_view_rect = mapchip->GetChipSize();
    const auto  chip_size      = chip_size_def * _scale;
    const int   chip_cnt       = tex_array->size();

    const auto& array_size = mapchip->GetArraySize();
    for (int y = 0; y < array_size.y; y++) {
        for (int x = 0; x < array_size.x; x++) {
            Vector2 pos;
            pos.x = x * chip_size.x - _scroll.x + offset_pos.x;
            pos.y = y * chip_size.y - _scroll.y + offset_pos.y;
            const int chip_no = mapchip->GetMapChip(x, y) - 1;
            if (chip_no >= chip_cnt) {
                RenderErrorRect(mapchip, pos);
                continue;
            }
            if (chip_no < 0) {
                continue;
            }
            auto texture = &(*tex_array)[chip_no];
            auto rect    = mapchip->GetChipViewRect(chip_no, 0);
            texture->RenderScale(pos.x, pos.y, _scale, rect, color);
            if (mapchip->IsShowNo()) {
                RenderNumRect(chip_no, pos, chip_size);
            }
        }
    }
}

void EditWindow::RenderErrorRect(MapChip* mapchip, const Vector2& render_pos) {
    CRectangle error_rect(Vector2(0, 0), mapchip->GetChipSize() * _scale);
    error_rect.Translation(render_pos);
    CGraphicsUtilities::RenderFillRect(error_rect, MOF_COLOR_RED);
}

void EditWindow::RenderNumRect(int chip_no, const Vector2& render_pos, const Vector2& chip_size) {
    const auto num_str = std::to_string(chip_no + 1);
    CRectangle out;
    CRectangle no_rect(0, 0, chip_size.x, chip_size.y);
    no_rect.Translation(render_pos);
    const auto col = *theParam.GetDataPointer<MofU32>(ParamKey::EditFontColor);
    CGraphicsUtilities::CalculateStringRect(0, 0, num_str.c_str(), out);
    CGraphicsUtilities::RenderString(no_rect, col, num_str.c_str());
    CGraphicsUtilities::RenderRect(no_rect, col);
}

void EditWindow::RenderSelectRect(const Vector2& offset_pos, const std::pair<int, int>& select_chips, const Vector2& select_pos, const Vector2& chip_size_def, const Vector2& tex_size_def) {
    auto        select_rect = EditorUtilities::CalcSelectRect(select_chips.first, select_chips.second, chip_size_def, tex_size_def, _scale);
    const int   chip_x      = (int)(tex_size_def.x / chip_size_def.x);
    const float tmp_x1      = (offset_pos.x - _scroll.x) + (select_pos.x * chip_size_def.x * _scale);
    const float tmp_y1      = (offset_pos.y - _scroll.y) + (select_pos.y * chip_size_def.y * _scale);
    const float tmp_x2      = (select_chips.first % chip_x) * chip_size_def.x * _scale;
    const float tmp_y2      = (select_chips.first / chip_x) * chip_size_def.y * _scale;
    select_rect.Translation(Vector2(tmp_x1 - tmp_x2, tmp_y1 - tmp_y2));
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
    auto log_area = theImGuiWindowManager.Find(ParamKey::ShowLogWindow);
    if (log_area && log_area->CollisionPoint(mp)) {
        return;
    }
    auto edit_area = *theImGuiWindowManager.Find(ParamKey::ShowEditWindowChild);
    if (edit_area.CollisionPoint(mp)) {
        const float wheel_move = g_pInput->GetMouseWheelMove();
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

    if (!g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON) && !g_pInput->IsMouseKeyHold(MOFMOUSE_RBUTTON)) {
        if (_edit_chip_command) {
            if (std::dynamic_pointer_cast<EditChipCommand>(_edit_chip_command)->IsChange()) {
                theCommandManager.Register(std::move(_edit_chip_command));
            }
            _edit_chip_command = nullptr;
        }
        if (g_pInput->IsMouseKeyPush(2)) {
            const auto& select_pos  = GetEditPos();
            auto&       select_chip = *theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
            int         copy_chip   = mapchip->GetMapChip(select_pos.x, select_pos.y);
            select_chip.first = select_chip.second = copy_chip - 1;
        }
        if (!EditorUtilities::IsSelectMode()) {
            return;
        }
    }
    if (EditorUtilities::IsWriteMode()) {
        UpdateWriteMode(mapchip);
    }
    else if (EditorUtilities::IsDeleteMode()) {
        UpdateEraseMode(mapchip);
    }
    else if (EditorUtilities::IsSelectMode()) {
        UpdateSelectMode(mapchip);
    }
}

void EditWindow::Show(void) {
    auto show = theParam.GetDataPointer<bool>(ParamKey::ShowEditWindow);
    if (!(*show)) {
        return;
    }
    const int   w         = g_pGraphics->GetTargetWidth();
    const int   h         = g_pGraphics->GetTargetHeight();
    const int   size_w    = min(w * k_layer_width_ratio, k_layer_width_max);
    const auto& tool_rect = *theImGuiWindowManager.Find(ParamKey::ToolMenu);
    ImGui::SetNextWindowPos (ImVec2(    size_w,     tool_rect.Bottom));
    ImGui::SetNextWindowSize(ImVec2(w - size_w, h - tool_rect.Bottom));
    ImGui::SetNextWindowBgAlpha(0);
    ImGui::Begin("editwindow", show, EditorUtilities::GetImGuiDefWindowFlag()); {
        theImGuiWindowManager.Register(ParamKey::ShowEditWindow);
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
    auto        mapchip          = &(*_mapchip_array)[*_select_chip_layer];
    const auto& chip_size        = mapchip->GetChipSize() * _scale;
    const auto& array_size       = mapchip->GetArraySize();
    const auto& max_size         = array_size * chip_size;
    const auto  edit_window_rect = *theImGuiWindowManager.Find(ParamKey::ShowEditWindow);
    
    CGraphicsUtilities::RenderFillRect(edit_window_rect, MOF_XRGB(16, 16, 16));
    
    g_pGraphics->SetStencilEnable(TRUE);
    g_pGraphics->SetStencilValue(245);
    g_pGraphics->SetStencilControl(COMPARISON_ALWAYS, STENCIL_REPLACE, STENCIL_REPLACE, STENCIL_REPLACE);
    
    const auto  edit_child_rect = EditorUtilities::GetEditArea();
    CGraphicsUtilities::RenderFillRect(edit_child_rect, *theParam.GetDataPointer<MofU32>(ParamKey::EditBackColor));
    
    g_pGraphics->SetStencilValue(244);
    g_pGraphics->SetStencilControl(COMPARISON_LESS_EQUAL, STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP);

    const auto  max_area   = Vector2(min(edit_child_rect.GetWidth(), max_size.x), min(edit_child_rect.GetHeight(), max_size.y));
    const auto& offset_pos = edit_child_rect.GetTopLeft();
    RenderbackGround(offset_pos, offset_pos + max_area);

    RenderChips(offset_pos);

    if (*theParam.GetDataPointer<bool>(ParamKey::EditGridFlag)) {
        EditorUtilities::RenderGrid(offset_pos, max_size, chip_size, _scroll);
    }

    {
        const auto col = *theParam.GetDataPointer<MofU32>(ParamKey::EditFontColor);
        if (*theParam.GetDataPointer<bool>(ParamKey::MemoryX)) {
            int i = 0;
            for (float x = -_scroll.x; x < max_area.x; x += chip_size.x) {
                CGraphicsUtilities::RenderLine(offset_pos.x + x, offset_pos.y, offset_pos.x + x, offset_pos.y + chip_size.y * 0.5f, col);
                CGraphicsUtilities::RenderString(offset_pos.x + x, offset_pos.y, "%d", i++);
            }
        }
        if (*theParam.GetDataPointer<bool>(ParamKey::MemoryY)) {
            int i = 0;
            for (float y = -_scroll.y; y < max_area.y; y += chip_size.y) {
                CGraphicsUtilities::RenderLine(offset_pos.x, offset_pos.y + y, offset_pos.x + chip_size.x * 0.5f, offset_pos.y + y, col);
                CGraphicsUtilities::RenderString(offset_pos.x, offset_pos.y + y, "%d", i++);
            }
        }
    }

    auto select_pos = GetEditPos();
    if (!_preview_flag) {
        const auto select_chips = *theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
        const int  tex_no       = mapchip->GetTextureNo();
        auto       tex_size_def = mapchip->GetChipSize();
        if (tex_no >= 0 && !mapchip->IsTextureArray()) {
            const auto texture = &(*_mapchip_texture_array)[tex_no];
            tex_size_def = Vector2((float)texture->GetWidth(), (float)texture->GetHeight());
        }
        if (EditorUtilities::IsSelectMode()) {
            tex_size_def = array_size * mapchip->GetChipSize();
            select_pos.x = select_chips.first % (int)array_size.x;
            select_pos.y = select_chips.first / (int)array_size.x;
        }
        else if (select_pos != Vector2(-1, -1) && !EditorUtilities::IsNoEditAreaHold() && EditorUtilities::IsDeleteMode()) {
            tex_size_def = array_size * mapchip->GetChipSize();
            if (g_pInput->IsMouseKeyHold(MOFMOUSE_RBUTTON)) {
                select_pos.x = select_chips.first % (int)array_size.x;
                select_pos.y = select_chips.first / (int)array_size.x;
            }
        }
        RenderSelectRect(offset_pos, select_chips, select_pos, mapchip->GetChipSize(), tex_size_def);
    }

    g_pGraphics->SetStencilEnable(FALSE);
}
