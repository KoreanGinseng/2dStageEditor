#include "CommandManager.h"

using namespace Editor;


// ********************************************************************************
/// <summary>
/// �z��T�C�Y�̊Ǘ�
/// </summary>
/// <param name="list">���䂷�郊�X�g</param>
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
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
// ********************************************************************************
CCommandManager::~CCommandManager(void) 
{
}

// ********************************************************************************
/// <summary>
/// ���ɖ߂�
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
/// ��蒼��
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
/// �R�}���h�̓o�^
/// </summary>
/// <param name="command">�o�^����R�}���h</param>
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
/// ���X�g�̃L���p�V�e�B�ύX
/// </summary>
/// <param name="size">�ύX�T�C�Y</param>
// ********************************************************************************
void CCommandManager::Reserve(int size) 
{
    _exec_list.reserve(size);
    _redo_list.reserve(size);
}

// ********************************************************************************
/// <summary>
/// Undo�\��
/// </summary>
/// <returns>true : Undo�\, false : Undo�s�\</returns>
// ********************************************************************************
bool CCommandManager::IsUndo(void) const 
{
    return (_exec_list.size() > 0);
}

// ********************************************************************************
/// <summary>
/// Redo�\��
/// </summary>
/// <returns>true : Redo�\, false : Redo�s�\</returns>
// ********************************************************************************
bool CCommandManager::IsRedo(void) const
{
    return (_redo_list.size() > 0);
}

// ********************************************************************************
/// <summary>
/// ���s���X�g�̎擾
/// </summary>
/// <returns>���s���X�g</returns>
// ********************************************************************************
const CommandList& CCommandManager::GetExecList(void) const
{
    return _exec_list;
}

// ********************************************************************************
/// <summary>
/// ��蒼�����X�g�̎擾
/// </summary>
/// <returns>��蒼�����X�g</returns>
// ********************************************************************************
const CommandList& CCommandManager::GetRedoList(void) const 
{
    return _redo_list;
}

// ********************************************************************************
/// <summary>
/// ���X�g�̃N���A
/// </summary>
// ********************************************************************************
void CCommandManager::Clear(void)
{
    _exec_list.clear();
    _redo_list.clear();
}
