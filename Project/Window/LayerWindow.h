#pragma once
#include <Mof.h>
#include "../Stage/Stage.h"

class LayerWindow {
private:

    std::vector<MapChip>*       _mapchip_array;
    std::vector<CTexture>*      _mapchip_texture_array;
    std::vector<BackGround>*    _background_array;
    std::vector<CollisionData>* _collision_array;
    int                         _select_chip_layer;
    int                         _select_background_layer;
    int                         _select_collision_layer;

    void ShowMapChipLayerTab(void);
    void ShowBackgroundLayerTab(void);
    void ShowCollisionLayerTab(void);
    void ShowColorEditTab(void);

    void ShowAddLayerModal(void);

public:

    void Initialize(void);
    void Show(void);
};

