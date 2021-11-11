/******************************************************************************/
/*! @file       ServiceLocator.h
    @brief      サービスロケータークラス定義ファイル
*******************************************************************************/

#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <memory>
#include "Singleton.h"

namespace Editor
{
    /******************************************************************************/
    /*! @class CServiceLocator
        @brief  サービスロケータークラス
    *******************************************************************************/
    template < typename T >
    class CServiceLocator : public CSingleton<CServiceLocator<T>>
    {
        friend class CSingleton<CServiceLocator<T>>;
        using Service = std::shared_ptr<T>;

    protected:

        /*! サービスの共有ポインタ */
        Service m_Service;

        /******************************************************************************/
        /*! コンストラクタ
        *******************************************************************************/
        CServiceLocator()
            : m_Service()
        {
        }

    public:

        /******************************************************************************/
        /*! サービスの設定
            @param[in]      obj    サービス
        *******************************************************************************/
        static void SetService(const Service& obj) noexcept
        {
            CServiceLocator<T>::GetInstance().m_Service = obj;
        }

        /******************************************************************************/
        /*! サービスの取得
            @return         サービス
        *******************************************************************************/
        static Service& GetService() noexcept
        {
            return CServiceLocator<T>::GetInstance().m_Service;
        }
    };
}

#endif //SERVICELOCATOR_H
