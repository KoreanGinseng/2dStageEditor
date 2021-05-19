#pragma once
#include <memory>
#include <vector>

// �O���錾
class ICommand;
using CommandPtr     = std::shared_ptr<ICommand>;
using CommandWeakPtr = std::weak_ptr<ICommand>;
using CommandList    = std::vector<CommandPtr>;

/// <summary>
/// ���R�}���h�N���X
/// </summary>
class ICommand {
public:

    /// <summary>
    /// ���s
    /// </summary>
    virtual void Execute(void) = 0;

    /// <summary>
    /// ���ɖ߂�
    /// </summary>
    virtual void Undo(void) = 0;

    /// <summary>
    /// �����f�[�^�o�^
    /// </summary>
    virtual void Register(void) = 0;
};

