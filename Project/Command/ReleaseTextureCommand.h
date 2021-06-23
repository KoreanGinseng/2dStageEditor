#pragma once
#include <Mof.h>
#include "Command.h"

class ReleaseTextureCommand : public ICommand {
private:

    std::string _path;
    CTexture*   _texture;

public:

    ReleaseTextureCommand(CTexture* texture);
    virtual ~ReleaseTextureCommand(void);

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

