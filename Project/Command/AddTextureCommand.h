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
    /// 実行
    /// </summary>
    virtual void Execute(void) override;

    /// <summary>
    /// 元に戻す
    /// </summary>
    virtual void Undo(void) override;

    /// <summary>
    /// 内部データ登録
    /// </summary>
    virtual void Register(void) override;

    /// <summary>
    /// ログ文字列の取得
    /// </summary>
    /// <returns>ログ文字列</returns>
    virtual std::string GetLog(void) const override;

};

