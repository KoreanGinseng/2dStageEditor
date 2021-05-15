#include "Easing.h"

namespace sip
{
    float Easing::Sine::EaseIn(float t)
    {
        return 1.0f - MOF_COS((t * MOF_MATH_HALFPI));
    }

    float Easing::Sine::EaseOut(float t)
    {
        return MOF_SIN(t * MOF_MATH_HALFPI);
    }

    float Easing::Sine::EaseInOut(float t)
    {
        return -(MOF_COS(MOF_MATH_PI * t) - 1.0f) * 0.5f;
    }

    float Easing::Cubic::EaseIn(float t)
    {
        return t * t * t;
    }

    float Easing::Cubic::EaseOut(float t)
    {
        return 1.0f - MOF_POW(1.0f - t, 3);
    }

    float Easing::Cubic::EaseInOut(float t)
    {
        return t < 0.5f ? 4.0f * t * t * t : 1.0f - MOF_POW(-2.0f * t + 2.0f, 3) * 0.5f;
    }

    float Easing::Quint::EaseIn(float t)
    {
        return t * t * t * t * t;
    }

    float Easing::Quint::EaseOut(float t)
    {
        return 1.0f - MOF_POW(1.0f - t, 5.0f);
    }

    float Easing::Quint::EaseInOut(float t)
    {
        return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - MOF_POW(-2.0f * t + 2.0f, 5.0f) * 0.5f;
    }

    float Easing::Circ::EaseIn(float t)
    {
        return 1.0f - MOF_SQRT(1.0f - MOF_POW(t, 2.0f));
    }

    float Easing::Circ::EaseOut(float t)
    {
        return MOF_SQRT(1.0f - MOF_POW(t - 1.0f, 2.0f));
    }

    float Easing::Circ::EaseInOut(float t)
    {
        return t < 0.5f
            ? (1.0f - MOF_SQRT(1.0f - MOF_POW(2.0f * t, 2.0f))) * 0.5f
            : (MOF_SQRT(1.0f - MOF_POW(-2.0f * t + 2.0f, 2.0f)) + 1.0f) * 0.5f;
    }

    float Easing::Elastic::EaseIn(float t)
    {
        const float c4 = (2.0f * MOF_MATH_PI) / 3.0f;
        return t == 0.0f
            ? 0.0f
            : t == 1.0f
            ? 1.0f
            : -MOF_POW(2.0f, 10.0f * t - 10.0f) * MOF_SIN((t * 10.0f - 10.75f) * c4);
    }

    float Easing::Elastic::EaseOut(float t)
    {
        const float c4 = (2.0f * MOF_MATH_PI) / 3.0f;
        return t == 0.0f
            ? 0.0f
            : t == 1.0f
            ? 1.0f
            : MOF_POW(2.0f, -10.0f * t) * MOF_SIN((t * 10.0f - 0.75f) * c4) + 1.0f;
    }

    float Easing::Elastic::EaseInOut(float t)
    {
        const float c5 = (2.0f * MOF_MATH_PI) / 4.5f;

        return t == 0.0f
            ? 0.0f
            : t == 1.0f
            ? 1.0f
            : t < 0.5f
            ? -(MOF_POW(2.0f, 20.0f * t - 10.0f) * MOF_SIN((20.0f * t - 11.125f) * c5)) * 0.5f
            : (MOF_POW(2.0f, -20.0f * t + 10.0f) * MOF_SIN((20.0f * t - 11.125f) * c5)) * 0.5f + 1.0f;
    }

    float Easing::Quad::EaseIn(float t)
    {
        return t * t;
    }

    float Easing::Quad::EaseOut(float t)
    {
        return 1.0f - (1.0f - t) * (1.0f - t);
    }

    float Easing::Quad::EaseInOut(float t)
    {
        return t < 0.5f ? 2.0f * t * t : 1.0f - MOF_POW(-2.0f * t + 2.0f, 2.0f) * 0.5f;
    }

    float Easing::Quart::EaseIn(float t)
    {
        return t * t * t * t;
    }

    float Easing::Quart::EaseOut(float t)
    {
        return 1.0f - MOF_POW(1.0f - t, 4.0f);
    }

    float Easing::Quart::EaseInOut(float t)
    {
        return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - MOF_POW(-2.0f * t + 2.0f, 4.0f) * 0.5f;
    }

    float Easing::Expo::EaseIn(float t)
    {
        return t == 0.0f ? 0.0f : MOF_POW(2.0f, 10.0f * t - 10.0f);
    }

    float Easing::Expo::EaseOut(float t)
    {
        return t == 1.0f ? 1.0f : 1.0f - MOF_POW(2.0f, -10.0f * t);
    }

