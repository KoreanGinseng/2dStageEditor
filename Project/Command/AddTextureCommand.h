#pragma once
#include "Command.h"
#include "../Stage/Stage.h"

class AddTextureCommand : public ICommand {
private:

    std::string   _path;
    std::string   _path_prev;
    TextureArray* _texture_array;
    MapChip*      _mapchip;

public:

    AddTextureCommand(char* path, TextureArray* texture_array, MapChip* mapchip = nullptr);
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

