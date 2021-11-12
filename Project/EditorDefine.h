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
        UNKNOWN = 0x00, // �s��
        TEXTURE = 0x01, // �摜
        ANIM    = 0x02, // �A�j���[�V����
        CUSTOM  = 0xFF, // ���[�U�[�J�X�^����`
    };
    enum class MOF_EDITOR_CHUNK : MofU8
    {
        UNKNOWN    = 0x00, // �s��
        TEXTURE    = 0x01, // �摜
        ANIM       = 0x02, // �A�j���[�V����
        STAGE      = 0x03, // �X�e�[�W
        BACKGROUND = 0x04, // �w�i
        COLLISION  = 0x05, // �����蔻��
        CUSTOM     = 0xFF, // ���[�U�[�J�X�^����`
    };
}