    float Easing::Expo::EaseInOut(float t)
    {
        return t == 0.0f
            ? 0.0f
            : t == 1.0f
            ? 1.0f
            : t < 0.5f ? MOF_POW(2.0f, 20.0f * t - 10.0f) * 0.5f
            : (2.0f - MOF_POW(2.0f, -20.0f * t + 10.0f)) * 0.5f;
    }

    float Easing::Back::EaseIn(float t)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;

        return c3 * t * t * t - c1 * t * t;
    }

    float Easing::Back::EaseOut(float t)
    {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;

        return 1.0f + c3 * MOF_POW(t - 1.0f, 3.0f) + c1 * MOF_POW(t - 1.0f, 2.0f);
    }

    float Easing::Back::EaseInOut(float t)
    {
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;

        return t < 0.5f
            ? (MOF_POW(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) * 0.5f
            : (MOF_POW(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) * 0.5f;
    }

    float Easing::Bounce::EaseIn(float t)
    {
        return 1.0f - EaseOut(1.0f - t);
    }

    float Easing::Bounce::EaseOut(float t)
    {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;

        if (t < 1.0f / d1)
        {
            return n1 * t * t;
        }
        else if (t < 2.0f / d1)
        {
            return n1 * (t -= 1.5f / d1) * t + 0.75f;
        }
        else if (t < 2.5f / d1)
        {
            return n1 * (t -= 2.25f / d1) * t + 0.9375f;
        }
        else
        {
            return n1 * (t -= 2.625f / d1) * t + 0.984375f;
        }
    }

    float Easing::Bounce::EaseInOut(float t)
    {
        return t < 0.5f
            ? (1.0f - EaseOut(1.0f - 2.0f * t)) * 0.5f
            : (1.0f + EaseOut(2.0f * t - 1.0f)) * 0.5f;
    }

    float Easing::Easing(Ease ease, EaseType type, float t)
    {
        switch (ease)
        {
        case Ease::In:              return Easing::EaseIn(type, t);

        case Ease::Out:             return Easing::EaseOut(type, t);

        case Ease::InOut:           return Easing::EaseInOut(type, t);
        }

        return t;
    }

    float Easing::EaseIn(EaseType type, float t)
    {
        switch (type)
        {
        case EaseType::Sine:        return Easing::Sine::EaseIn(t);

        case EaseType::Cubic:       return Easing::Cubic::EaseIn(t);

        case EaseType::Quint:       return Easing::Quint::EaseIn(t);

        case EaseType::Circ:        return Easing::Circ::EaseIn(t);

        case EaseType::Elastic:     return Easing::Elastic::EaseIn(t);

        case EaseType::Quad:        return Easing::Quad::EaseIn(t);

        case EaseType::Quart:       return Easing::Quart::EaseIn(t);

        case EaseType::Expo:        return Easing::Expo::EaseIn(t);

        case EaseType::Back:        return Easing::Back::EaseIn(t);

        case EaseType::Bounce:      return Easing::Bounce::EaseIn(t);
        }

        return t;
    }

    float Easing::EaseOut(EaseType type, float t)
    {
        switch (type)
        {
        case EaseType::Sine:        return Easing::Sine::EaseOut(t);

        case EaseType::Cubic:       return Easing::Cubic::EaseOut(t);

        case EaseType::Quint:       return Easing::Quint::EaseOut(t);

        case EaseType::Circ:        return Easing::Circ::EaseOut(t);

        case EaseType::Elastic:     return Easing::Elastic::EaseOut(t);

        case EaseType::Quad:        return Easing::Quad::EaseOut(t);

        case EaseType::Quart:       return Easing::Quart::EaseOut(t);

        case EaseType::Expo:        return Easing::Expo::EaseOut(t);

        case EaseType::Back:        return Easing::Back::EaseOut(t);

        case EaseType::Bounce:      return Easing::Bounce::EaseOut(t);
        }

        return t;
    }

    float Easing::EaseInOut(EaseType type, float t)
    {
        switch (type)
        {
        case EaseType::Sine:        return Easing::Sine::EaseInOut(t);

        case EaseType::Cubic:       return Easing::Cubic::EaseInOut(t);

        case EaseType::Quint:       return Easing::Quint::EaseInOut(t);

        case EaseType::Circ:        return Easing::Circ::EaseInOut(t);

        case EaseType::Elastic:     return Easing::Elastic::EaseInOut(t);

        case EaseType::Quad:        return Easing::Quad::EaseInOut(t);

        case EaseType::Quart:       return Easing::Quart::EaseInOut(t);

        case EaseType::Expo:        return Easing::Expo::EaseInOut(t);

        case EaseType::Back:        return Easing::Back::EaseInOut(t);

        case EaseType::Bounce:      return Easing::Bounce::EaseInOut(t);
        }

        return t;
    }
}