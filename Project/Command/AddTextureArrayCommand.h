#pragma once
#include "Command.h"

class AddTextureArrayCommand : public ICommand {
private:

    std::vector<std::string> _path_array;
    int                      _tex_no;
    bool                     _array_flag;
    int                      _insert_point;
    bool                     _set_flag;

public:

    AddTextureArrayCommand(char* path, int tex_no, bool array_flag);
    virtual ~AddTextureArrayCommand(void);

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

