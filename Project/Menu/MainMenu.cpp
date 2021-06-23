#include "MainMenu.h"

#include <fstream>
#include <MofImGui/MofImGui.h>
#include "../Manager/ImGuiWindowManager.h"
#include "../Manager/CommandManager.h"
#include "../EditorParam/EditorParameter.h"
#include "../Utilities/EditorUtilities.h"
#include "../Utilities/FileDialog.h"
#include "../Resource.h"
#include "../Stage/Exporter/JsonExporter.h"
#include "../Stage/Exporter/TextExporter.h"
#include "../Stage/Exporter/OriginalExporter.h"
#include "../Stage/Stage.h"
#include "../Stage/Parser/JsonParser.h"
#include "../Stage/Parser/TextParser.h"
#include "../Stage/Parser/OriginalParser.h"

void MainMenu::File(void) {
    if (ImGui::MenuItem("new"       , "Ctrl + N"))         { NewProject();    };
    if (ImGui::MenuItem("open"      , "Ctrl + O"))         { OpenProject();   };
    //if (ImGui::MenuItem("close"     , "Ctrl + Shift + C")) { Close();         };
    ImGui::Separator();
    if (ImGui::MenuItem("save"      , "Ctrl + S"))         { SaveProject();   };
    if (ImGui::MenuItem("save as...", "Ctrl + Shift + S")) { SaveAsProject(); };
    ImGui::Separator();
    if (ImGui::MenuItem("quit"      , "Alt + F4"))         { Quit();          };
}

void MainMenu::Edit(void) {
    if (ImGui::MenuItem("undo", "Ctrl + Z", nullptr, theCommandManager.IsUndo())) { theCommandManager.Undo(); }
    if (ImGui::MenuItem("redo", "Ctrl + Y", nullptr, theCommandManager.IsRedo())) { theCommandManager.Redo(); }
}

void MainMenu::View(void) {
    if (ImGui::MenuItem("chip grid" , "", theParam.GetDataPointer<bool>(ParamKey::ChipGridFlag))) {};
    if (ImGui::MenuItem("edit grid" , "", theParam.GetDataPointer<bool>(ParamKey::EditGridFlag))) {};
    if (ImGui::MenuItem("log window", "", theParam.GetDataPointer<bool>(ParamKey::LogWindow)))    {};
    if (ImGui::MenuItem("memory x"  , "", theParam.GetDataPointer<bool>(ParamKey::MemoryX)))      {};
    if (ImGui::MenuItem("memory y"  , "", theParam.GetDataPointer<bool>(ParamKey::MemoryY)))      {};
}

void MainMenu::Help(void) {
    if (ImGui::MenuItem("open manual", "Ctrl + M")) { OpenManual(); };
    if (ImGui::MenuItem("version"    , "Ctrl + I")) { Version(); };
}

