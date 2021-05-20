#include "CommandManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
CommandManager::CommandManager(void) 
    : _list_max(50)
    , _exec_list()
    , _redo_list() {
    Reserve(_list_max);
}

/// <summary>
/// 配列サイズの管理
/// </summary>
/// <param name="list">制御するリスト</param>
void CommandManager::SizeControll(CommandList& list) {
    if ((int)list.size() < _list_max) {
        return;
    }
    list.erase(list.begin());
}

/// <summary>
/// インスタンスの取得
/// </summary>
/// <returns>インスタンス</returns>
CommandManager& CommandManager::GetInstance(void) {
    static CommandManager instance;
    return instance;
}

/// <summary>
/// デストラクタ
/// </summary>
CommandManager::~CommandManager(void) {
}

/// <summary>
/// 元に戻す
/// </summary>
void CommandManager::Undo(void) {
    if (!IsUndo()) {
        return;
    }
    CommandPtr command = _exec_list.back();
    command->Undo();
    _exec_list.pop_back();
    _redo_list.push_back(command);
    SizeControll(_redo_list);
}

/// <summary>
/// やり直し
/// </summary>
void CommandManager::Redo(void) {
    if (!IsRedo()) {
        return;
    }
    CommandPtr command = _redo_list.back();
    command->Execute();
    _redo_list.pop_back();
    _exec_list.push_back(command);
    SizeControll(_exec_list);
}

/// <summary>
/// コマンドの登録
/// </summary>
/// <param name="command">登録するコマンド</param>
void CommandManager::Register(const CommandPtr& command) {
    command->Execute();
    _exec_list.push_back(command);
    _redo_list.clear();
    SizeControll(_exec_list);
}

/// <summary>
/// リストのキャパシティ変更
/// </summary>
/// <param name="size">変更サイズ</param>
void CommandManager::Reserve(int size) {
    _exec_list.reserve(size);
    _redo_list.reserve(size);
}

bool CommandManager::IsUndo(void) const {
    return (_exec_list.size() > 0);
}

bool CommandManager::IsRedo(void) const {
    return (_redo_list.size() > 0);
}

const CommandList& CommandManager::GetExecList(void) const {
    return _exec_list;
}

const CommandList& CommandManager::GetRedoList(void) const {
    return _redo_list;
}
