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
    constexpr char* IconUndo            = "IconUndo";
    constexpr char* IconRedo            = "IconRedo";
    constexpr char* IconModeWrite       = "IconModeWrite";
    constexpr char* IconModeErase       = "IconModeErase";
    constexpr char* OpenFile            = "OpenFile";
    constexpr char* Stage               = "Stage";
    constexpr char* TextureArrays       = "TextureArrays";
    constexpr char* ChipScale           = "ChipScale";
    constexpr char* EditScale           = "EditScale";
    constexpr char* DefaultCreate       = "DefaultCreate";
    constexpr char* EditFontColor       = "EditFontColor";
    constexpr char* LogWindow           = "LogWindow";
    constexpr char* MemoryX             = "MemoryX";
    constexpr char* MemoryY             = "MemoryY";
    
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
        IconUndo           ,
        IconRedo           ,
        IconModeWrite      ,
        IconModeErase      ,
        OpenFile           ,
        Stage              ,
        TextureArrays      ,
        ChipScale          ,
        EditScale          ,
        DefaultCreate      ,
        EditFontColor      ,
        LogWindow          ,
        MemoryX            ,
        MemoryY            ,
    };
    using Type = int;
#endif
}