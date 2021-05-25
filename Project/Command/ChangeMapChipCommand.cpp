#include "ChangeMapChipCommand.h"

ChangeMapChipCommand::ChangeMapChipCommand(MapChip* mapchip) 
    : _mapchip(mapchip)
    , _prev()
    , _next() {
    _prev = *_mapchip;
}

ChangeMapChipCommand::~ChangeMapChipCommand(void) {
}

void ChangeMapChipCommand::Execute(void) {
    *_mapchip = _next;
}

void ChangeMapChipCommand::Undo(void) {
    *_mapchip = _prev;
}

void ChangeMapChipCommand::Register(void) {
    _next = *_mapchip;
}

std::string ChangeMapChipCommand::GetLog(void) const {
    std::stringstream log;
    log << "ChangeMapChipCommand\n";
    log << "name : " << _mapchip->GetName() << "\n";
    log << "chip_size(x, y) : " << _prev.GetChipSize().x << ", " << _prev.GetChipSize().y;
    log << " -> " << _next.GetChipSize().x << ", " << _next.GetChipSize().y << "\n";
    log << "map_size(x, y) : " << _prev.GetArraySize().x << ", " << _prev.GetArraySize().y;
    log << " -> " << _next.GetArraySize().x << ", " << _next.GetArraySize().y << "\n";
    return log.str();
}
