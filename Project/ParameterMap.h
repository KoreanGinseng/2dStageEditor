/******************************************************************************/
/*! @file       ParameterMap.h
    @brief      パラメーターマップクラス定義ファイル
*******************************************************************************/

#ifndef PARAMETERMAP_H
#define PARAMETERMAP_H

#include <Mof.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <cassert>

namespace Editor
{
    /*! 利用するキーの置き換え */
    using ParameterMapKeyType = std::string;

/******************************************************************************/
/*! @class CParameterMap
    @brief  パラメーター管理クラス
*******************************************************************************/
    template < typename T >
    class CParameterMap
    {
    public:
        /*! 管理対象 */
        using Parameter = T;

    private:
        /*! 管理マップ */
        std::unordered_map< ParameterMapKeyType, Parameter& > m_Parameters;

    public:

        /******************************************************************************/
        /*! コンストラクタ
        *******************************************************************************/
        CParameterMap()
            : m_Parameters()
        {
        }

        /******************************************************************************/
        /*! デストラクタ
        *******************************************************************************/
        ~CParameterMap() = default;

        /******************************************************************************/
        /*! パラメーターの追加
            @param[in]      key      登録キー
            @param[in]      value    追加するパラメーター
            @return         *this
        *******************************************************************************/
        CParameterMap& Add(const ParameterMapKeyType& key, Parameter& value)
        {
            m_Parameters.insert(std::pair<ParameterMapKeyType, Parameter&>(key, value));
            return *this;
        }

        /******************************************************************************/
        /*! パラメーターの削除
            @param[in]      key      登録キー
            @return         *this
        *******************************************************************************/
        CParameterMap& Delete(const ParameterMapKeyType& key)
        {
            m_Parameters.erase(key);
            return *this;
        }

        /******************************************************************************/
        /*! パラメーターの取得
            @param[in]      key      登録キー
            @return         該当するパラメーター
        *******************************************************************************/
        Parameter& Get(const ParameterMapKeyType& key)
        {
            auto it = m_Parameters.find(key);
            assert(it != m_Parameters.end());
            return it->second;
        }

        /******************************************************************************/
        /*! パラメーターの取得
            @param[in]      key      登録キー
            @return         該当するパラメーター
        *******************************************************************************/
        const Parameter& Get(const ParameterMapKeyType& key) const
        {
            auto it = m_Parameters.find(key);
            assert(it != m_Parameters.end());
            return it->second;
        }

        /******************************************************************************/
        /*! パラメーターの取得
            @param[in]      key      登録キー
            @return         該当するパラメーター
        *******************************************************************************/
        Parameter& operator[] (const ParameterMapKeyType& key)
        {
            return Get(key);
        }

        /******************************************************************************/
        /*! パラメーターの取得
            @param[in]      key      登録キー
            @return         該当するパラメーター
        *******************************************************************************/
        const Parameter& operator[] (const ParameterMapKeyType& key) const
        {
            return Get(key);
        }

        /******************************************************************************/
        /*! パラメーター数の取得
            @return         パラメーター数
        *******************************************************************************/
        size_t GetSize() const
        {
            return m_Parameters.size();
        }

        /******************************************************************************/
        /*! パラメーターマップの取得
            @return         パラメーターマップ
        *******************************************************************************/
        std::unordered_map<ParameterMapKeyType, Parameter>& Parameters()
        {
            return m_Parameters;
        }
    };

    /*! ポインタ置き換え */
    template< typename T > using ParameterMapPtr = std::shared_ptr< CParameterMap<T> >;
}

#endif //PARAMETERMAP_H
