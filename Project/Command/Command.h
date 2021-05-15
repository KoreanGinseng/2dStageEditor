#pragma once
#include <memory>
#include <vector>

// 前方宣言
class ICommand;
using CommandPtr     = std::shared_ptr<ICommand>;
using CommandWeakPtr = std::weak_ptr<ICommand>;
using CommandList    = std::vector<CommandPtr>;

/// <summary>
/// 基底コマンドクラス
/// </summary>
class ICommand {
public:

    /// <summary>
    /// 実行
    /// </summary>
    virtual void Execute(void) = 0;

    /// <summary>
    /// 元に戻す
    /// </summary>
    virtual void Undo(void) = 0;

    /// <summary>
    /// 内部データ登録
    /// </summary>
    virtual void Register(void) = 0;

    /// <summary>
    /// 名前の取得
    /// </summary>
    /// <return>コマンド名</return>
    virtual const char* GetName(void) const = 0;
};

