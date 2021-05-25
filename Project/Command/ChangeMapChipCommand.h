#pragma once
#include "Command.h"
#include "../Stage/Stage.h"

class ChangeMapChipCommand : public ICommand {
private:

    MapChip* _mapchip;
    struct {
        Vector2 _chip_size;
        Vector2 _map_size;
    } _prev, _next;
    using MapData = std::vector<std::vector<int>>;
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

