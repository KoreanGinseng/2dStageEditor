#include "../ImGui/MofImGui.h"

#include "ToolMenu.h"
#include "MainMenu.h"
#include "../EditorParam/EditorParameter.h"
#include "../Manager/ImGuiWindowManager.h"
#include "../Manager/CommandManager.h"
#include "../Window/WindowDefine.h"
#include "../Utilities/ToolIcon.h"
#include "../Utilities/EditorUtilities.h"

void ToolMenu::Show(void) {
    ImGuiWindowFlags window_flags = EditorUtilities::GetImGuiDefWindowFlag();

    float w = g_pGraphics->GetTargetWidth();
    ImGui::SetNextWindowPos (ImVec2(0, k_main_menu_height));
    ImGui::SetNextWindowSize(ImVec2(w, k_tool_menu_height));
    ImGui::SetNextWindowBgAlpha(1);
    const ImVec2 def_window_padding = ImGui::GetStyle().WindowPadding;
    const ImVec2 def_item_spacing   = ImGui::GetStyle().ItemSpacing;
    ImGui::GetStyle().WindowPadding = ImVec2(2, 2);
    ImGui::GetStyle().ItemSpacing   = ImVec2(2, 2);
    ImGui::Begin("tool menu", 0, window_flags); {
        theImGuiWindowManager.Register(ParamKey::ToolMenu);

        ImVec2 size = ImVec2(22, 22);
        ImVec2 uv0 = ImVec2(0, 0);
        ImVec2 uv1 = ImVec2(1, 1);
        ImGui::Dummy(ImVec2(size.x * 0.5f, size.y)); ImGui::SameLine();
        if (ImGui::ImageButton(ToolIcon::GetTexture(ParamKey::IconNewFile), size)) {
            MainMenu::NewProject();
        }ImGui::SameLine();
        if (ImGui::ImageButton(ToolIcon::GetTexture(ParamKey::IconOpenFile), size)) {
            MainMenu::OpenProject();
        }ImGui::SameLine();
        if (ImGui::ImageButton(ToolIcon::GetTexture(ParamKey::IconSaveFile), size)) {
            MainMenu::SaveProject();
        }ImGui::SameLine();
        ImGui::Dummy(ImVec2(size.x * 2.5f, size.y)); ImGui::SameLine();
        if (ImGui::ImageButton(ToolIcon::GetTexture(ParamKey::IconUndo), size)) {
            theCommandManager.Undo();
        }ImGui::SameLine();
        if (ImGui::ImageButton(ToolIcon::GetTexture(ParamKey::IconRedo), size)) {
            theCommandManager.Redo();
        }ImGui::SameLine();
        ImGui::Dummy(ImVec2(size.x * 3.5f, size.y)); ImGui::SameLine();
        ImVec4 on_color     = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
        ImVec4 off_color    = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        ImVec4 def_color    = ImGui::GetStyle().Colors[ImGuiCol_Button];
        ImVec4 write_color  = (EditorUtilities::IsWriteMode()  ? on_color : off_color);
        ImVec4 erase_color  = (EditorUtilities::IsDeleteMode() ? on_color : off_color);
        ImVec4 select_color = (EditorUtilities::IsSelectMode() ? on_color : off_color);
        ImGui::GetStyle().Colors[ImGuiCol_Button] = (EditorUtilities::IsWriteMode() ? write_color : def_color);
        if (ImGui::ImageButton(ToolIcon::GetTexture(ParamKey::IconModeWrite), size, uv0, uv1, -1, write_color)) {
            EditorUtilities::SetWriteMode();
        }ImGui::SameLine();
        ImGui::GetStyle().Colors[ImGuiCol_Button] = (EditorUtilities::IsDeleteMode() ? erase_color : def_color);
        if (ImGui::ImageButton(ToolIcon::GetTexture(ParamKey::IconModeErase), size, uv0, uv1, -1, erase_color)) {
            EditorUtilities::SetDeleteMode();
        }ImGui::SameLine();
        ImGui::GetStyle().Colors[ImGuiCol_Button] = (EditorUtilities::IsSelectMode() ? select_color : def_color);
        if (ImGui::ImageButton(ToolIcon::GetTexture(ParamKey::IconModeSelect), size, uv0, uv1, -1, select_color)) {
            EditorUtilities::SetSelectMode();
        }ImGui::SameLine();
        ImGui::GetStyle().Colors[ImGuiCol_Button] = def_color;
    }
    ImGui::GetStyle().WindowPadding = def_window_padding;
    ImGui::GetStyle().ItemSpacing   = def_item_spacing;
    ImGui::End();
}
