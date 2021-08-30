#pragma once
#include "Command.h"

class AddTextureArrayCommand : public ICommand {
private:

    std::vector<std::string> _path_array;
    int                      _tex_no;
    bool                     _array_flag;
    int                      _insert_point;
    bool                     _set_flag;

public:

    AddTextureArrayCommand(char* path, int tex_no, bool array_flag);
    virtual ~AddTextureArrayCommand(void);

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

