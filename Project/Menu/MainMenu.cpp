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
    if (ImGui::MenuItem("close"     , "Ctrl + Shift + C")) { Close();         };
    ImGui::Separator();
    if (ImGui::MenuItem("save"      , "Ctrl + S"))         { SaveProject();   };
    if (ImGui::MenuItem("save as...", "Ctrl + Shift + S")) { SaveAsProject(); };
    ImGui::Separator();
    if (ImGui::MenuItem("export..." , "Ctrl + Shift + E")) { Export();        };
    ImGui::Separator();
    if (ImGui::MenuItem("quit"      , "Alt + F4"))         { Quit();          };
}

void MainMenu::Edit(void) {
    if (ImGui::MenuItem("undo", "Ctrl + Z", nullptr, theCommandManager.IsUndo())) { theCommandManager.Undo(); }
    if (ImGui::MenuItem("redo", "Ctrl + Y", nullptr, theCommandManager.IsRedo())) { theCommandManager.Redo(); }
}

void MainMenu::View(void) {
    ImGui::Checkbox("chip grid", theParam.GetDataPointer<bool>(ParamKey::ChipGridFlag));
    ImGui::Checkbox("edit grid", theParam.GetDataPointer<bool>(ParamKey::EditGridFlag));
}

void MainMenu::Help(void) {
    if (ImGui::MenuItem("open manual", "Ctrl + H")) { OpenManual(); };
    if (ImGui::MenuItem("version")) { Version(); };
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

void MainMenu::NewProject(void) {
    char path[PATH_MAX];
    bool array_flag = false;
    bool open = FileDialog::Open(
        g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Save, "�v���W�F�N�g�̐V�K�쐬",
        "2DMapEditor�v���W�F�N�g(*.map2dproj)\0*.2dmapproj\0Json�f�[�^(*.json)\0*.json\0���ȏ��Ή��f�[�^(*.txt)\0*.txt\0\0",
        "map2dproj\0json\0txt", path, array_flag
    );
    if (open) {
        Close();
        auto open_file = theParam.GetDataPointer<std::string>(ParamKey::OpenFile);
        *open_file = path;
        SaveProject();
    }
}

void MainMenu::OpenProject(void) {
    char path[PATH_MAX];
    bool array_flag = false;
    bool open = FileDialog::Open(
        g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Open, "�v���W�F�N�g���J��",
        "2DMapEditor�v���W�F�N�g(*.map2dproj)\0*.2dmapproj\0Json�f�[�^(*.json)\0*.json\0���ȏ��Ή��f�[�^(*.txt)\0*.txt\0\0",
        "map2dproj\0json\0txt", path, array_flag
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
            Close();
            ParseData data;
            data.background_array      = theParam.GetDataPointer<BackGroundArray>(ParamKey::BackgroundArray);
            data.collisionrect_array   = theParam.GetDataPointer<CollisionDataArray>(ParamKey::CollisionArray);
            data.mapchip_array         = theParam.GetDataPointer<MapChipArray>(ParamKey::MapChipArray);
            data.mapchip_texture_array = theParam.GetDataPointer<TextureArray>(ParamKey::MapChipTextureArray);
            data.texture_arrays        = theParam.GetDataPointer<std::vector<TextureArray>>(ParamKey::TextureArrays);
            if (parser->Parse(str, &data)) {
                *open_file = path;
            }
        }
    }
}

void MainMenu::SaveProject(void) {
    auto open_file = theParam.GetDataPointer<std::string>(ParamKey::OpenFile);
    if (open_file->length()) {
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
        if (exporter) {
            exporter->Export(*open_file);
        }
    }
    else {
        SaveAsProject();
    }
}

void MainMenu::SaveAsProject(void) {
    char path[PATH_MAX];
    bool array_flag = false;
    bool open = FileDialog::Open(
        g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Save,
        "�v���W�F�N�g�̕ۑ�",
        "2DMapEditor�v���W�F�N�g(*.map2dproj)\0*.2dmapproj\0Json�f�[�^(*.json)\0*.json\0���ȏ��Ή��f�[�^(*.txt)\0*.txt\0\0",
        "map2dproj\0json\0txt", path, array_flag
    );
    if (open) {
        auto open_file = theParam.GetDataPointer<std::string>(ParamKey::OpenFile);
        *open_file = path;
        SaveProject();
    }
}

void MainMenu::Close(void) {
    auto open_file = theParam.GetDataPointer<std::string>(ParamKey::OpenFile);
    if (open_file->length() && MessageBox(g_pMainWindow->GetWindowHandle(),
        "�ۑ�����Ă��Ȃ��f�[�^�͕����ł��܂���B\n��낵���ł����H",
        "�t�@�C�������", MB_YESNO | MB_ICONEXCLAMATION | MB_APPLMODAL) == IDYES) {
        theParam.GetDataPointer<Stage>(ParamKey::Stage)->Release();
        *open_file = "";
    }
}

void MainMenu::Export(void) {
    char path[PATH_MAX];
    bool array_flag = false;
    bool open = FileDialog::Open(
        g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Save,
        "�}�b�v�f�[�^�̏����o��",
        "���ȏ��Ή��e�L�X�g�`��(*.txt)\0*.txt\0Json�����o��(*.json)\0*json\0��p2d�}�b�v�f�[�^(*.map2dproj)\0*.map2dproj\0\0",
        "txt\0json\0map2dproj", path, array_flag
    );
    if (open) {
        auto open_file = theParam.GetDataPointer<std::string>(ParamKey::OpenFile);
        *open_file = path;
        SaveProject();
    }
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