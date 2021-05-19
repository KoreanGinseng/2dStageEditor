#pragma once
#include "Command.h"
#include "../Stage/Stage.h"

class RemoveTextureCommand : public ICommand {
private:

    TextureArray* _texture_array;
    int           _tex_no;
    std::string   _remove_name;
    MapChip*      _mapchip;

public:

    RemoveTextureCommand(TextureArray* tex_array, int select, MapChip* mapchip);
    virtual ~RemoveTextureCommand(void);

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

};

