#include "WriteChipCommand.h"

WriteChipCommand::WriteChipCommand(MapChip* chip)
	: _chip_data_prev()
	, _chip_data_next()
    , _chip_pointer(chip)
    , _set_next_data(false) {
	const int ycnt = _chip_pointer->GetArraySize().y;
	const int xcnt = _chip_pointer->GetArraySize().x;
	_chip_data_prev.resize(ycnt);
	_chip_data_next.resize(ycnt);
	for (int y = 0; y < ycnt; y++) {
		_chip_data_prev[y].resize(xcnt);
		_chip_data_next[y].resize(xcnt);
		for (int x = 0; x < xcnt; x++) {
			_chip_data_prev[y][x] = _chip_pointer->GetMapChip(x, y);
		}
	}
}

WriteChipCommand::~WriteChipCommand(void) {
}

void WriteChipCommand::Execute(void) {
	const int ycnt = _chip_data_next.size();
	for (int y = 0; y < ycnt; y++) {
		const int xcnt = _chip_data_next[y].size();
		for (int x = 0; x < xcnt; x++) {
			if (!_set_next_data) {
				_chip_data_next[y][x] = _chip_pointer->GetMapChip(x, y);
			}
			else {
				_chip_pointer->SetMapChip(x, y, _chip_data_next[y][x]);
			}
		}
	}
	_set_next_data = true;
}

void WriteChipCommand::Undo(void) {
	const int ycnt = _chip_data_next.size();
	for (int y = 0; y < ycnt; y++) {
		const int xcnt = _chip_data_next[y].size();
		for (int x = 0; x < xcnt; x++) {
			_chip_pointer->SetMapChip(x, y, _chip_data_prev[y][x]);
		}
	}
}

void WriteChipCommand::Register(void) {
}
