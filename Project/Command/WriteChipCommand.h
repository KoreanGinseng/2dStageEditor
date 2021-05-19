#pragma once
#include "Command.h"
#include "../Stage/MapChip.h"

class WriteChipCommand : public ICommand
{
private:

    std::vector<std::vector<int>> _chip_data_prev;
    std::vector<std::vector<int>> _chip_data_next;
    MapChip*                      _chip_pointer;
    bool                          _set_next_data;

public:

    WriteChipCommand(MapChip* chip);
    virtual ~WriteChipCommand(void);

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
};

