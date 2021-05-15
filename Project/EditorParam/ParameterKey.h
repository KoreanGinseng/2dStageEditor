#pragma once
namespace ParamKey {
#ifdef _DEBUG

    constexpr char* WindowRectArray     = "WindowRectArray";
    constexpr char* LayerWindow         = "LayerWindow";
    constexpr char* ChipWindow          = "ChipWindow";
    constexpr char* ChipWindowChild     = "ChipWindowChild";
    constexpr char* EditWindow          = "EditWindow";
    constexpr char* EditWindowChild     = "EditWindowChild";
    constexpr char* MainMenu            = "MainMenu";
    constexpr char* ToolMenu            = "ToolMenu";
    constexpr char* MapChipLayerSelect  = "MapChipLayerSelect";
    constexpr char* MapChipArray        = "MapChipArray";
    constexpr char* MapChipTextureArray = "MapChipTextureArray";
    constexpr char* BackgroundArray     = "BackgroundArray";
    constexpr char* CollisionArray      = "CollisionArray";
    constexpr char* MapChipSelect       = "MapChipSelect";
    constexpr char* EditBackColor       = "EditBackColor";
    constexpr char* ChipGridFlag        = "ChipGridFlag";
    constexpr char* EditGridFlag        = "EditGridFlag";
    constexpr char* EditScroll          = "EditScroll";
    constexpr char* ChipScroll          = "ChipScroll";
    constexpr char* ResourcePath        = "ResourcePath";
    constexpr char* WriteMode           = "WriteMode";
    constexpr char* IconNewFile         = "IconNewFile";
    constexpr char* IconOpenFile        = "IconOpenFile";
    constexpr char* IconSaveFile        = "IconSaveFile";
    constexpr char* IconExportFile      = "IconExportFile";
    constexpr char* IconUndo            = "IconUndo";
    constexpr char* IconRedo            = "IconRedo";
    constexpr char* IconModeWrite       = "IconModeWrite";
    constexpr char* IconModeErase       = "IconModeErase";
    constexpr char* OpenFile            = "OpenFile";
    constexpr char* Stage               = "Stage";
    constexpr char* TextureArrays       = "TextureArrays";
    
    using Type = const char*;
#else
    enum detail {
        WindowRectArray    ,
        LayerWindow        ,
        ChipWindow         ,
        ChipWindowChild    ,
        EditWindow         ,
        EditWindowChild    ,
        MainMenu           ,
        ToolMenu           ,
        MapChipLayerSelect ,
        MapChipArray       ,
        MapChipTextureArray,
        BackgroundArray    ,
        CollisionArray     ,
        MapChipSelect      ,
        EditBackColor      ,
        ChipGridFlag       ,
        EditGridFlag       ,
        EditScroll         ,
        ChipScroll         ,
        ResourcePath       ,
        WriteMode          ,
        IconNewFile        ,
        IconOpenFile       ,
        IconSaveFile       ,
        IconExportFile     ,
        IconUndo           ,
        IconRedo           ,
        IconModeWrite      ,
        IconModeErase      ,
        OpenFile           ,
        Stage              ,
        TextureArrays      ,
    };
    using Type = int;
#endif
}