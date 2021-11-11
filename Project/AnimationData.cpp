#include "AnimationData.h"
#include "FileBinaryLoader.h"
#include "ParserSac.h"
#include "EditorUtilities.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ********************************************************************************
CAnimationData::CAnimationData()
{
}

// ********************************************************************************
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ********************************************************************************
CAnimationData::~CAnimationData()
{
}

// ********************************************************************************
/// <summary>
/// �A�j���[�V�����f�[�^�̓ǂݍ���
/// </summary>
/// <param name="fileName">�t�@�C���p�X</param>
/// <returns>TRUE�@�����A����ȊO�@���s</returns>
// ********************************************************************************
MofBool CAnimationData::Load(LPCMofChar fileName)
{
    auto ext = CEditorUtilities::GetInstance().GetExt(fileName);
    FileLoaderPtr loader = nullptr;
    ParserPtr     parser = nullptr;
    if (ext == ".sac")
    {
        loader = std::make_shared<CFileBinaryLoader>();
        parser = std::make_shared<CParserSac>();
    }
    if (!loader->Load(fileName))
    {
        loader.reset();
        parser.reset();
        return FALSE;
    }
    if (!parser->Parse(loader))
    {
        loader.reset();
        parser.reset();
        return FALSE;
    }
    loader.reset();
    parser.reset();
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// �摜�̓ǂݍ���
/// </summary>
/// <param name="fileName">�t�@�C���p�X</param>
/// <returns>TRUE�@�����A����ȊO�@���s</returns>
// ********************************************************************************
MofBool CAnimationData::LoadTexture(LPCMofChar fileName)
{
    if (m_Texture)
    {
        m_Texture->Release();
        m_Texture.reset();
    }
    m_Texture = std::make_shared<CTexture>();
    if (!m_Texture->Load(fileName))
    {
        return FALSE;
    }
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// �A�j���[�V�����̐���
/// </summary>
/// <param name="anim">�A�j���[�V�����z��</param>
/// <param name="count">�A�j���[�V������</param>
/// <returns>TRUE�@�����A����ȊO�@���s</returns>
// ********************************************************************************
MofBool CAnimationData::Create(SpriteAnimationCreate* anim, MofS32 count)
{
    m_Animation.Release();
    return m_Animation.Create(anim, count);
}

// ********************************************************************************
/// <summary>
/// �A�j���[�V�����̍X�V
/// </summary>
/// <returns>TRUE�@�����A����ȊO�@���s</returns>
// ********************************************************************************
MofBool CAnimationData::Update()
{
    m_Animation.AddTimer(CUtilities::GetFrameSecond());
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// �A�j���[�V�����̕`��
/// </summary>
/// <param name="x">���WX</param>
/// <param name="y">���WY</param>
/// <returns>TRUE�@�����A����ȊO�@���s</returns>
// ********************************************************************************
MofBool CAnimationData::Render(MofFloat x, MofFloat y)
{
    CRectangle src = m_Animation.GetSrcRect();
    if (m_Texture)
    {
        m_Texture->Render(x, y, src);
    }
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// ���
/// </summary>
/// <returns>TRUE�@�����A����ȊO�@���s</returns>
// ********************************************************************************
MofBool CAnimationData::Release()
{
    if (m_Texture)
    {
        m_Texture->Release();
        m_Texture.reset();
    }
    m_Animation.Release();
    return TRUE;
}
