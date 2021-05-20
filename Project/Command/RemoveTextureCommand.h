#pragma once
#include "Command.h"
#include "../Stage/Stage.h"

class RemoveTextureCommand : public ICommand {
private:

    TextureArray* _texture_array;
    int           _tex_no;
    std::string   _remove_name;
    MapChip*      _mapchip;

public:

    RemoveTextureCommand(TextureArray* tex_array, int select, MapChip* mapchip);
    virtual ~RemoveTextureCommand(void);

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

