#include "ChipWindow.h"
#include "LayerWindow.h"
#include "MofGridRender.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// コンストラクタ
/// </summary>
// ********************************************************************************
CChipWindow::CChipWindow()
    : m_TextureArray()
    , m_TargetArray()
    , m_DummyTarget()
    , m_Scroll()
    , m_SelectNo(-1)
{
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
// ********************************************************************************
CChipWindow::~CChipWindow()
{
}

// ********************************************************************************
/// <summary>
/// 初期化
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CChipWindow::Initialize()
{
    CSingletonBlackboard<MofS32>::GetInstance()
        .Get<MofS32>()->Add("ChipSelectNo", m_SelectNo);
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// ダミーターゲットの作成
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool Editor::CChipWindow::DummyCreate()
{
    if (m_DummyTarget)
    {
        m_DummyTarget->Release();
        m_DummyTarget.reset();
    }
    m_DummyTarget = std::make_shared<CTexture>();
    m_DummyTarget->CreateTarget(k_SceneW, k_SceneH, PIXELFORMAT_R8G8B8A8_UNORM,
        BUFFERACCESS_GPUREADWRITE);
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// 読み込み
/// </summary>
/// <param name="path">ファイルパス</param>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CChipWindow::Load(LPCMofChar path)
{
    TexturePtr ptr = std::make_shared<CTexture>();
    if (!ptr->Load(path))
    {
        return FALSE;
    }
    m_TextureArray.push_back(ptr);
    m_NameArray.push_back(path);
    TexturePtr target = std::make_shared<CTexture>();
    if (!target->CreateTarget(ptr->GetWidth(), ptr->GetHeight(),
        PIXELFORMAT_R8G8B8A8_UNORM, BUFFERACCESS_GPUREADWRITE))
    {
        return FALSE;
    }
    m_TargetArray.push_back(target);
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// 更新
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CChipWindow::Update()
{
    const auto& layerWindow = CSingletonBlackboard<CLayerWindow>::GetInstance()
                              .Get<CLayerWindow>("LayerWindow");
    const auto& layerData   = layerWindow.GetLayerDataArray();
    const auto& layerSelect = layerWindow.GetSelectLayer();
    const auto& mousePos    = ImGui::GetMousePos();
    const auto& padding     = ImGui::GetStyle().WindowPadding;

    const auto localMousePos = Vector2(
        mousePos.x - m_WindowPos.x - m_Scroll.x - padding.x,
        mousePos.y - m_WindowPos.y - m_Scroll.y - padding.y
    );

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        if (layerData.size() > 0)
        {
            const auto& textureNo = layerData[layerSelect].textureNo;
            Vector2 texSize  = Vector2(k_SceneW, k_SceneH);
            MofS32  chipSize = layerData[layerSelect].chipSize;
            if (textureNo >= 0)
            {
                const auto& texture = m_TextureArray[textureNo];
                texSize.x = texture->GetWidth();
                texSize.y = texture->GetHeight();
            }
            if (chipSize > 0 && 
                CRectangleUtilities::CollisionPoint(
                    CRectangle( 0, 0, texSize.x, texSize.y), localMousePos
                ))
            {
                MofS32 tx  = ((MofS32)(texSize.x) / chipSize);
                m_SelectNo = ((MofS32)(localMousePos.x) / chipSize) % tx +
                             ((MofS32)(localMousePos.y) / chipSize) * tx;
            }
        }
    }

    ImGui::Begin("tesw");
    ImGui::Text("%.3f, %.3f", localMousePos.x, localMousePos.y);
    ImGui::Text("%d", m_SelectNo);
    ImGui::End();
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// 描画
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CChipWindow::Render()
{
    const auto& layerWindow = CSingletonBlackboard<CLayerWindow>::GetInstance()
                              .Get<CLayerWindow>("LayerWindow");
    const auto& layerData   = layerWindow.GetLayerDataArray();
    const auto& layerSelect = layerWindow.GetSelectLayer();

    ImGui::Begin("ChipWindow");

    if (layerData.size() > 0)
    {
        const auto& renderTarget = g_pGraphics->GetRenderTarget();
        const auto& depthTarget  = g_pGraphics->GetDepthTarget();
        const auto& textureNo    = layerData[layerSelect].textureNo;
        if (textureNo >= 0)
        {
            const auto& texture     = m_TextureArray[textureNo];
            const auto& target      = m_TargetArray[textureNo];
            const auto& textureName = m_NameArray[textureNo];
            ImVec2 texSize(texture->GetWidth(), texture->GetHeight());
            ImGui::Text(textureName.c_str());
            g_pGraphics->SetRenderTarget(target->GetRenderTarget(), depthTarget);
            g_pGraphics->ClearTarget();
            texture->Render(0, 0);
            CMofGridRender::RenderGrid(layerData[layerSelect].chipSize, texSize.x, texSize.y);
            // TODO :
            g_pGraphics->SetRenderTarget(renderTarget, depthTarget);
            
            ImGui::BeginChild("ChipTexture", ImVec2(0, 0), true,
                ImGuiWindowFlags_AlwaysHorizontalScrollbar |
                ImGuiWindowFlags_AlwaysVerticalScrollbar);
            m_Scroll    = Vector2(ImGui::GetScrollX(), ImGui::GetScrollY());
            m_WindowPos = Vector2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            ImTextureID textureID = static_cast<ImTextureID>(target->GetTexture());
            ImGui::Image(textureID, texSize);
            ImGui::EndChild();
        }
        else
        {
            ImGui::Text("Dummy");
            ImGui::BeginChild("ChipTexture", ImVec2(0, 0), true,
                ImGuiWindowFlags_AlwaysHorizontalScrollbar |
                ImGuiWindowFlags_AlwaysVerticalScrollbar);
            m_Scroll    = Vector2(ImGui::GetScrollX(), ImGui::GetScrollY());
            m_WindowPos = Vector2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            MofS32 chipSize = layerData[layerSelect].chipSize;
            g_pGraphics->SetRenderTarget(m_DummyTarget->GetRenderTarget(), depthTarget);
            g_pGraphics->ClearTarget();
            CMofGridRender::RenderGrid(chipSize, k_SceneW, k_SceneH);
            g_pGraphics->SetRenderTarget(renderTarget, depthTarget);
            ImVec2 texSize(m_DummyTarget->GetWidth(), m_DummyTarget->GetHeight());
            ImTextureID textureID = static_cast<ImTextureID>(m_DummyTarget->GetTexture());
            ImGui::Image(textureID, texSize);
            ImGui::EndChild();
        }
    }
    
    ImGui::End();
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// 解放
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CChipWindow::Release()
{
    for (auto& it : m_TextureArray)
    {
        if (it)
        {
            it->Release();
            it.reset();
        }
    }
    m_TextureArray.clear();
    for (auto& it : m_TargetArray)
    {
        if (it)
        {
            it->Release();
            it.reset();
        }
    }
    m_TargetArray.clear();
    if (m_DummyTarget)
    {
        m_DummyTarget->Release();
        m_DummyTarget.reset();
    }
    return TRUE;
}
