#include "AnimationData.h"
#include "FileBinaryLoader.h"
#include "ParserSac.h"
#include "EditorUtilities.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// コンストラクタ
/// </summary>
// ********************************************************************************
CAnimationData::CAnimationData()
{
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
// ********************************************************************************
CAnimationData::~CAnimationData()
{
}

// ********************************************************************************
/// <summary>
/// アニメーションデータの読み込み
/// </summary>
/// <param name="fileName">ファイルパス</param>
/// <returns>TRUE　成功、それ以外　失敗</returns>
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
/// 画像の読み込み
/// </summary>
/// <param name="fileName">ファイルパス</param>
/// <returns>TRUE　成功、それ以外　失敗</returns>
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
/// アニメーションの生成
/// </summary>
/// <param name="anim">アニメーション配列</param>
/// <param name="count">アニメーション数</param>
/// <returns>TRUE　成功、それ以外　失敗</returns>
// ********************************************************************************
MofBool CAnimationData::Create(SpriteAnimationCreate* anim, MofS32 count)
{
    m_Animation.Release();
    return m_Animation.Create(anim, count);
}

// ********************************************************************************
/// <summary>
/// アニメーションの更新
/// </summary>
/// <returns>TRUE　成功、それ以外　失敗</returns>
// ********************************************************************************
MofBool CAnimationData::Update()
{
    m_Animation.AddTimer(CUtilities::GetFrameSecond());
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// アニメーションの描画
/// </summary>
/// <param name="x">座標X</param>
/// <param name="y">座標Y</param>
/// <returns>TRUE　成功、それ以外　失敗</returns>
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
/// 解放
/// </summary>
/// <returns>TRUE　成功、それ以外　失敗</returns>
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
