#pragma once
#include "Command.h"
#include "../Stage/Stage.h"

class ChangeMapChipCommand : public ICommand {
private:

    MapChip* _mapchip;
    struct {
        Vector2 _chip_size;
        Vector2 _map_size;
    } _prev, _next;
    using MapData = std::vector<std::vector<int>>;
    std::vector<MapData> _map_data;

public:

    ChangeMapChipCommand(MapChip* mapchip);
    virtual ~ChangeMapChipCommand(void);

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

