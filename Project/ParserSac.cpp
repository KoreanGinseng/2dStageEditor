#include "ParserSac.h"
#include "AnimationData.h"
#include "EditorDefine.h"
#include "EditorUtilities.h"
#include <filesystem>

using namespace Editor;

// ********************************************************************************
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ********************************************************************************
CParserSac::CParserSac()
{
}

// ********************************************************************************
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ********************************************************************************
CParserSac::~CParserSac()
{
}

// ********************************************************************************
/// <summary>
/// �p�[�T�[
/// </summary>
/// <param name="data">�f�[�^</param>
/// <returns>TRUE ����, ����ȊO ���s</returns>
// ********************************************************************************
MofBool CParserSac::Parse(FileLoaderPtr data)
{
    MofS32 dataSize = data->GetDataSize();
    if (dataSize <= 0)
    {
        return FALSE;
    }
    // �ǂݍ��݂̊֐�
    MofS32 offset = 0;
    std::string dataArray;
    for (MofS32 i = 0; i < dataSize; i++)
    {
        dataArray += ((LPCMofChar)(data->GetData()))[i];
    }
    auto readChunk = [&](MOF_ANIMATION_CHUNK& c)
    {
        c = (MOF_ANIMATION_CHUNK)
            (dataArray.substr(offset, sizeof(MOF_ANIMATION_CHUNK))[0]);
        offset += sizeof(MOF_ANIMATION_CHUNK);
    };
    auto readS32 = [&](MofS32& d)
    {
        d = 0x00000000;
        std::string parse = dataArray.substr(offset, sizeof(MofS32));
        for (MofS32 i = 3; i >= 0; i--)
        {
            d |= ((MofU8)(parse[i]) << i * 8);
        }
        offset += sizeof(MofS32);
    };
    auto readString = [&](std::string& s, MofS32 len)
    {
        s = dataArray.substr(offset, len);
        offset += len;
    };

    // �p�[�X�J�n
    std::string header;
    readString(header, k_AnimFileHeaderLen);
    // �t�@�C���w�b�_���擾
    if (strcmp(header.c_str(), k_AnimFileHeader) != 0)
    {
        MOF_PRINTLOG("header not find for animake data.\n");
        return FALSE;
    }

    // �A�j���[�V���������p
    auto& animData = CSingletonBlackboard<CAnimationData>::GetInstance()
        .Get<CAnimationData>("AnimationData");
    auto& texture  = animData.GetTexture();
    SpriteAnimationCreate* anims;

    // �`�����N���
    MOF_ANIMATION_CHUNK chunk     = MOF_ANIMATION_CHUNK::UNKNOWN;
    MofS32              chunkSize = 0;

    // �t�@�C���G���h�܂œǂ�
    while (offset < dataSize)
    {
        // �`�����N��ǂ�
        readChunk(chunk);
        // �`�����N�T�C�Y��ǂ�
        readS32(chunkSize);

        switch (chunk)
        {
        case MOF_ANIMATION_CHUNK::TEXTURE:
        {
            // �t�@�C�����̒���
            MofS32 len;
            readS32(len);
            // �t�@�C����
            std::string filePath;
            readString(filePath, len);
            // �e�N�X�`���t�@�C���̓ǂݍ���
            if (!animData.LoadTexture(filePath.c_str()))
            {
                auto fileName = CEditorUtilities::GetInstance().GetFileName(filePath) +
                                CEditorUtilities::GetInstance().GetExt(filePath);
                if (!animData.LoadTexture(fileName.c_str()))
                {
                    auto& current  = std::filesystem::current_path().u8string();
                    auto& findPath = CEditorUtilities::GetInstance()
                                     .GetFilePath(current, fileName);
                    if (findPath == "" || !animData.LoadTexture(findPath.c_str()))
                    {
                        auto message = filePath + "��������܂���ł����B";
                        MessageBox(NULL, message.c_str(), "Texture Not Find", MB_OK);
                    }
                }
            }
        } break;
        case MOF_ANIMATION_CHUNK::ANIM:
        {
            MofS32 animCount = 0;
            // �A�j���[�V������
            readS32(animCount);
            anims = NEW SpriteAnimationCreate[animCount];
            for (MofS32 i = 0; i < animCount; i++)
            {
                MofS32 parse = 0;
                auto& anim = anims[i];
                std::string name;
                readS32(parse); readString(name, parse); anim.Name += name.c_str();
                readS32(parse); anim.OffsetX = parse * 0.001f;
                readS32(parse); anim.OffsetY = parse * 0.001f;
                readS32(parse); anim.Width   = parse * 0.001f;
                readS32(parse); anim.Height  = parse * 0.001f;
                readS32(parse); anim.bLoop   = parse;
                // �p�^�[����
                MofS32 patternCount = 0;
                readS32(patternCount);
                for (MofS32 j = 0; j < patternCount && j < 30; j++)
                {
                    auto& pattern = anim.Pattern[j];
                    readS32(parse); pattern.Wait = parse;
                    readS32(pattern.No);
                    readS32(pattern.Step);
                }
            }
            animData.Create(anims, animCount);
            delete[] anims;
            anims = nullptr;
        } break;
        case MOF_ANIMATION_CHUNK::UNKNOWN:
        case MOF_ANIMATION_CHUNK::CUSTOM:
        {
            offset += chunkSize;
        } break;
        }
    }
    return TRUE;
}