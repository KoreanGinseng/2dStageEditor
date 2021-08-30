#pragma once
#include <Mof.h>
#include "Command.h"

class LoadTextureCommand : public ICommand {
private:

    std::string _path;
    std::string _path_prev;
    CTexture*   _texture;

public:

    LoadTextureCommand(char* path, CTexture* texture);
    virtual ~LoadTextureCommand(void);

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

