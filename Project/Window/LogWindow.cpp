#include "../ImGui/MofImGui.h"

#include "LogWindow.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"
#include "../Manager/CommandManager.h"
#include "../Manager/ImGuiWindowManager.h"
#include "../Utilities/FileDialog.h"
#include <filesystem>

int LogWindow::_save_log_count = 5;

void LogWindow::Show(void) {

    ImGui::Begin("log window", theParam.GetDataPointer<bool>(ParamKey::ShowLogWindow)); {
        theImGuiWindowManager.Register(ParamKey::ShowLogWindow);

        if (ImGui::BeginTabBar("log_tab")) {

            if (ImGui::BeginTabItem("exec")) {
                if (ImGui::BeginChild("exec_tab", ImVec2(), true)) {
                    const auto& exec_list = theCommandManager.GetExecList();
                    const int   cnt       = exec_list.size();
                    for (int i = 0; i < cnt; i++) {
                        const auto log   = exec_list[i]->GetLog();
                        const int  index = log.find_first_of('\n');
                        if (ImGui::TreeNode((log.substr(0, index) + "##" + std::to_string(i)).c_str())) {
                            ImGui::Text(log.substr(index + 1, log.length()).c_str());
                            ImGui::TreePop();
                        }
                    }
                    ImGui::EndChild();
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("redo")) {
                if (ImGui::BeginChild("redo_tab", ImVec2(), true)) {
                    const auto& redo_list = theCommandManager.GetRedoList();
                    const int   cnt       = redo_list.size();
                    for (int i = 0; i < cnt; i++) {
                        const auto log   = redo_list[i]->GetLog();
                        const int  index = log.find_first_of('\n');
                        if (ImGui::TreeNode((log.substr(0, index) + "##" + std::to_string(i)).c_str())) {
                            ImGui::Text(log.substr(index + 1, log.length()).c_str());
                            ImGui::TreePop();
                        }
                    }
                    ImGui::EndChild();
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

    }
    ImGui::End();
}

void LogWindow::Save(void) {
    int file_count = 0;
    std::vector<std::filesystem::path> paths;
    for (const auto& it : std::filesystem::directory_iterator(".")) {
        file_count++;
        paths.push_back(it);
    }
    std::sort(paths.begin(), paths.end());
    if (file_count >= _save_log_count - 1) {
        for (int i = 0; i < file_count - (_save_log_count - 1); i++) {
            std::filesystem::remove(*(paths.begin() + i));
        }
    }

    std::string file_name = EditorUtilities::getDatetimeStr() + ".txt";
    FILE* file_pointer = fopen(file_name.c_str(), "wt");
    if (file_pointer == NULL) {
        return;
    }
    {
        fprintf_s(file_pointer, "\nexec_list");
        fprintf_s(file_pointer, "\n-----------------------------------------------\n");

        const auto& exec_list = theCommandManager.GetExecList();
        const int   cnt       = exec_list.size();
        for (int i = 0; i < cnt; i++) {
            const auto log = exec_list[i]->GetLog();
            fprintf_s(file_pointer, "%s\n", log.c_str());
        }
    }
    {
        fprintf_s(file_pointer, "\nredo_list");
        fprintf_s(file_pointer, "\n-----------------------------------------------\n");

        const auto& redo_list = theCommandManager.GetRedoList();
        const int   cnt       = redo_list.size();
        for (int i = 0; i < cnt; i++) {
            const auto log = redo_list[i]->GetLog();
            fprintf_s(file_pointer, "%s\n", log.c_str());
        }
    }

    fclose(file_pointer);
}
