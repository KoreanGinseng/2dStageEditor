#pragma once
#include "Command.h"
#include "../Stage/MapChip.h"

class AddTextureCommand : public ICommand {
private:

    std::string _path;
    MapChip*    _mapchip;
    std::string _path_prev;

public:

    AddTextureCommand(char* path, MapChip* mapchip);
    virtual ~AddTextureCommand(void);

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

