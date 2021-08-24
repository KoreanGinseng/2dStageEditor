#include "../ImGui/MofImGui.h"

#include "LayerWindow.h"
#include "WindowDefine.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"
#include "../Utilities/FileDialog.h"
#include "../Manager/ImGuiWindowManager.h"
#include "../Manager/CommandManager.h"
#include "../Command/LoadTextureCommand.h"
#include "../Command/ReleaseTextureCommand.h"
#include "../Command/ChangeMapChipCommand.h"

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
        
        ShowLayerData();

        ImGui::Separator();
        
        // map data
        ShowMapData();

        ImGui::Separator();

        // chip back color
        //ShowChipBackColor();

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
    if (_background_array->size() > 0 && (*_background_array)[0]._texture.GetName()->GetLength() > 0) {
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
    bool open = EditorUtilities::OpenTextureFileDialog("背景画像の読み込み", path);
    if (open) {
        if (_background_array->size() <= 0) {
            _background_array->push_back(BackGround());
        }
        auto tmp = &(*_background_array)[0];
        theCommandManager.Register(std::make_shared<LoadTextureCommand>(path, &(tmp->_texture)));
    }
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// 背景の削除
/// </summary>
void LayerWindow::RemoveBackGroundTexture(void) {
    if (_background_array->size() > 0) {
        auto tmp = &(*_background_array)[0];
        theCommandManager.Register(std::make_shared<ReleaseTextureCommand>(&(tmp->_texture)));
    }
}

void LayerWindow::ShowLayerData(void) {
    auto mapchip = &(*_mapchip_array)[_select_chip_layer];
    char tmp_name[MAX_PATH] = "";
    strcpy(tmp_name, mapchip->GetName().c_str());
    bool change_name = ImGui::InputText("name", tmp_name, MAX_PATH, ImGuiInputTextFlags_EnterReturnsTrue);
    if(change_name) mapchip->SetName(tmp_name);
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// マップデータの表示
/// </summary>
void LayerWindow::ShowMapData(void) {
    auto mapchip = &(*_mapchip_array)[_select_chip_layer];

    ImGui::Text("map data");
    int cs  = (int)mapchip->GetChipSize().x;
    int asx = (int)mapchip->GetArraySize().x;
    int asy = (int)mapchip->GetArraySize().y;
    
    bool change_size = ImGui::InputInt("chip size", &cs);
    bool change_x    = ImGui::InputInt("map size x", &asx, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);
    bool change_y    = ImGui::InputInt("map size y", &asy, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue);

    // 何も操作されていないとき、カウンタを増やす
    if (!change_size && !change_x && !change_y) {
        _change_frame_count++;
    }

    if (!_change_hold_flag && (change_size || change_x || change_y)) {
        _change_mapchip_command = std::make_shared<ChangeMapChipCommand>(mapchip);
        _change_hold_flag   = true;
        _change_frame_count = 0;
    }

    // chip size
    if (change_size) {
        cs = std::clamp(cs, 1, INT_MAX);
        EditorUtilities::ResetSelectPair();
        for (auto& it : *_mapchip_array) {
            it.SetChipSize(Vector2(cs, cs));
        }
    }

    // map size
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
    
    if (_change_hold_flag && _change_frame_count > change_wait_frame) {
        _change_mapchip_command->Register();
        theCommandManager.Register(std::move(_change_mapchip_command));
        _change_mapchip_command = nullptr;
        _change_hold_flag   = false;
        _change_frame_count = 0;
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

void LayerWindow::ShowBackGroundLayerTab(void) {
    // background
    ShowBackGround();

    ImGui::Separator();
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// 色タブ
/// </summary>
void LayerWindow::ShowColorEditTab(void) {
    if (ImGui::BeginChild("color", ImVec2(), true)) {
        ShowChipBackColor();
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
    _change_frame_count    = 0;
    theParam.Register(ParamKey::MapChipLayerSelect, &_select_chip_layer);
}

/// /////////////////////////////////////////////////////////////
/// <summary>
/// ImGui周りの表示
/// </summary>
void LayerWindow::Show(void) {
    auto show = theParam.GetDataPointer<bool>(ParamKey::ShowLayerWindow);
    if (!(*show)) {
        return;
    }
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
    int flags = EditorUtilities::GetImGuiDefWindowFlag();
    ImGui::Begin("layer window", show, flags); {
        theImGuiWindowManager.Register(ParamKey::ShowLayerWindow);
        if (ImGui::BeginTabBar("layer tab")) {
            if (ImGui::BeginTabItem("mapchip"))    { ShowMapChipLayerTab();    ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("background")) { ShowBackGroundLayerTab(); ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("color"))      { ShowColorEditTab();       ImGui::EndTabItem(); }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}
