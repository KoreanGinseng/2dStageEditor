#pragma once
#include <Mof.h>
#include "ImGui/MofImGui.h"
#include "Utilities/BlackBoardContainer.h"
#include "Utilities/EditorUtilities.h"
#include "Utilities/InResourceTexture.h"
#include "Resource.h"

constexpr char kEmptyFileName[] = ".txt";

enum class EditorMode {
    write,
    erase,
    select,
    layout,
};

extern EditorMode editor_mode;

//BB_string keys
enum class BB_string {
    DefaultPath,
};

#define theBB_string BlackBoardContainer<BB_string, std::string>::GetInstance()

//SHOW
enum class ShowFlag {
    layer,
    layer_data,
    background,
    color,
    textures,
    maphcip_texture,
    rect_edit,
    map_edit,
    map_edit_tab,
    map_preview_tab,
    count,
};

constexpr char* show_flag_names[] = {
    "layer",
    "layer_data",
    "background",
    "color",
    "textures",
    "maphcip_texture",
    "rect_edit",
    "map_edit",
    "map_edit_tab",
    "map_preview_tab",
    "count",
};

extern bool show_flags[static_cast<int>(ShowFlag::count)];

//BB_ImVec2 Keys
enum class BB_ImVec2 {
    MainMenuPos,
    MainMenuSize,
    ToolBarPos,
    ToolBarSize,
};

#define theBB_ImVec2 BlackBoardContainer<BB_ImVec2, ImVec2>::GetInstance()

//BB_InResourceTexture Keys
enum class BB_InResourceTexture {
    new_document,
    folder,
    diskette,
    back_arrow,
    redo_arrow,
    edit,
    eraser_tool,
    selection,
    layout,
    count,
};

constexpr int in_resource_texture_ids[] = {
    IDB_PNG1,
    IDB_PNG2,
    IDB_PNG3,
    IDB_PNG4,
    IDB_PNG5,
    IDB_PNG6,
    IDB_PNG7,
    IDB_PNG8,
    IDB_PNG9,
    -1, //COUNT END
};

constexpr char* in_resource_texture_names[] = {
    "icons/new-document.png",
    "icons/folder.png",
    "icons/diskette.png",
    "icons/back-arrow.png",
    "icons/redo-arrow.png",
    "icons/edit.png",
    "icons/eraser-tool.png",
    "icons/selection.png",
    "icons/layout.png",
    "count",
};

#define theBB_InResourceTexture BlackBoardContainer<BB_InResourceTexture, CInResourceTexture>::GetInstance()