#pragma once
#include "../Command/Command.h"

/// <summary>
/// コマンド管理クラス
/// シングルトン
/// </summary>
class CommandManager {
private:

    //! 実行リスト
    CommandList _exec_list;

    //! やり直しリスト
    CommandList _redo_list;

    //! リスト最大数(def 50)
    int         _list_max;

protected:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    CommandManager(void);

    /// <summary>
    /// 配列サイズの管理
    /// </summary>
    /// <param name="list">制御するリスト</param>
    void SizeControll(CommandList& list);

public:

    /// <summary>
    /// インスタンスの取得
    /// </summary>
    /// <returns>インスタンス</returns>
    static CommandManager& GetInstance(void);

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~CommandManager(void);

    /// <summary>
    /// 元に戻す
    /// </summary>
    void Undo(void);

    /// <summary>
    /// やり直し
    /// </summary>
    void Redo(void);

    /// <summary>
    /// コマンドの登録
    /// </summary>
    /// <param name="command">登録するコマンド</param>
    void Register(const CommandPtr& command);

    /// <summary>
    /// リストのキャパシティ変更
    /// </summary>
    /// <param name="size">変更サイズ</param>
    void Reserve(int size);

    bool IsUndo(void) const;
    bool IsRedo(void) const;
    const CommandList& GetExecList(void) const;
    const CommandList& GetRedoList(void) const;
};

// シングルトン置き換え
#define theCommandManager CommandManager::GetInstance()