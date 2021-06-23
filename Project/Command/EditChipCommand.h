#pragma once
#include "Command.h"
#include "../Stage/MapChip.h"

class EditChipCommand : public ICommand {
private:

    std::vector<std::vector<int>> _chip_data_prev;
    std::vector<std::vector<int>> _chip_data_next;
    MapChip*                      _chip_pointer;
    bool                          _set_next_data;

public:

    EditChipCommand(MapChip* chip);
    virtual ~EditChipCommand(void);

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

    bool IsChange(void) const;
};

