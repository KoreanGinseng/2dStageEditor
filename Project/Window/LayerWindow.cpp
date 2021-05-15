#include <MofImGui/MofImGui.h>

#include "LayerWindow.h"
#include "WindowDefine.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"
#include "../Utilities/FileDialog.h"
#include "../Manager/ImGuiWindowManager.h"

void LayerWindow::ShowMapChipLayerTab(void) {

    MapChip* mapchip = nullptr;
    std::string name = "no data";
    if (_mapchip_array->size() > 0) {
        mapchip = &(*_mapchip_array)[_select_chip_layer];
        name    = mapchip->GetName();
    }
    char input_name[PATH_MAX];
    strcpy(input_name, name.c_str());
    if (ImGui::InputText("layer name", input_name, PATH_MAX, ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (mapchip) {
            mapchip->SetName(EditorUtilities::SerchDuplicateName(input_name, EditorUtilities::GetChipLayerNames()));
        }
    }
    float w      = g_pGraphics->GetTargetWidth();
    float size_w = min(w * k_layer_width_ratio, k_layer_width_max);
    if (ImGui::BeginChild("layer", ImVec2(size_w - k_layer_updown_width, k_layer_select_height), true)) {
        for (int n = 0; n < _mapchip_array->size(); n++) {
            bool show_flag = mapchip->IsShow();
            if (ImGui::Checkbox(std::string("##chip_layer_show" + std::to_string(n)).c_str(), &show_flag)) {
                mapchip->SetShow(show_flag);
            } ImGui::SameLine();
            if (ImGui::Selectable((*_mapchip_array)[n].GetName().c_str(), _select_chip_layer == n)) {
                if (_select_chip_layer != n) {
                    EditorUtilities::ResetSelectPair();
                }
                _select_chip_layer = n;
                break;
            }
        }
        ImGui::EndChild();
    } ImGui::SameLine();
    if (ImGui::BeginChild("layer_up_down", ImVec2(0, k_layer_select_height))) {
        if (ImGui::ArrowButton("##chip_layer_up", ImGuiDir_Up)) {
            int prev = _select_chip_layer - 1;
            if (prev >= 0) {
                std::swap((*_mapchip_array)[_select_chip_layer], (*_mapchip_array)[prev]);
                _select_chip_layer = prev;
            }
        }
        if (ImGui::ArrowButton("##chip_layer_down", ImGuiDir_Down)) {
            int next = _select_chip_layer + 1;
            if (next < _mapchip_array->size()) {
                std::swap((*_mapchip_array)[_select_chip_layer], (*_mapchip_array)[next]);
                _select_chip_layer = next;
            }
        }
        ImGui::EndChild();
    }
    if (ImGui::Button("add")) {
        ImGui::OpenPopup("add layer");
        EditorUtilities::OpenPopupModal();
    }
    if (_mapchip_array->size() > 0) {
        ImGui::SameLine();
        if (ImGui::Button("delete")) {
            std::string str = "選択されたレイヤー[" +
                (*_mapchip_array)[_select_chip_layer].GetName() + 
                "]を削除します。\nよろしいですか？";
            if (MessageBox(g_pMainWindow->GetWindowHandle(), str.c_str(), "レイヤーの削除", MB_YESNO | MB_APPLMODAL) == IDYES) {
                auto tmp = _mapchip_array->begin() + _select_chip_layer;
                tmp->Release();
                _mapchip_array->erase(tmp);
                _select_chip_layer = 0;
            }
        }
    }
    if (ImGui::BeginChild("detail", ImVec2(0, k_layer_detail_height), true)) {
        if (mapchip) {
            int cs[2] = { (int)mapchip->GetChipSize().x , (int)mapchip->GetChipSize().y  };
            int as[2] = { (int)mapchip->GetArraySize().x, (int)mapchip->GetArraySize().y };
            if (ImGui::DragInt2("chip size" , cs, 0.5f)) {
                int size_max[2] = { INT_MAX, INT_MAX };
                EditorUtilities::ResetSelectPair();
                mapchip->SetChipSize(Vector2(std::clamp(cs[0], 1, size_max[0]), std::clamp(cs[1], 1, size_max[1])));
            }
            ImGui::AlignTextToFramePadding();
            if (ImGui::InputInt2("map size", as, ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (as[0] > 0 && as[1] > 0) {
                    mapchip->Create(as[0], as[1]);
                }
                else {
                    MessageBox(g_pMainWindow->GetWindowHandle(),
                        "マップサイズは1以上を指定してください。", "入力値間違えたのかな？",
                        MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
                }
            }
            bool tex_array = !mapchip->IsTextureArray();
            if (ImGui::Checkbox("one texture", &tex_array)) {
                mapchip->SetTextureArray(!tex_array);
                mapchip->SetTextureNo(-1);
            }
            if (ImGui::TreeNode("chip back color")) {
                MofU32 color = mapchip->GetBackColor();
                if (EditorUtilities::ColorPicker(color)) {
                    mapchip->SetBackColor(color);
                }
                ImGui::TreePop();
            }
        }
        else {
            ImGui::Text("no data");
        }
        ImGui::EndChild();
    }
    ImGui::Checkbox("chip grid", theParam.GetDataPointer<bool>(ParamKey::ChipGridFlag)); ImGui::SameLine();
    ImGui::Checkbox("edit grid", theParam.GetDataPointer<bool>(ParamKey::EditGridFlag));
    ShowAddLayerModal();
}

void LayerWindow::ShowBackgroundLayerTab(void) {
    float w      = g_pGraphics->GetTargetWidth();
    float size_w = min(w * k_layer_width_ratio, k_layer_width_max);
    if (ImGui::BeginChild("background", ImVec2(size_w - k_layer_updown_width, k_layer_background_height), true)) {
        for (int i = 0; i < _background_array->size(); i++) {
            bool show_flag = (*_background_array)[i]._show_flag;
            if (ImGui::Checkbox(("##background" + std::to_string(i)).c_str(), &show_flag)) {
                (*_background_array)[i]._show_flag = show_flag;
            } ImGui::SameLine();
            if (ImGui::Selectable((*_background_array)[i]._texture.GetName()->GetString(), (_select_background_layer == i))) {
                _select_background_layer = i;
                break;
            }
        }
        ImGui::EndChild();
    } ImGui::SameLine();
    if (ImGui::BeginChild("layer_up_down", ImVec2(0, k_layer_select_height))) {
        if (ImGui::ArrowButton("##chip_layer_up", ImGuiDir_Up)) {
            int prev = _select_background_layer - 1;
            if (prev >= 0) {
                std::swap((*_background_array)[_select_background_layer], (*_background_array)[prev]);
                _select_background_layer = prev;
            }
        }
        if (ImGui::ArrowButton("##chip_layer_down", ImGuiDir_Down)) {
            int next = _select_background_layer + 1;
            if (next < _background_array->size()) {
                std::swap((*_background_array)[_select_background_layer], (*_background_array)[next]);
                _select_background_layer = next;
            }
        }
        ImGui::EndChild();
    }
    if (ImGui::Button("add")) {
        char path[PATH_MAX];
        bool array_flag = false;
        bool open = FileDialog::Open(g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Open,
            "背景画像の読み込み",
            "画像 ファイル\0*.png;*.bmp;*.dds\0all file(*.*)\0*.*\0\0",
            "png\0dds\0bmp", path, array_flag
            );
        std::string resource_path = EditorUtilities::GetResourcePath();
        if (open) {
            if (array_flag) {
                std::string relative;
                std::vector<std::string> out;
                FileDialog::SeparatePath(path, out, &relative);
                for (const auto& file : out) {
                    BackGround tmp;
                    std::string file_path = (relative + "\\" + file);
                    tmp._texture.Load(file_path.c_str());
                    tmp._texture.SetName(FileDialog::ChangeRelativePath(file_path.c_str(), resource_path.c_str()).c_str());
                    _background_array->push_back(std::move(tmp));
                }
            }
            else {
                BackGround tmp;
                tmp._texture.Load(path);
                tmp._texture.SetName(FileDialog::ChangeRelativePath(path, resource_path.c_str()).c_str());
                _background_array->push_back(std::move(tmp));
            }
        }
    }
    ImGui::SameLine();
    if (_background_array->size() > 0 && ImGui::Button("delete")) {
        std::string str = "選択された背景[" + 
            std::string((*_background_array)[_select_background_layer]._texture.GetName()->GetString()) + 
            "]を削除します。\nよろしいですか？";
        if (MessageBox(g_pMainWindow->GetWindowHandle(), str.c_str(), "背景の削除", MB_YESNO | MB_APPLMODAL) == IDYES) {
            auto tmp = _background_array->begin() + _select_background_layer;
            tmp->_texture.Release();
            _background_array->erase(tmp);
            _select_background_layer = 0;
        }
    }
}

void LayerWindow::ShowCollisionLayerTab(void) {
    if (ImGui::BeginChild("collision", ImVec2(), true)) {
        ImGui::EndChild();
    }
}

void LayerWindow::ShowColorEditTab(void) {
    if (ImGui::BeginChild("color", ImVec2(), true)) {
        if (ImGui::TreeNode("edit back color")) {
            EditorUtilities::ColorPicker(*theParam.GetDataPointer<MofU32>(ParamKey::EditBackColor));
            ImGui::TreePop();
        }
        ImGui::EndChild();
    }
}

void LayerWindow::ShowAddLayerModal(void) {
    int flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
    if (ImGui::BeginPopupModal("add layer", NULL, flags)) {
        ImVec2 size(k_addlayer_width, k_addlayer_height);
        ImVec2 pos((g_pGraphics->GetTargetWidth() - size.x) * 0.5f, (g_pGraphics->GetTargetHeight() - size.y) * 0.5f);
        ImGui::SetWindowSize(size);
        ImGui::SetWindowPos(pos);
        static int  create_chip_size[2]        = {  32,32 };
        static int  create_map_size[2]         = { 100,20 };
        static char layer_name[MAX_PATH]       = "new layer";
        static char texture_file[PATH_MAX]     = "";
        static char relative_path[PATH_MAX]    = "no file";
        static int  select_texture             = -1;
        static bool load_texture               = false;
        CTexture*   texture                    = nullptr;
        std::string resource_path              = EditorUtilities::GetResourcePath();
        ImGui::InputText("layer name" , layer_name, MAX_PATH);
        ImGui::InputInt2("chip size"  , create_chip_size);
        ImGui::InputInt2("map size"   , create_map_size );
        ImGui::Checkbox("load texture", &load_texture   );
        if (load_texture) {
            ImGui::InputText("file", relative_path, PATH_MAX, ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
            if (ImGui::Button("load")) {
                bool array_flag = false;
                bool open       = FileDialog::Open(
                    g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Open,
                    "マップチップ画像の選択",
                    "画像 ファイル\0*.png;*.bmp;*.dds\0all file(*.*)\0*.*\0\0",
                    "png\0bmp\0dds", texture_file, array_flag
                );
                if (open) {
                    std::string path = FileDialog::ChangeRelativePath(texture_file, resource_path.c_str());
                    strcpy(relative_path, path.c_str());
                }
            }
        }
        else {
            std::string preview = "no texutre";
            if (select_texture >= 0 && select_texture < _mapchip_texture_array->size()) {
                texture = &(*_mapchip_texture_array)[select_texture];
                preview = texture->GetName()->GetString();
            }
            if (ImGui::BeginCombo("textures", preview.c_str())) {
                if (ImGui::Selectable("no texture", preview == "no texture")) {
                    select_texture = -1;
                }
                for (int i = 0; i < _mapchip_texture_array->size(); i++) {
                    CTexture* texture = &(*_mapchip_texture_array)[i];
                    if (ImGui::Selectable(texture->GetName()->GetString(), (i == select_texture))) {
                        select_texture = i;
                    }
                }
                ImGui::EndCombo();
            }
        }
        if (ImGui::Button("add")) {
            MapChip tmp_chip;
            tmp_chip.SetName(EditorUtilities::SerchDuplicateName(layer_name, EditorUtilities::GetChipLayerNames()));
            tmp_chip.SetChipSize(Vector2(create_chip_size[0], create_chip_size[1]));
            tmp_chip.Create(max(create_map_size[0], 1), max(create_map_size[1], 1));
            tmp_chip.SetTextureNo(select_texture);
            if (load_texture) {
                CTexture texture;
                if (texture.Load(texture_file)) {
                    tmp_chip.SetTextureNo(_mapchip_texture_array->size());
                    texture.SetName(relative_path);
                    _mapchip_texture_array->push_back(std::move(texture));
                }
            }
            _select_chip_layer = _mapchip_array->size();
            _mapchip_array->push_back(std::move(tmp_chip));
            EditorUtilities::ResetSelectPair();
            ImGui::CloseCurrentPopup();
            EditorUtilities::ClosePopupModal();
        } ImGui::SameLine();
        if (ImGui::Button("cancel")) {
            ImGui::CloseCurrentPopup();
            EditorUtilities::ClosePopupModal();
        }
        ImGui::EndPopup();
    }
}

void LayerWindow::Initialize(void) {
    _mapchip_array         = theParam.GetDataPointer<std::vector<MapChip>>(ParamKey::MapChipArray);
    _mapchip_texture_array = theParam.GetDataPointer<std::vector<CTexture>>(ParamKey::MapChipTextureArray);
    _background_array      = theParam.GetDataPointer<std::vector<BackGround>>(ParamKey::BackgroundArray);
    theParam.Register(ParamKey::MapChipLayerSelect, &_select_chip_layer);
}

void LayerWindow::Show(void) {
    float      w           = g_pGraphics->GetTargetWidth();
    float      h           = g_pGraphics->GetTargetHeight();
    float      size_w      = min(w * k_layer_width_ratio, k_layer_width_max);
    CRectangle tool_rect   = *theImGuiWindowManager.Find(ParamKey::ToolMenu);
    ImVec2     window_pos  = ImVec2(0, tool_rect.Bottom);
    ImVec2     window_size = ImVec2(size_w, k_layer_height);
    ImGui::SetNextWindowBgAlpha(1);
    ImGui::SetNextWindowPos (window_pos);
    ImGui::SetNextWindowSize(window_size);
    if (h - window_pos.y < k_safe_layer_height) {
        return;
    }
    int flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
    ImGui::Begin("layer window", NULL, flags); {
        theImGuiWindowManager.Register(ParamKey::LayerWindow);
        if (ImGui::BeginTabBar("layer tab")) {
            if (ImGui::BeginTabItem("mapchip"))    { ShowMapChipLayerTab();    ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("background")) { ShowBackgroundLayerTab(); ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("collision"))  { ShowCollisionLayerTab();  ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("color"))      { ShowColorEditTab();       ImGui::EndTabItem(); }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}
