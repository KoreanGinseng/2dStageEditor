#pragma once
#include "Command.h"
#include "../Stage/MapChip.h"

class EditChipCommand : public ICommand {
private:

    std::vector<std::vector<int>> _chip_data_prev;
    std::vector<std::vector<int>> _chip_data_next;
    MapChip*                      _chip_pointer;
    bool                          _set_next_data;

public:

    EditChipCommand(MapChip* chip);
    virtual ~EditChipCommand(void);

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

    bool IsChange(void) const;
};

