#pragma once
#include "../Command/Command.h"

/// <summary>
/// �R�}���h�Ǘ��N���X
/// �V���O���g��
/// </summary>
class CommandManager {
private:

    //! ���s���X�g
    CommandList _exec_list;

    //! ��蒼�����X�g
    CommandList _redo_list;

    //! ���X�g�ő吔(def 50)
    int         _list_max;

protected:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    CommandManager(void);

    /// <summary>
    /// �z��T�C�Y�̊Ǘ�
    /// </summary>
    /// <param name="list">���䂷�郊�X�g</param>
    void SizeControll(CommandList& list);

public:

    /// <summary>
    /// �C���X�^���X�̎擾
    /// </summary>
    /// <returns>�C���X�^���X</returns>
    static CommandManager& GetInstance(void);

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~CommandManager(void);

    /// <summary>
    /// ���ɖ߂�
    /// </summary>
    void Undo(void);

    /// <summary>
    /// ��蒼��
    /// </summary>
    void Redo(void);

    /// <summary>
    /// �R�}���h�̓o�^
    /// </summary>
    /// <param name="command">�o�^����R�}���h</param>
    void Register(const CommandPtr& command);

    /// <summary>
    /// ���X�g�̃L���p�V�e�B�ύX
    /// </summary>
    /// <param name="size">�ύX�T�C�Y</param>
    void Reserve(int size);

    bool IsUndo(void) const;
    bool IsRedo(void) const;
    const CommandList& GetExecList(void) const;
    const CommandList& GetRedoList(void) const;
};

// �V���O���g���u������
#define theCommandManager CommandManager::GetInstance()