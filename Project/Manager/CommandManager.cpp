#include "CommandManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CommandManager::CommandManager(void) 
    : _list_max(50)
    , _exec_list()
    , _redo_list() {
    Reserve(_list_max);
}

/// <summary>
/// �z��T�C�Y�̊Ǘ�
/// </summary>
/// <param name="list">���䂷�郊�X�g</param>
void CommandManager::SizeControll(CommandList& list) {
    if ((int)list.size() < _list_max) {
        return;
    }
    list.erase(list.begin());
}

/// <summary>
/// �C���X�^���X�̎擾
/// </summary>
/// <returns>�C���X�^���X</returns>
CommandManager& CommandManager::GetInstance(void) {
    static CommandManager instance;
    return instance;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CommandManager::~CommandManager(void) {
}

/// <summary>
/// ���ɖ߂�
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
/// ��蒼��
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
/// �R�}���h�̓o�^
/// </summary>
/// <param name="command">�o�^����R�}���h</param>
void CommandManager::Register(const CommandPtr& command) {
    command->Execute();
    _exec_list.push_back(command);
    _redo_list.clear();
    SizeControll(_exec_list);
}

/// <summary>
/// ���X�g�̃L���p�V�e�B�ύX
/// </summary>
/// <param name="size">�ύX�T�C�Y</param>
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
