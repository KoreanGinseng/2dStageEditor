#pragma once
#include "Command.h"
#include "../Stage/Stage.h"

class ChangeMapChipCommand : public ICommand {
private:

    using MapData = std::vector<std::vector<int>>;
    MapChip* _mapchip;
    struct Data {
        Vector2 _chip_size;
        std::vector<MapData> _mapdata_array;
        Vector2 GetMapSize() const {
            auto mapdata = _mapdata_array[0];
            return Vector2(mapdata[0].size(), mapdata.size());
        }
    } _prev, _next;
    std::vector<MapData> _map_data;

public:

    ChangeMapChipCommand(MapChip* mapchip);
    virtual ~ChangeMapChipCommand(void);

    /// <summary>
    /// ���s
    /// </summary>
    virtual void Execute(void) override;

    /// <summary>
    /// ���ɖ߂�
    /// </summary>
    virtual void Undo(void) override;

    /// <summary>
    /// �����f�[�^�o�^
    /// </summary>
    virtual void Register(void) override;

    /// <summary>
    /// ���O������̎擾
    /// </summary>
    /// <returns>���O������</returns>
    virtual std::string GetLog(void) const override;
};