void MainMenu::Show(void) {
    ImGui::BeginMainMenuBar();
    {
        theImGuiWindowManager.Register(ParamKey::MainMenu);
        if (ImGui::BeginMenu("File")) {
            File();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            Edit();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            View();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            Help();
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();
}

bool MainMenu::NewProject(void) {
    char path[PATH_MAX];
    bool array_flag = false;
    bool open = FileDialog::Open(
        g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Save, "プロジェクトの新規作成",
        "教科書対応データ(*.txt)\0*.txt\0\0",
        "txt", path, array_flag
    );
    if (open) {
        if (Close()) {
            auto open_file = theParam.GetDataPointer<std::string>(ParamKey::OpenFile);
            *open_file = path;
            auto def_create_func = theParam.GetDataPointer<std::function<void(void)>>(ParamKey::DefaultCreate);
            (*def_create_func)();
            SaveProject();
            return true;
        }
    }
    return false;
}

bool MainMenu::OpenProject(void) {
    char path[PATH_MAX];
    bool array_flag = false;
    bool open = FileDialog::Open(
        g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Open, "プロジェクトを開く",
        "教科書対応データ(*.txt)\0*.txt\0\0",
        "txt", path, array_flag
    );
    if (open) {
        auto open_file = theParam.GetDataPointer<std::string>(ParamKey::OpenFile);
        std::ifstream ifs(path);
        std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        ParserPtr parser;
        std::string ext = EditorUtilities::GetExt(path);
        if (ext == ".txt") {
            parser = std::make_shared<TextParser>();
        }
        if (parser) {
            if (Close()) {
                ParseData data;
                data.background_array      = theParam.GetDataPointer<BackGroundArray>(ParamKey::BackgroundArray);
                data.collisionrect_array   = theParam.GetDataPointer<CollisionDataArray>(ParamKey::CollisionArray);
                data.mapchip_array         = theParam.GetDataPointer<MapChipArray>(ParamKey::MapChipArray);
                data.mapchip_texture_array = theParam.GetDataPointer<TextureArray>(ParamKey::MapChipTextureArray);
                data.texture_arrays        = theParam.GetDataPointer<std::vector<TextureArray>>(ParamKey::TextureArrays);
                if (parser->Parse(str, &data)) {
                    *open_file = path;
                    return true;
                }
                else {
                    MessageBox(g_pMainWindow->GetWindowHandle(), "ファイルを開けませんでした。", "キャンセルしたよ", MB_OK);
                }
            }
            MessageBox(g_pMainWindow->GetWindowHandle(), "ファイルを開きませんでした。", "キャンセルしたよ", MB_OK);
        }
        else {
            MessageBox(g_pMainWindow->GetWindowHandle(), "ファイルを開けませんでした。", "キャンセルしたよ", MB_OK);
        }
    }
    return false;
}

bool MainMenu::SaveProject(void) {
    auto open_file = theParam.GetDataPointer<std::string>(ParamKey::OpenFile);
    if (*open_file != ".txt" && open_file->length()) {
        std::string ext = EditorUtilities::GetExt(*open_file);
        ExporterPtr exporter = nullptr;
        if (ext == ".map2dproj") {
            exporter = std::make_shared<OriginalExporter>();
        }
        else if (ext == ".json") {
            exporter = std::make_shared<JsonExporter>();
        }
        else if (ext == ".txt") {
            exporter = std::make_shared<TextExporter>();
        }
        if (exporter && exporter->Export(*open_file)) {
            return true;
        }
        MessageBox(g_pMainWindow->GetWindowHandle(), "書き出し失敗しちゃった\nてへぺろ", "失敗", MB_OK);
    }
    else {
        return SaveAsProject();
    }
    return false;
}

bool MainMenu::SaveAsProject(void) {
    char path[PATH_MAX];
    bool array_flag = false;
    bool open = FileDialog::Open(
        g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Save,
        "プロジェクトの保存",
        "教科書対応データ(*.txt)\0*.txt\0\0",
        "txt", path, array_flag
    );
    if (open) {
        auto open_file = theParam.GetDataPointer<std::string>(ParamKey::OpenFile);
        *open_file = path;
        return SaveProject();
    }
    return false;
}

bool MainMenu::Close(void) {
    auto open_file = theParam.GetDataPointer<std::string>(ParamKey::OpenFile);
    if (open_file->length() && MessageBox(g_pMainWindow->GetWindowHandle(),
        "保存されていないデータは復元できません。\nよろしいですか？",
        "ファイルを閉じる", MB_YESNO | MB_ICONEXCLAMATION | MB_APPLMODAL) == IDYES) {
        theParam.GetDataPointer<Stage>(ParamKey::Stage)->Release();
        *open_file = ".txt";
        theCommandManager.Clear();
    }
    return true;
}

void MainMenu::Quit(void) {
    PostQuitMessage(0);
}

void MainMenu::OpenManual(void) {
    std::string path = EditorUtilities::GetResourcePath() + "\\..\\manual.pdf";
    ShellExecute(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void MainMenu::Version(void) {
    std::string appname   = EditorUtilities::GetVersionResourceData()->ProductName;
    std::string version   = EditorUtilities::GetVersionResourceData()->ProductVersion;
    std::string copyright = EditorUtilities::GetVersionResourceData()->LegalCopyright;
    std::string detail = 
        appname + " ver." + version  + "\n"
        "Copyright (c) 2021 " + copyright;
    ShellAbout(g_pMainWindow->GetWindowHandle(), appname.c_str(), detail.c_str(), ::LoadIcon(g_pMainWindow->GetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON)));
}
