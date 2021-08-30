#include "ChangeMapChipCommand.h"
#include "../EditorParam/EditorParameter.h"
#include "../Stage/Stage.h"

ChangeMapChipCommand::ChangeMapChipCommand(MapChip* mapchip) 
    : _mapchip(mapchip)
    , _prev()
    , _next() {
    auto mapchip_array = theParam.GetDataPointer<MapChipArray>(ParamKey::MapChipArray);
    const int size_x = _mapchip->GetArraySize().x;
    const int size_y = _mapchip->GetArraySize().y;
    const int size_i = mapchip_array->size();
    for (int i = 0; i < size_i; i++) {
        _prev._mapdata_array.push_back(MapData());
        for (int y = 0; y < size_y; y++) {
            auto& mapdata = _prev._mapdata_array[i];
            mapdata.push_back(std::vector<int>());
            for (int x = 0; x < size_x; x++) {
                mapdata[y].push_back((*mapchip_array)[i].GetMapChip(x, y));
            }
        }
    }
    _prev._chip_size = _mapchip->GetChipSize();
}

ChangeMapChipCommand::~ChangeMapChipCommand(void) {
}

void ChangeMapChipCommand::Execute(void) {
    // 一括で変えているのでレイヤーごとにサイズを変える場合、消す必要あり。
    auto mapchip_array = theParam.GetDataPointer<MapChipArray>(ParamKey::MapChipArray);
    for (auto& mapchip : *mapchip_array) {
        mapchip.Release();
        mapchip.Create(_next.GetMapSize().x, _next.GetMapSize().y);
        mapchip.SetChipSize(_next._chip_size);
    }
    const int size_x = _mapchip->GetArraySize().x;
    const int size_y = _mapchip->GetArraySize().y;
    const int size_i = mapchip_array->size();
    for (int i = 0; i < size_i; i++) {
        auto& mapdata = _next._mapdata_array[i];
        for (int y = 0; y < size_y; y++) {
            for (int x = 0; x < size_x; x++) {
                (*mapchip_array)[i].SetMapChip(x, y, mapdata[y][x]);
            }
        }
    }
}

void ChangeMapChipCommand::Undo(void) {
    // 一括で変えているのでレイヤーごとにサイズを変える場合、消す必要あり。
    auto mapchip_array = theParam.GetDataPointer<MapChipArray>(ParamKey::MapChipArray);
    for (auto& mapchip : *mapchip_array) {
        mapchip.Release();
        mapchip.Create(_prev.GetMapSize().x, _prev.GetMapSize().y);
        mapchip.SetChipSize(_prev._chip_size);
    }
    const int size_x = _mapchip->GetArraySize().x;
    const int size_y = _mapchip->GetArraySize().y;
    const int size_i = mapchip_array->size();
    for (int i = 0; i < size_i; i++) {
        auto& mapdata = _prev._mapdata_array[i];
        for (int y = 0; y < size_y; y++) {
            for (int x = 0; x < size_x; x++) {
                (*mapchip_array)[i].SetMapChip(x, y, mapdata[y][x]);
            }
        }
    }
}

void ChangeMapChipCommand::Register(void) {
    auto mapchip_array = theParam.GetDataPointer<MapChipArray>(ParamKey::MapChipArray);
    const int size_x = _mapchip->GetArraySize().x;
    const int size_y = _mapchip->GetArraySize().y;
    const int size_i = mapchip_array->size();
    for (int i = 0; i < size_i; i++) {
        _next._mapdata_array.push_back(MapData());
        for (int y = 0; y < size_y; y++) {
            auto& mapdata = _next._mapdata_array[i];
            mapdata.push_back(std::vector<int>());
            for (int x = 0; x < size_x; x++) {
                mapdata[y].push_back((*mapchip_array)[i].GetMapChip(x, y));
            }
        }
    }
    _next._chip_size = _mapchip->GetChipSize();
}

std::string ChangeMapChipCommand::GetLog(void) const {
    std::stringstream log;
    log << "ChangeMapChipCommand\n";
    log << "name : " << _mapchip->GetName() << "\n";
    log << "chip_size(x, y) : " << _prev._chip_size.x << ", " << _prev._chip_size.y;
    log << " -> " << _next._chip_size.x << ", " << _next._chip_size.y << "\n";
    log << "map_size(x, y) : " << _prev.GetMapSize().x << ", " << _prev.GetMapSize().y;
    log << " -> " << _next.GetMapSize().x << ", " << _next.GetMapSize().y << "\n";
    return log.str();
}
