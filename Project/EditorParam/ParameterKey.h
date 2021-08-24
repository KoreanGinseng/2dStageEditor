#pragma once
namespace ParamKey {
#ifdef _DEBUG

    constexpr char* WindowRectArray     = "WindowRectArray";
    constexpr char* ShowLayerWindow     = "ShowLayerWindow";
    constexpr char* ShowMapChipWindow   = "ShowMapChipWindow";
    constexpr char* ShowMapChipWindowChild = "ShowMapChipWindowChild";
    constexpr char* ShowEditWindow      = "ShowEditWindow";
    constexpr char* ShowEditWindowChild = "ShowEditWindowChild";
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
    constexpr char* EditMode            = "EditMode";
    constexpr char* IconNewFile         = "IconNewFile";
    constexpr char* IconOpenFile        = "IconOpenFile";
    constexpr char* IconSaveFile        = "IconSaveFile";
    constexpr char* IconUndo            = "IconUndo";
    constexpr char* IconRedo            = "IconRedo";
    constexpr char* IconModeWrite       = "IconModeWrite";
    constexpr char* IconModeErase       = "IconModeErase";
    constexpr char* IconModeSelect      = "IconModeSelect";
    constexpr char* OpenFile            = "OpenFile";
    constexpr char* Stage               = "Stage";
    constexpr char* TextureArrays       = "TextureArrays";
    constexpr char* ChipScale           = "ChipScale";
    constexpr char* EditScale           = "EditScale";
    constexpr char* DefaultCreate       = "DefaultCreate";
    constexpr char* EditFontColor       = "EditFontColor";
    constexpr char* ShowLogWindow       = "ShowLogWindow";
    constexpr char* MemoryX             = "MemoryX";
    constexpr char* MemoryY             = "MemoryY";
    
    using Type = const char*;
#else
    enum detail {
        WindowRectArray    ,
        ShowLayerWindow    ,
        ShowMapChipWindow  ,
        ShowMapChipWindowChild,
        ShowEditWindow     ,
        ShowEditWindowChild,
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
        EditMode           ,
        IconNewFile        ,
        IconOpenFile       ,
        IconSaveFile       ,
        IconUndo           ,
        IconRedo           ,
        IconModeWrite      ,
        IconModeErase      ,
        IconModeSelect     ,
        OpenFile           ,
        Stage              ,
        TextureArrays      ,
        ChipScale          ,
        EditScale          ,
        DefaultCreate      ,
        EditFontColor      ,
        ShowLogWindow      ,
        MemoryX            ,
        MemoryY            ,
    };
    using Type = int;
#endif
}