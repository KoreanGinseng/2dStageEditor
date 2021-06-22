#pragma once
#include "Command.h"
#include "../Stage/Stage.h"

class ChangeMapChipCommand : public ICommand {
private:

    using MapData = std::vector<std::vector<int>>;
    MapChip* _mapchip;
    struct Data {
        Vector2 _chip_size;
        std::vector<MapData> _mapdata_array;
        Vector2 GetMapSize() const {
            auto mapdata = _mapdata_array[0];
            return Vector2(mapdata[0].size(), mapdata.size());
        }
    } _prev, _next;
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

