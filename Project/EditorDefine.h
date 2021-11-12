#pragma once
#include <memory>
#include <Mof.h>
#include "MofImGui.h"
#include "SingletonBlackboard.h"
#include "FileDialog.h"

namespace Editor
{
    using TexturePtr   = std::shared_ptr<CTexture>;
    using TextureArray = std::vector<TexturePtr>;

    using MapData      = std::vector<std::vector<MofU32>>;

    constexpr MofFloat k_SceneW = 1280.0f;
    constexpr MofFloat k_SceneH =  720.0f;

    constexpr MofS8  k_AnimFileHeader[]  = "MOF_ANIM_FILE";
    constexpr MofS32 k_AnimFileHeaderLen = sizeof(k_AnimFileHeader);

    constexpr MofS8  k_EditorFileHeader[]  = "MOF_EDIT_FILE";
    constexpr MofS32 k_EditorFileHeaderLen = sizeof(k_EditorFileHeader);

    enum class MOF_ANIMATION_CHUNK : MofU8
    {
        UNKNOWN = 0x00, // 不明
        TEXTURE = 0x01, // 画像
        ANIM    = 0x02, // アニメーション
        CUSTOM  = 0xFF, // ユーザーカスタム定義
    };
    enum class MOF_EDITOR_CHUNK : MofU8
    {
        UNKNOWN    = 0x00, // 不明
        TEXTURE    = 0x01, // 画像
        ANIM       = 0x02, // アニメーション
        STAGE      = 0x03, // ステージ
        BACKGROUND = 0x04, // 背景
        COLLISION  = 0x05, // 当たり判定
        CUSTOM     = 0xFF, // ユーザーカスタム定義
    };
}