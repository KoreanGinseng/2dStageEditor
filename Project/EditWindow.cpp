#include "EditWindow.h"
#include "LayerWindow.h"
#include "MofGridRender.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// コンストラクタ
/// </summary>
// ********************************************************************************
CEditWindow::CEditWindow()
{
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
// ********************************************************************************
CEditWindow::~CEditWindow()
{
}

// ********************************************************************************
/// <summary>
/// 描画ターゲットの生成
/// </summary>
/// <param name="w">横幅</param>
/// <param name="h">高さ</param>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CEditWindow::Create(MofU32 w, MofU32 h)
{
    Release();
    m_RenderTarget = std::make_shared<CTexture>();
    return m_RenderTarget->CreateTarget(w, h, PIXELFORMAT_R8G8B8A8_UNORM, BUFFERACCESS_GPUREADWRITE);
}

// ********************************************************************************
/// <summary>
/// 更新
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CEditWindow::Update()
{
    
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// 描画
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CEditWindow::Render()
{
    const auto& renderTarget   = g_pGraphics->GetRenderTarget();
    const auto& depthTarget    = g_pGraphics->GetDepthTarget();
    const auto& layerWindow    = CSingletonBlackboard<CLayerWindow>::GetInstance()
                                 .Get<CLayerWindow>("LayerWindow");
    const auto& layerDataArray = layerWindow.GetLayerDataArray();
    const auto& layerSelect    = layerWindow.GetSelectLayer();

    ImGui::Begin("EditWindow");
    ImGuiWindowFlags flag = ImGuiWindowFlags_AlwaysHorizontalScrollbar |
                            ImGuiWindowFlags_AlwaysVerticalScrollbar;
    ImGui::BeginChild("EditRenderTexture", ImVec2(0, 0), true, flag);
    m_Scroll.x    = ImGui::GetScrollX();
    m_Scroll.y    = ImGui::GetScrollY();
    m_WindowPos.x = ImGui::GetWindowPos().x;
    m_WindowPos.y = ImGui::GetWindowPos().y;
    if (m_RenderTarget)
    {
        ImVec2 texSize(m_RenderTarget->GetWidth(), m_RenderTarget->GetHeight());
        g_pGraphics->SetRenderTarget(m_RenderTarget->GetRenderTarget(), depthTarget);
        g_pGraphics->ClearTarget();

        if (layerDataArray.size() > 0)
        {
            MofS32 chipSize = layerDataArray[layerSelect].chipSize;
            CMofGridRender::RenderGrid(chipSize, texSize.x, texSize.y);
            if (chipSize > 0)
            {
                const auto& mousePos      = ImGui::GetMousePos();
                const auto& padding       = ImGui::GetStyle().WindowPadding;
                const auto  localMousePos = Vector2(
                    mousePos.x - m_WindowPos.x + m_Scroll.x - padding.x,
                    mousePos.y - m_WindowPos.y + m_Scroll.y - padding.y
                );
                MofS32 tx       = ((MofS32)(texSize.x) / chipSize);
                MofS32 selectNo = ((MofS32)(localMousePos.x) / chipSize) % tx +
                                  ((MofS32)(localMousePos.y) / chipSize) * tx;
                CGraphicsUtilities::RenderRect(
                    CMofGridRender::GetRect(selectNo, chipSize, texSize.x),
                    MOF_COLOR_RED);
            }
        }
        g_pGraphics->SetRenderTarget(renderTarget, depthTarget);

        ImTextureID textureID = static_cast<ImTextureID>(m_RenderTarget->GetTexture());
        ImGui::Image(textureID, texSize);
    }
    ImGui::EndChild();
    ImGui::End();
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// 解放
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CEditWindow::Release()
{
    if (m_RenderTarget)
    {
        m_RenderTarget->Release();
        m_RenderTarget.reset();
    }
    return TRUE;
}
