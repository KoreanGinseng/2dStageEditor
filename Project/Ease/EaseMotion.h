#pragma once
#include "Easing.h"

namespace sip
{
    // ********************************************************************************
    /// <summary>
    /// イーズモーションクラス
    /// </summary>
    // ********************************************************************************
    template <typename T > class CEaseMotion
    {
    protected:

        float              m_Timer;            //! タイマー(秒)

        T                  m_Value;            //! 値

        T                  m_Begin;            //! 開始値

        T                  m_End;              //! 終了値

        bool               m_bStart;           //! 開始フラグ

        bool               m_bEnd;             //! 終了フラグ

        float              m_MotionTime;       //! モーションする時間(秒)

        Ease               m_Ease;             //! イーズ効果

        EaseType           m_EaseType;         //! イーズ種類

    public:

        // ********************************************************************************
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        CEaseMotion(void) :
            m_Timer(0.0f),
            m_Value(),
            m_Begin(),
            m_End(),
            m_bStart(false),
            m_bEnd(false),
            m_MotionTime(1.0f),
            m_Ease(Ease::In),
            m_EaseType(EaseType::Sine)
        {
        }

        // ********************************************************************************
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="begin">開始値</param>
        /// <param name="end">終了値</param>
        /// <param name="ease">イージング</param>
        /// <param name="type">イージング種類</param>
        /// <param name="time">開始から終了までの秒</param>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        CEaseMotion(const T& begin, const T& end, const Ease& ease, const EaseType& type, const float& time = 1.0f)
        {
            SetBeginEnd(begin, end);
            SetEase(ease, type);
            SetMotionTime(time);
        }

        // ********************************************************************************
        /// <summary>
        /// デストラクタ
        /// </summary>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual ~CEaseMotion(void) = default;

        // ********************************************************************************
        /// <summary>
        /// 初期化
        /// </summary>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void Initialize(void)
        {
            Reset();
            m_bStart = false;
        }

        // ********************************************************************************
        /// <summary>
        /// 初めからスタートする。
        /// </summary>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void Start(void)
        {
            Reset();
            m_bStart = true;
        }

        // ********************************************************************************
        /// <summary>
        /// 一時停止する。
        /// </summary>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void Stop(void)
        {
            m_bStart = false;
        }

        // ********************************************************************************
        /// <summary>
        /// 再開する。
        /// </summary>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void Resume(void)
        {
            m_bStart = true;
        }

        // ********************************************************************************
        /// <summary>
        /// 更新
        /// </summary>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void Update(void)
        {
            // 始まっていないなら更新しない。
            if (!m_bStart)
            {
                return;
            }

            // タイマーの加算
            m_Timer += CUtilities::GetFrameSecond();

            // タイマーから指定した秒数で 0.0f ~ 1.0f に動くように計算する。
            const float timer = m_Timer / m_MotionTime;

            // タイマーから 0.0f ～ 1.0f を取得する。
            const float t = MOF_MIN(timer, 1.0f);

            // タイマーが 1 秒を超えているなら終了フラグを立てる。
            m_bEnd = (t >= 1.0f);

            // タイマーを使ってイージングする。
            const float e = Easing::Easing(m_Ease, m_EaseType, t);

            // イージングの値で線形補間。
            m_Value = MOF_LERP(m_Begin, m_End, e);
        }

        // ********************************************************************************
        /// <summary>
        /// モーションのリセット
        /// </summary>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void Reset(void)
        {
            m_bEnd = false;

            ResetTimer();
        }

        // ********************************************************************************
        /// <summary>
        /// タイマーのリセット
        /// </summary>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void ResetTimer(void)
        {
            m_Timer = 0.0f;
        }

        // ********************************************************************************
        /// <summary>
        /// モーションする時間を指定する。(秒)
        /// </summary>
        /// <param name="second">秒</param>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void SetMotionTime(const float& second)
        {
            m_MotionTime = second;
        }

        // ********************************************************************************
        /// <summary>
        /// 開始値と終了値の設定
        /// </summary>
        /// <param name="begin">開始値</param>
        /// <param name="end">終了値</param>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void SetBeginEnd(const T& begin, const T& end)
        {
            m_Begin = m_Value = begin;
            m_End = end;
        }

        // ********************************************************************************
        /// <summary>
        /// イーズ効果の設定
        /// </summary>
        /// <param name="ease">イーズ効果</param>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void SetEase(const Ease& ease)
        {
            m_Ease = ease;
        }

        // ********************************************************************************
        /// <summary>
        /// イーズの設定
        /// </summary>
        /// <param name="ease">イーズ効果</param>
        /// <param name="easeType">イーズ種類</param>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void SetEase(const Ease& ease, const EaseType& easeType)
        {
            SetEase(ease);
            SetEaseType(easeType);
        }

        // ********************************************************************************
        /// <summary>
        /// イーズ種類の設定
        /// </summary>
        /// <param name="easeType">イーズ種類</param>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        virtual void SetEaseType(const EaseType& easeType)
        {
            m_EaseType = easeType;
        }

        // ********************************************************************************
        /// <summary>
        /// 現在の値の取得
        /// </summary>
        /// <returns>現在の値</returns>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        const T& GetValue(void) const
        {
            return m_Value;
        }

        // ********************************************************************************
        /// <summary>
        /// スタートしてからの経過時間を返す。(秒)
        /// </summary>
        /// <returns>スタートしてからの経過時間(秒)</returns>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        float GetTimer(void) const
        {
            return m_Timer;
        }

        // ********************************************************************************
        /// <summary>
        /// 終了フラグの取得
        /// </summary>
        /// <returns>終了フラグ</returns>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        bool IsEnd(void) const
        {
            return m_bEnd;
        }

        // ********************************************************************************
        /// <summary>
        /// 開始フラグの取得
        /// </summary>
        /// <returns>開始フラグ</returns>
        /// <created>いのうえ,2020/09/10</created>
        /// <changed>いのうえ,2020/09/10</changed>
        // ********************************************************************************
        bool IsStart(void) const
        {
            return m_bStart;
        }
    };
}