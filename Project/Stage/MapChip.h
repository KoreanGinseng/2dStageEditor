#pragma once
#include <Mof.h>

class MapChip {
private:

    bool        _show_flag         { true };
    std::string _name              { "mapchip" };
    MofU32      _back_color        { MOF_XRGB(16, 16, 16) };
    int         _texture_no        { -1 };
    bool        _texture_array_flag{ false };
    Vector2     _chip_size         { 0, 0 };
    Vector2     _array_size        { 0, 0 };
    int*        _mapchip_array     { nullptr };

public:

    bool Create(int x, int y);

    void Release(void);

    int* GetMapChip(void);
    int  GetMapChip(int x, int y) const;
    void SetMapChip(int x, int y, int n);

    int GetTextureNo(void) const;
    void SetTextureNo(int no);

    Vector2 GetChipSize(void) const;
    void SetChipSize(const Vector2& size);

    Vector2 GetArraySize(void) const;
    int GetArrayCount(void) const;

    std::string GetName(void) const;
    void SetName(const std::string& name);

    MofU32 GetBackColor(void) const;
    void SetBackColor(MofU32 col);

    bool IsShow(void) const;
    void SetShow(bool b);

    bool IsTextureArray(void) const;
    void SetTextureArray(bool b);
};

