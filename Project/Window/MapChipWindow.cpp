#include <MofImGui/MofImGui.h>

#include "WindowDefine.h"
#include "MapChipWindow.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"
#include "../Utilities/FileDialog.h"
#include "../Manager/ImGuiWindowManager.h"

void MapChipWindow::ShowTextureData(MapChip* mapchip) {
    if (mapchip == nullptr) {
        ImGui::Text("no data");
        return;
    }

    if (mapchip->IsTextureArray()) {
        std::string file_name = "no textures";
        int tex_no = mapchip->GetTextureNo();
        if (tex_no >= 0) {
            TextureArray* textures = &(*_texture_arrays)[tex_no];
            file_name = (*textures)[0].GetName()->GetString();
        }
        if (ImGui::BeginCombo("##texture_file_name", &file_name[0])) {
            if (ImGui::Selectable("no textures", file_name == "no textures")) {
                mapchip->SetTextureNo(-1);
            }
            for (int n = 0; n < _texture_arrays->size(); n++) {
                TextureArray* tmp_textures = &(*_texture_arrays)[n];
                if (!ImGui::Selectable((*tmp_textures)[0].GetName()->GetString(), mapchip->GetTextureNo() == n)) {
                    continue;
                }
                mapchip->SetTextureNo(n);
            }
            ImGui::EndCombo();
        }
    }
    else {
        std::string file_name = "no texture";
        int tex_no = mapchip->GetTextureNo();
        if (tex_no >= 0) {
            CTexture* texture = &(*_mapchip_texture_array)[tex_no];
            file_name = texture->GetName()->GetString();
        }
        if (ImGui::BeginCombo("##texture_file_name", &file_name[0])) {
            if (ImGui::Selectable("no texture", file_name == "no texture")) {
                mapchip->SetTextureNo(-1);
            }
            for (int n = 0; n < _mapchip_texture_array->size(); n++) {
                CTexture* tmp_texture = &(*_mapchip_texture_array)[n];
                if (!ImGui::Selectable(tmp_texture->GetName()->GetString(), mapchip->GetTextureNo() == n)) {
                    continue;
                }
                mapchip->SetTextureNo(n);
            }
            ImGui::EndCombo();
        }
    }
    bool btn_load      = false;
    bool btn_add_tex   = false;
    bool btn_add_array = false;
    if (mapchip->IsTextureArray()) {
        btn_add_tex   = ImGui::Button("add tex"); ImGui::SameLine();
        btn_add_array = ImGui::Button("add array");
    }
    else {
        btn_load = ImGui::Button("load");
    }
    if (btn_load || btn_add_tex || btn_add_array) {
        std::string resource_path = EditorUtilities::GetResourcePath();
        char texture_file[PATH_MAX];
        bool array_flag = false;
        bool open = FileDialog::Open(
            g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Open,
            "画像の選択",
            "画像 ファイル\0*.png;*.bmp;*.dds\0all file(*.*)\0*.*\0\0",
            "png\0bmp\0dds", texture_file, array_flag
        );
        if (open) {
            TextureArray tmp_array;
            if (array_flag) {
                std::string current;
                std::vector<std::string> files;
                FileDialog::SeparatePath(texture_file, files, &current);
                std::string relative_path = FileDialog::ChangeRelativePath(current.c_str(), resource_path.c_str());
                for (const auto& file : files) {
                    CTexture texture;
                    if (!texture.Load((current + "\\" + file).c_str())) {
                        continue;
                    }
                    texture.SetName((relative_path + "\\" + file).c_str());
                    if (mapchip->IsTextureArray()) {
                        if (btn_add_array) {
                            tmp_array.push_back(std::move(texture));
                        }
                        else if(btn_add_tex) {
                            int  tex_no       = mapchip->GetTextureNo();
                            auto select_array = &(*_texture_arrays)[tex_no];
                            select_array->push_back(std::move(texture));
                        }
                    }
                    else {
                        mapchip->SetTextureNo(_mapchip_texture_array->size());
                        _mapchip_texture_array->push_back(std::move(texture));
                    }
                }
                if (mapchip->IsTextureArray() && btn_add_array) {
                    mapchip->SetTextureNo(_texture_arrays->size());
                    _texture_arrays->push_back(std::move(tmp_array));
                }
            }
            else {
                CTexture texture;
                if (texture.Load(texture_file)) {
                    std::string relative_path = FileDialog::ChangeRelativePath(texture_file, resource_path.c_str());
                    texture.SetName(relative_path.c_str());

                    if (mapchip->IsTextureArray()) {
                        if (btn_add_array) {
                            tmp_array.push_back(std::move(texture));
                            mapchip->SetTextureNo(_texture_arrays->size());
                            _texture_arrays->push_back(std::move(tmp_array));
                        }
                        else if (btn_add_tex) {
                            int  tex_no       = mapchip->GetTextureNo();
                            auto select_array = &(*_texture_arrays)[tex_no];
                            select_array->push_back(std::move(texture));
                        }
                    }
                    else {
                        mapchip->SetTextureNo(_mapchip_texture_array->size());
                        _mapchip_texture_array->push_back(std::move(texture));
                    }
                }
            }
        }
    } ImGui::SameLine();

    if (ImGui::Button("delete")) {
        if (mapchip->GetTextureNo() >= 0) {
            if (mapchip->IsTextureArray()) {
                auto tmp = _texture_arrays->begin() + mapchip->GetTextureNo();
                std::string str = "[" + std::string((*tmp)[0].GetName()->GetString()) +
                    "]を含む画像配列を削除します。\nよろしいですか？";
                if (MessageBox(g_pMainWindow->GetWindowHandle(), str.c_str(), "画像配列の削除", MB_YESNO | MB_APPLMODAL) == IDYES) {
                    for(auto& it : *tmp) {
                        it.Release();
                    }
                    _texture_arrays->erase(tmp);
                    mapchip->SetTextureNo(-1);
                }
            }
            else {
                auto tmp = _mapchip_texture_array->begin() + mapchip->GetTextureNo();
                std::string str = "選択されている画像[" + 
                    std::string(tmp->GetName()->GetString()) +
                    "]を削除します。\nよろしいですか？";
                if (MessageBox(g_pMainWindow->GetWindowHandle(), str.c_str(), "画像の削除", MB_YESNO | MB_APPLMODAL) == IDYES) {
                    tmp->Release();
                    _mapchip_texture_array->erase(tmp);
                    mapchip->SetTextureNo(-1);
                }
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
                dummy_area.x += texture.GetWidth();
                dummy_area.y  = max(texture.GetHeight(), dummy_area.y);
            }
        }
        else {
            CTexture* texture = &(*_mapchip_texture_array)[tex_no];
            dummy_area.x = texture->GetWidth();
            dummy_area.y = texture->GetHeight();
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

void MapChipWindow::RenderTexture(CTexture* texture, const Vector2& render_pos) {
    texture->Render(render_pos.x - _scroll.x, render_pos.y - _scroll.y);
}

void MapChipWindow::RenderGrid(const Vector2& render_pos, const Vector2& tex_size, const Vector2& chip_size) {
    for (int y = render_pos.y - _scroll.y; y <= render_pos.y + tex_size.y - _scroll.y; y += chip_size.y) {
        CGraphicsUtilities::RenderLine(
            render_pos.x, y, render_pos.x + tex_size.x, y,
            MOF_COLOR_CWHITE
        );
    }
    for (int x = render_pos.x - _scroll.x; x <= render_pos.x + tex_size.x - _scroll.x; x += chip_size.x) {
        CGraphicsUtilities::RenderLine(
            x, render_pos.y, x, render_pos.y + tex_size.y,
            MOF_COLOR_CWHITE
        );
    }
}

void MapChipWindow::RenderSelectRect(const Vector2& render_pos, const Vector2& tex_size, const Vector2& chip_size) {
    auto select_rect = EditorUtilities::CalcSelectRect(_select_chips.first, _select_chips.second, chip_size, tex_size);
    select_rect.Translation(Vector2(render_pos.x - _scroll.x, render_pos.y - _scroll.y));
    CGraphicsUtilities::RenderRect(select_rect, MOF_COLOR_RED);
}

MapChipWindow::MapChipWindow(void)
    : _select_layer(nullptr)
    , _mapchip_array(nullptr)
    , _mapchip_texture_array(nullptr)
    , _scroll(0, 0)
    , _scale(0)
    , _select_chips() {
}

MapChipWindow::~MapChipWindow(void) {
}

void MapChipWindow::Initialize(void) {
    _select_layer          = theParam.GetDataPointer<int>(ParamKey::MapChipLayerSelect);
    _mapchip_array         = theParam.GetDataPointer<std::vector<MapChip>>(ParamKey::MapChipArray);
    _mapchip_texture_array = theParam.GetDataPointer<std::vector<CTexture>>(ParamKey::MapChipTextureArray);
    _texture_arrays        = theParam.GetDataPointer<std::vector<TextureArray>>(ParamKey::TextureArrays);
    theParam.Register(ParamKey::MapChipSelect     , &_select_chips);
    theParam.Register(ParamKey::ChipScroll        , &_scroll      );
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
    float mouse_wheel = g_pInput->GetMouseWheelMove();
    if (mouse_wheel) {

    }
    if (EditorUtilities::IsNoMapChipAreaHold()) {
        return;
    }
    if (!g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON)) {
        return;
    }
    CRectangle mapchip_area = EditorUtilities::GetChipArea();
    Vector2 mp;
    g_pInput->GetMousePos(mp);
    if (mapchip_area.CollisionPoint(mp)) {
        int xcnt, ycnt;
        int selx, sely;
        int select = 0;
        if (mapchip->IsTextureArray()) {
            TextureArray* tex_array = &(*_texture_arrays)[tex_no];
            float offset_x = 0;
            for (int i = 0; i < tex_array->size(); i++) {
                CTexture*  texture = &(*tex_array)[i];
                CRectangle tex_rect(offset_x, 0, offset_x + texture->GetWidth(), (float)texture->GetHeight());
                selx = (int)(mp.x - mapchip_area.Left + _scroll.x);
                sely = (int)(mp.y - mapchip_area.Top + _scroll.y);
                if (tex_rect.CollisionPoint(selx, sely)) {
                    select = i;
                    break;
                }
                offset_x += texture->GetWidth();
            }
        }
        else {
            CTexture* texture  = &(*_mapchip_texture_array)[tex_no];
            Vector2   tex_size = Vector2(texture->GetWidth(), texture->GetHeight());
            xcnt   = (int)(tex_size.x                             / mapchip->GetChipSize().x);
            ycnt   = (int)(tex_size.y                             / mapchip->GetChipSize().y);
            selx   = (int)((mp.x - mapchip_area.Left + _scroll.x) / mapchip->GetChipSize().x);
            sely   = (int)((mp.y - mapchip_area.Top  + _scroll.y) / mapchip->GetChipSize().y);
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

    auto chip_child_rect = *theImGuiWindowManager.Find(ParamKey::ChipWindowChild);
    CGraphicsUtilities::RenderFillRect(chip_child_rect, mapchip->GetBackColor());

    g_pGraphics->SetStencilValue(254);
    g_pGraphics->SetStencilControl(COMPARISON_LESS_EQUAL, STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP);

    Vector2 render_pos = Vector2(chip_child_rect.Left + k_mapchip_offset, chip_child_rect.Top + k_mapchip_offset);

    if (mapchip->IsTextureArray()) {
        int offset_x = 0;
        for (auto& texture : (*_texture_arrays)[tex_no]) {
            Vector2 render_pos = Vector2(chip_child_rect.Left + k_mapchip_offset, chip_child_rect.Top + k_mapchip_offset);
            render_pos.x += offset_x;
            RenderTexture(&texture, render_pos);
            offset_x += texture.GetWidth();
        }
        CRectangle select_rect = EditorUtilities::CalcSelectTextureRect(tex_no);
        select_rect.Translation(Vector2(render_pos.x - _scroll.x, render_pos.y - _scroll.y));
        CGraphicsUtilities::RenderRect(select_rect, MOF_COLOR_RED);
    }
    else {
        CTexture* texture = &(*_mapchip_texture_array)[tex_no];
        RenderTexture(texture, render_pos);

        Vector2 tex_size  = Vector2(texture->GetWidth(), texture->GetHeight());
        Vector2 chip_size = mapchip->GetChipSize();
        if (*theParam.GetDataPointer<bool>(ParamKey::ChipGridFlag)) {
            RenderGrid(render_pos, tex_size, chip_size);
        }
        if (EditorUtilities::IsWriteMode()) {
            RenderSelectRect(render_pos, tex_size, chip_size);
        }
    }
    
    g_pGraphics->SetStencilEnable(FALSE);
}
