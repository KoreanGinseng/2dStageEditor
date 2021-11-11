/******************************************************************************/
/*! @file       BlackBoard.h
    @brief      ブラックボードクラス定義ファイル
*******************************************************************************/

#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "ParameterMap.h"
#include <tuple>

namespace Editor
{

    /******************************************************************************/
    /*! @class CBlackboard
        @brief  ブラックボードクラス
    *******************************************************************************/
    template < typename... Types >
    class CBlackboard
    {
    private:
        /*! データ */
        std::tuple<ParameterMapPtr<Types>...> m_Elements;

        /*! データタイプ置き換え */
        template < MofU32 Index >
        using elementType = typename std::tuple_element<Index, decltype(m_Elements)>::type;

        /*! エレメントチェック置き換え */
        template < typename T, MofU32 Index >
        using IsElement = std::is_same<elementType<Index>, T>;

    /******************************************************************************/
    /*! @struct FindElement
        @brief  エレメントの検索構造体
    *******************************************************************************/
        template < typename T, MofU32 Index = 0 >
    struct FindElement : public std::conditional_t
            <
                IsElement<T, Index>::value,
                std::integral_constant< decltype(Index), Index>,
                FindElement<T, Index + 1>
            >
        {
        };

    public:

        /******************************************************************************/
        /*! コンストラクタ
        *******************************************************************************/
        CBlackboard()
            : m_Elements()
        {
        }

        /******************************************************************************/
        /*! デストラクタ
        *******************************************************************************/
        ~CBlackboard() = default;

        /******************************************************************************/
        /*! 登録コンテナの取得
            @return         登録コンテナ
        *******************************************************************************/
        template < typename T >
        ParameterMapPtr<T>& Get()
        {
            constexpr MofU32 index = FindElement<ParameterMapPtr<T>>::value;
            auto& result = std::get<index>(m_Elements);
            if (result == nullptr)
            {
                result = std::make_shared<CParameterMap<T>>();
            }
            return result;
        }

        /******************************************************************************/
        /*! 登録コンテナの取得
            @return         登録コンテナ
        *******************************************************************************/
        template < typename T >
        const ParameterMapPtr<T>& Get() const
        {
            return const_cast<CBlackboard*>(this)->Get<T>();
        }

        /******************************************************************************/
        /*! 登録パラメーターの取得
            @param[in]      key    登録キー
            @return         登録パラメーター
        *******************************************************************************/
        template < typename T >
        T& Get(const ParameterMapKeyType& key)
        {
            auto& result = Get<T>();
            return result->Get(key);
        }

        /******************************************************************************/
        /*! 登録パラメーターの取得
            @param[in]      key    登録キー
            @return         登録パラメーター
        *******************************************************************************/
        template < typename T >
        const T& Get(const ParameterMapKeyType& key) const
        {
            return const_cast<CBlackboard*>(this)->Get<T>(key);
        }

        /******************************************************************************/
        /*! 登録パラメーター数の取得
            @return         登録パラメーター数
        *******************************************************************************/
        template < typename T >
        size_t GetSize() const
        {
            return Get<T>()->GetSize();
        }
    };

    /*! ポインタ置き換え */
    template < typename... Types >
    using BlackboardPtr = std::shared_ptr<CBlackboard<Types...>>;
}

#endif //BLACKBOARD_H
