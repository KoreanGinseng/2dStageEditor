#pragma once
#include <unordered_map>

// ********************************************************************************
/// <summary>
/// 暗黒クラス
/// </summary>
// ********************************************************************************
template<class Key, class T > class BlackBoardContainer {
private:

    std::unordered_map<Key, T*> _blackboard {   }; //!<黒板

    // ********************************************************************************
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    BlackBoardContainer() = default;

public:

    // ********************************************************************************
    /// <summary>
    /// デストラクタ
    /// </summary>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    ~BlackBoardContainer() = default;

    // ********************************************************************************
    /// <summary>
    /// インスタンスの取得
    /// </summary>
    /// <returns>インスタンス</returns>
    /// <created>inoue,2021/08/26</created>
    /// <changed>inoue,2021/08/26</changed>
    // ********************************************************************************
    static BlackBoardContainer& GetInstance() {
        static BlackBoardContainer instance;
        return instance;
    }

    // ********************************************************************************
    /// <summary>
    /// 値の登録
    /// </summary>
    /// <param name="key">キー</param>
    /// <param name="data">値</param>
    /// <param name="is_over">上書きフラグ</param>
    /// <returns>true : 成功, false : 失敗</returns>
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
    /// 値の取得
    /// </summary>
    /// <param name="key">検索するキー</param>
    /// <returns>nullptr : 存在しない, 値 : 成功</returns>
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
    /// 全データの削除
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
    /// データのクリア
    /// </summary>
    /// <created>inoue,2021/08/30</created>
    /// <changed>inoue,2021/08/30</changed>
    // ********************************************************************************
    void Clear() {
        _blackboard.clear();
    }
};