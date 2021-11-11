#include "CommandManager.h"

using namespace Editor;


// ********************************************************************************
/// <summary>
/// 配列サイズの管理
/// </summary>
/// <param name="list">制御するリスト</param>
// ********************************************************************************
void CCommandManager::SizeControll(CommandList& list)
{
    if ((int)list.size() < _list_max)
    {
        return;
    }
    list.erase(list.begin());
}

// ********************************************************************************
/// <summary>
/// コンストラクタ
/// </summary>
// ********************************************************************************
CCommandManager::CCommandManager(void)
    : _list_max(50)
    , _exec_list()
    , _redo_list() 
{
    Reserve(_list_max);
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
// ********************************************************************************
CCommandManager::~CCommandManager(void) 
{
}

// ********************************************************************************
/// <summary>
/// 元に戻す
/// </summary>
// ********************************************************************************
void CCommandManager::Undo(void)
{
    if (!IsUndo())
    {
        return;
    }
    CommandPtr command = _exec_list.back();
    command->Undo();
    _exec_list.pop_back();
    _redo_list.push_back(command);
    SizeControll(_redo_list);
}

// ********************************************************************************
/// <summary>
/// やり直し
/// </summary>
// ********************************************************************************
void CCommandManager::Redo(void) 
{
    if (!IsRedo())
    {
        return;
    }
    CommandPtr command = _redo_list.back();
    command->Execute();
    _redo_list.pop_back();
    _exec_list.push_back(command);
    SizeControll(_exec_list);
}

// ********************************************************************************
/// <summary>
/// コマンドの登録
/// </summary>
/// <param name="command">登録するコマンド</param>
// ********************************************************************************
void CCommandManager::Register(const CommandPtr& command)
{
    command->Execute();
    _exec_list.push_back(command);
    _redo_list.clear();
    SizeControll(_exec_list);
}

// ********************************************************************************
/// <summary>
/// リストのキャパシティ変更
/// </summary>
/// <param name="size">変更サイズ</param>
// ********************************************************************************
void CCommandManager::Reserve(int size) 
{
    _exec_list.reserve(size);
    _redo_list.reserve(size);
}

// ********************************************************************************
/// <summary>
/// Undo可能か
/// </summary>
/// <returns>true : Undo可能, false : Undo不可能</returns>
// ********************************************************************************
bool CCommandManager::IsUndo(void) const 
{
    return (_exec_list.size() > 0);
}

// ********************************************************************************
/// <summary>
/// Redo可能か
/// </summary>
/// <returns>true : Redo可能, false : Redo不可能</returns>
// ********************************************************************************
bool CCommandManager::IsRedo(void) const
{
    return (_redo_list.size() > 0);
}

// ********************************************************************************
/// <summary>
/// 実行リストの取得
/// </summary>
/// <returns>実行リスト</returns>
// ********************************************************************************
const CommandList& CCommandManager::GetExecList(void) const
{
    return _exec_list;
}

// ********************************************************************************
/// <summary>
/// やり直しリストの取得
/// </summary>
/// <returns>やり直しリスト</returns>
// ********************************************************************************
const CommandList& CCommandManager::GetRedoList(void) const 
{
    return _redo_list;
}

// ********************************************************************************
/// <summary>
/// リストのクリア
/// </summary>
// ********************************************************************************
void CCommandManager::Clear(void)
{
    _exec_list.clear();
    _redo_list.clear();
}
