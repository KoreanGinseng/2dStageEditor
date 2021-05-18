#include <MofImGui/MofImGui.h>

#include "LayerWindow.h"
#include "WindowDefine.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"
#include "../Utilities/FileDialog.h"
#include "../Manager/ImGuiWindowManager.h"

/// /////////////////////////////////////////////////////////////
/// <summary>
/// レイヤータブ
/// </summary>
void LayerWindow::ShowMapChipLayerTab(void) {
    
    // layer info
    if (ImGui::BeginChild("layer", ImVec2(0, k_layer_select_height), true)) {
        ShowLayerInfo();
        ImGui::EndChild();
    }

    // detail
    if (ImGui::BeginChild("detail", ImVec2(0, k_layer_detail_height), true)) {
        
        // background
        ShowBackGround();
        
        ImGui::Separator();

        // map data
        ShowMapData();

        ImGui::Separator();

        // chip back color
        ShowChipBackColor();

        ImGui::EndChild();
    }

    // grid check
    ImGui::Checkbox("chip grid", theParam.GetDataPointer<bool>(ParamKey::ChipGridFlag)); ImGui::SameLine();
    ImGui::Checkbox("edit grid", theParam.GetDataPointer<bool>(ParamKey::EditGridFlag));
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// レイヤー情報
/// </summary>
void LayerWindow::ShowLayerInfo(void) {
    ImGui::Text("show | no | layer");
    ImGui::Separator();
    if (ImGui::BeginChild("layer array", ImVec2(0, 0))) {
        for (int n = 0; n < _mapchip_array->size(); n++) {
            // show
            bool show_flag = (*_mapchip_array)[n].IsShow();
            if (ImGui::Checkbox(std::string("##chip_layer_show" + std::to_string(n)).c_str(), &show_flag)) {
                (*_mapchip_array)[n].SetShow(show_flag);
            } ImGui::SameLine(); ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();

            // no
            bool show_flag_no = (*_mapchip_array)[n].IsShowNo();
            if (ImGui::Checkbox(std::string("##chip_layer_show_no" + std::to_string(n)).c_str(), &show_flag_no)) {
                (*_mapchip_array)[n].SetShowNo(show_flag_no);
            } ImGui::SameLine(); ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();

            // layer
            if (ImGui::Selectable((*_mapchip_array)[n].GetName().c_str(), _select_chip_layer == n)) {
                if (_select_chip_layer != n) {
                    EditorUtilities::ResetSelectPair();
                }
                _select_chip_layer = n;
                break;
            }
        }
        ImGui::EndChild();
    }
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// 背景情報
/// </summary>
void LayerWindow::ShowBackGround(void) {
    ImGui::Text("background");
    std::string background_name = "no texture";
    
    // show
    bool show = false;
    if (_background_array->size() > 0) {
        background_name = (*_background_array)[0]._texture.GetName()->GetString();
        show = (*_background_array)[0]._show_flag;
    }
    if (ImGui::Checkbox("##show", &show)) {
        if (_background_array->size() > 0) {
            (*_background_array)[0]._show_flag = show;
        }
    }ImGui::SameLine();

    // file
    ImGui::InputText("file", &background_name[0], background_name.length(), ImGuiInputTextFlags_ReadOnly);
    
    if (ImGui::Button("load")) {
        LoadBackGroundTexture();
    } ImGui::SameLine();

    if (ImGui::Button("remove")) {
        RemoveBackGroundTexture();
    }
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// 背景の読み込み
/// </summary>
void LayerWindow::LoadBackGroundTexture(void) {
    char path[PATH_MAX];
    bool array_flag = false;
    bool open = FileDialog::Open(g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Open,
        "背景画像の読み込み",
        "画像 ファイル\0*.png;*.bmp;*.dds\0all file(*.*)\0*.*\0\0",
        "png\0dds\0bmp", path, array_flag);
    if (open) {
        std::string resource_path = EditorUtilities::GetResourcePath();
        if (_background_array->size() <= 0) {
            BackGround tmp;
            tmp._texture.Load(path);
            tmp._texture.SetName(FileDialog::ChangeRelativePath(path, resource_path.c_str()).c_str());
            _background_array->push_back(std::move(tmp));
        }
        else {
            auto tmp = &(*_background_array)[0];
            tmp->_texture.Release();
            tmp->_texture.Load(path);
            tmp->_texture.SetName(FileDialog::ChangeRelativePath(path, resource_path.c_str()).c_str());
        }
    }
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// 背景の削除
/// </summary>
void LayerWindow::RemoveBackGroundTexture(void) {
    if (_background_array->size() > 0) {
        auto tmp = &(*_background_array)[0];
        tmp->_texture.Release();
        _background_array->clear();
    }
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// マップデータの表示
/// </summary>
void LayerWindow::ShowMapData(void) {
    MapChip* mapchip = &(*_mapchip_array)[_select_chip_layer];

    ImGui::Text("map data");
    int cs  = (int)mapchip->GetChipSize().x;
    int asx = (int)mapchip->GetArraySize().x;
    int asy = (int)mapchip->GetArraySize().y;
    
    // chip size
    if (ImGui::InputInt("chip size", &cs)) {
        cs = std::clamp(cs, 1, INT_MAX);
        EditorUtilities::ResetSelectPair();
        for (auto& it : *_mapchip_array) {
            it.SetChipSize(Vector2(cs, cs));
        }
    }

    // map size
    bool change_x = ImGui::InputInt("map size x", &asx, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);
    bool change_y = ImGui::InputInt("map size y", &asy, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);
    if (change_x || change_y) {
        if (asx > 0 && asy > 0) {
            for (auto& it : *_mapchip_array) {
                it.Create(asx, asy);
            }
        }
        else {
            MessageBox(g_pMainWindow->GetWindowHandle(),
                "マップサイズは1以上を指定してください。", "入力値間違えたのかな？",
                MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
        }
    }
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// マップチップの背景色
/// </summary>
void LayerWindow::ShowChipBackColor(void) {
    MapChip* mapchip = &(*_mapchip_array)[_select_chip_layer];

    if (ImGui::TreeNode("chip back color")) {
        MofU32 color = mapchip->GetBackColor();
        if (EditorUtilities::ColorPicker(color)) {
            mapchip->SetBackColor(color);
        }
        ImGui::TreePop();
    }
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// 色タブ
/// </summary>
void LayerWindow::ShowColorEditTab(void) {
    if (ImGui::BeginChild("color", ImVec2(), true)) {
        if (ImGui::TreeNode("edit back color")) {
            EditorUtilities::ColorPicker(*theParam.GetDataPointer<MofU32>(ParamKey::EditBackColor));
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("edit font color")) {
            EditorUtilities::ColorPicker(*theParam.GetDataPointer<MofU32>(ParamKey::EditFontColor));
            ImGui::TreePop();
        }
        ImGui::EndChild();
    }
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// 初期化
/// </summary>
void LayerWindow::Initialize(void) {
    _mapchip_array         = theParam.GetDataPointer<std::vector<MapChip>>(ParamKey::MapChipArray);
    _mapchip_texture_array = theParam.GetDataPointer<std::vector<CTexture>>(ParamKey::MapChipTextureArray);
    _background_array      = theParam.GetDataPointer<std::vector<BackGround>>(ParamKey::BackgroundArray);
    theParam.Register(ParamKey::MapChipLayerSelect, &_select_chip_layer);
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// ImGui周りの表示
/// </summary>
void LayerWindow::Show(void) {
    float  w           = g_pGraphics->GetTargetWidth();
    float  h           = g_pGraphics->GetTargetHeight();
    float  size_w      = min(w * k_layer_width_ratio, k_layer_width_max);
    auto   tool_rect   = *theImGuiWindowManager.Find(ParamKey::ToolMenu);
    ImVec2 window_pos  = ImVec2(0, tool_rect.Bottom);
    ImVec2 window_size = ImVec2(size_w, k_layer_height);
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
            if (ImGui::BeginTabItem("mapchip")) { ShowMapChipLayerTab(); ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("color"))   { ShowColorEditTab();    ImGui::EndTabItem(); }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}
