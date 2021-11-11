#pragma once
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

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

    /// <summary>
    /// ���O������̎擾
    /// </summary>
    /// <returns>���O������</returns>
    virtual std::string GetLog(void) const = 0;
};
