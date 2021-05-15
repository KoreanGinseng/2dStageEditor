#pragma once
#include "Mof.h"

/// https://easings.net/ja

namespace sip
{
    // ********************************************************************************
    /// <summary>
    /// イーズの種類
    /// </summary>
    // ********************************************************************************
    enum class EaseType
    {
        Sine,
        Cubic,
        Quint,
        Circ,
        Elastic,
        Quad,
        Quart,
        Expo,
        Back,
        Bounce,

        Linear,
    };

    // ********************************************************************************
    /// <summary>
    /// イーズの効果
    /// </summary>
    // ********************************************************************************
    enum class Ease
    {
        In,
        Out,
        InOut,
    };

    namespace Easing
    {
        float Easing(Ease ease, EaseType type, float t);

        float EaseIn(EaseType type, float t);

        float EaseOut(EaseType type, float t);

        float EaseInOut(EaseType type, float t);

        namespace Sine
        {
            float EaseIn(float t);

            float EaseOut(float t);

            float EaseInOut(float t);
        }

        namespace Cubic
        {
            float EaseIn(float t);

            float EaseOut(float t);

            float EaseInOut(float t);
        }

        namespace Quint
        {
            float EaseIn(float t);

            float EaseOut(float t);

            float EaseInOut(float t);
        }

        namespace Circ
        {
            float EaseIn(float t);

            float EaseOut(float t);

            float EaseInOut(float t);
        }

        namespace Elastic
        {
            float EaseIn(float t);

            float EaseOut(float t);

            float EaseInOut(float t);
        }

        namespace Quad
        {
            float EaseIn(float t);

            float EaseOut(float t);

            float EaseInOut(float t);
        }

        namespace Quart
        {
            float EaseIn(float t);

            float EaseOut(float t);

            float EaseInOut(float t);
        }

        namespace Expo
        {
            float EaseIn(float t);

            float EaseOut(float t);

            float EaseInOut(float t);
        }

        namespace Back
        {
            float EaseIn(float t);

            float EaseOut(float t);

            float EaseInOut(float t);
        }

        namespace Bounce
        {
            float EaseIn(float t);

            float EaseOut(float t);

            float EaseInOut(float t);
        }
    }
}