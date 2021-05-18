#include "MapChip.h"

bool MapChip::Create(int x, int y) {
    int* tmp  = (int*)calloc(x * y, sizeof(int));
    if (tmp == NULL) {
        return false;
    }
    int  xcnt = min(x, _array_size.x);
    int  ycnt = min(y, _array_size.y);
    for (int ty = 0; ty < ycnt; ty++) {
        for (int tx = 0; tx < xcnt; tx++) {
            int from_index = ty * _array_size.x + tx;
            int to_index   = ty * x + tx;
            tmp[to_index]  = _mapchip_array[from_index];
        }
    }
    free(_mapchip_array);
    _mapchip_array = tmp;
    _array_size.x  = x;
    _array_size.y  = y;
    return true;
}

void MapChip::Release(void) {
    if (GetArrayCount() > 0) {
        free(_mapchip_array);
        _mapchip_array = nullptr;
    }
}

int* MapChip::GetMapChip(void) {
    return _mapchip_array;
}

int MapChip::GetMapChip(int x, int y) const {
    if (_mapchip_array == nullptr || 
        x >= _array_size.x        ||
        y >= _array_size.y        ||
        x < 0 || y < 0) {
        return -1;
    }
    return _mapchip_array[y * (int)_array_size.x + x];
}

void MapChip::SetMapChip(int x, int y, int n) {
    if (_mapchip_array == nullptr ||
        x >= _array_size.x        ||
        y >= _array_size.y        ||
        x < 0 || y < 0) {
        return;
    }
    _mapchip_array[y * (int)_array_size.x + x] = n;
}

int MapChip::GetTextureNo(void) const {
    return _texture_no;
}

void MapChip::SetTextureNo(int no) {
    _texture_no = no;
}

Vector2 MapChip::GetChipSize(void) const {
    return _chip_size;
}

void MapChip::SetChipSize(const Vector2& size) {
    _chip_size = size;
}

Vector2 MapChip::GetArraySize(void) const {
    return _array_size;
}

int MapChip::GetArrayCount(void) const {
    return (int)_array_size.x * (int)_array_size.y;
}

std::string MapChip::GetName(void) const {
    return _name;
}

void MapChip::SetName(const std::string& name) {
    _name = name;
}

MofU32 MapChip::GetBackColor(void) const {
    return _back_color;
}

void MapChip::SetBackColor(MofU32 col) {
    _back_color = col;
}

bool MapChip::IsShow(void) const {
    return _show_flag;
}

void MapChip::SetShow(bool b) {
    _show_flag = b;
}

bool MapChip::IsShowNo(void) const {
    return _show_no_flag;
}

void MapChip::SetShowNo(bool b) {
    _show_no_flag = b;
}

bool MapChip::IsTextureArray(void) const {
    return _texture_array_flag;
}

void MapChip::SetTextureArray(bool b) {
    _texture_array_flag = b;
}

CRectangle MapChip::GetChipViewRect(int chip_no, int chip_x) {
    if (!_texture_array_flag) {
        return CRectangle (
            _chip_size.x * (chip_no % chip_x),
            _chip_size.y * (chip_no / chip_x),
            _chip_size.x * (chip_no % chip_x + 1),
            _chip_size.y * (chip_no / chip_x + 1)
        );
    }
    if (_chip_view_rect == CRectangle()) {
        _chip_view_rect.SetValue(0, 0, _chip_size.x, _chip_size.y);
    }
    return _chip_view_rect;
}

CRectangle MapChip::SetChipViewRect(const CRectangle& rect) {
    return _chip_view_rect = rect;
}
