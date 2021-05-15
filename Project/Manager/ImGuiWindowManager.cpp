#include "ImGuiWindowManager.h"
#include "../EditorParam/EditorParameter.h"

ImGuiWindowManager::ImGuiWindowManager(void) {
    theParam.Register(ParamKey::WindowRectArray, &_data_array);
}

ImGuiWindowManager::~ImGuiWindowManager(void) {
}

void ImGuiWindowManager::Register(const ParamKey::Type& name) {
    LPRectangle   rect_pointer = Find(name);
    ImVec2        pos  = ImGui::GetWindowPos();
    ImVec2        size = ImGui::GetWindowSize();
    CRectangle    rect = CRectangle(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    if (rect_pointer != nullptr) {
        *rect_pointer   = rect;
    }
    else if (std::optional<bool*> active_pointer = theParam.GetData<bool>(name)) {
        GuiWindowData data;
        data.name = name;
        data.rect = rect;
        data.active_pointer = active_pointer.value();
        _data_array.push_back(data);
    }
}

CRectangle* ImGuiWindowManager::Find(const ParamKey::Type& name) {
    auto& data = std::find_if(_data_array.begin(), _data_array.end(),
        [&](const GuiWindowData& data) { return (data.name == name); });
    if (data != _data_array.end()) {
        return &(data->rect);
    }
    return nullptr;
}

ImGuiWindowManager& ImGuiWindowManager::GetInstance(void) {
    static ImGuiWindowManager instance;
    return instance;
}

bool ImGuiWindowManager::IsUseGuiItem(void) const {
    bool    over_guirect = false;
    Vector2 mouse_pos;
    g_pInput->GetMousePos(mouse_pos);
    for (const auto& it : _data_array) {
        const bool* show_pointer = (it.active_pointer);
        if (show_pointer == nullptr || *show_pointer == false) {
            continue;
        }
        if (it.rect.CollisionPoint(mouse_pos)) {
            over_guirect = true;
            break;
        }
    }

    bool is_item_active                 = ImGui::IsItemActive();
    bool is_item_hovered                = ImGui::IsItemHovered();
    bool is_item_focused                = ImGui::IsItemFocused();
    bool is_item_clicked                = ImGui::IsItemClicked();
    bool is_item_edited                 = ImGui::IsItemEdited();
    bool is_item_activated              = ImGui::IsItemActivated();
    bool is_item_deactivated            = ImGui::IsItemDeactivated();
    bool is_item_deactivated_after_edit = ImGui::IsItemDeactivatedAfterEdit();
    bool is_item_toggled_open           = ImGui::IsItemToggledOpen();
    bool is_any_item_hovered            = ImGui::IsAnyItemHovered();
    bool is_any_item_active             = ImGui::IsAnyItemActive();
    bool is_any_item_ficysed            = ImGui::IsAnyItemFocused();

    return (
        over_guirect         || is_item_active      || is_item_hovered                ||
        is_item_focused      || is_item_clicked     || is_item_edited                 ||
        is_item_activated    || is_item_deactivated || is_item_deactivated_after_edit ||
        is_item_toggled_open || is_any_item_hovered || is_any_item_active             ||
        is_any_item_ficysed
        );
}
