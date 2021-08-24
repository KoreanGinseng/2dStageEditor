#pragma once
#include <Mof.h>
#include "../Stage/Stage.h"
#include "../Command/Command.h"

constexpr int change_wait_frame = 15;

class LayerWindow {
private:

    std::vector<MapChip>*       _mapchip_array;
    std::vector<CTexture>*      _mapchip_texture_array;
    std::vector<BackGround>*    _background_array;
    std::vector<CollisionData>* _collision_array;
    int                         _select_chip_layer;
    int                         _select_background_layer;
    int                         _select_collision_layer;
    CommandPtr                  _change_mapchip_command;
    int                         _change_frame_count;
    bool                        _change_hold_flag;

    // LayerTab
    void ShowMapChipLayerTab(void);
    void ShowLayerInfo(void);
    void ShowBackGround(void);
    void LoadBackGroundTexture(void);
    void RemoveBackGroundTexture(void);
    void ShowLayerData(void);
    void ShowMapData(void);
    void ShowChipBackColor(void);

    void ShowBackGroundLayerTab(void);
    void ShowColorEditTab(void);

public:

    void Initialize(void);
    void Show(void);
};

