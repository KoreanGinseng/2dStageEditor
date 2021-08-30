#pragma once
#include <unordered_map>

// ********************************************************************************
/// <summary>
/// �Í��N���X
/// </summary>
// ********************************************************************************
template<class Key, class T > class BlackBoardContainer {
private:

    std::unordered_map<Key, T*> _blackboard {   }; //!<����

    // ********************************************************************************
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    BlackBoardContainer() = default;

public:

    // ********************************************************************************
    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    ~BlackBoardContainer() = default;

    // ********************************************************************************
    /// <summary>
    /// �C���X�^���X�̎擾
    /// </summary>
    /// <returns>�C���X�^���X</returns>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    static BlackBoardContainer& GetInstance() {
        static BlackBoardContainer instance;
        return instance;
    }

    // ********************************************************************************
    /// <summary>
    /// �l�̓o�^
    /// </summary>
    /// <param name="key">�L�[</param>
    /// <param name="data">�l</param>
    /// <param name="is_over">�㏑���t���O</param>
    /// <returns>true : ����, false : ���s</returns>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    bool Register(const Key& key, T* data, bool is_over = true) {
        auto it = _blackboard.find(key);
        if (it == _blackboard.end()) {
            _blackboard[key] = data;
        }
        else if (is_over) {
            it->second = data;
        }
        else {
            return false;
        }
        return true;
    }

    // ********************************************************************************
    /// <summary>
    /// �l�̎擾
    /// </summary>
    /// <param name="key">��������L�[</param>
    /// <returns>nullptr : ���݂��Ȃ�, �l : ����</returns>
    /// <created>inoue,2021/08/27</created>
    /// <changed>inoue,2021/08/27</changed>
    // ********************************************************************************
    T* GetData(const Key& key) {
        auto it = _blackboard.find(key);
        if (it == _blackboard.end()) {
            return nullptr;
        }
        return it->second;
    }

    // ********************************************************************************
    /// <summary>
    /// �S�f�[�^�̍폜
    /// </summary>
    /// <created>inoue,2021/08/30</created>
    /// <changed>inoue,2021/08/30</changed>
    // ********************************************************************************
    void AllDelete() {
        for (auto& it : _blackboard) {
            if (!it.second) {
                continue;
            }
            delete it.second;
            it.second = nullptr;
        }
        Clear();
    }

    // ********************************************************************************
    /// <summary>
    /// �f�[�^�̃N���A
    /// </summary>
    /// <created>inoue,2021/08/30</created>
    /// <changed>inoue,2021/08/30</changed>
    // ********************************************************************************
    void Clear() {
        _blackboard.clear();
    }
};