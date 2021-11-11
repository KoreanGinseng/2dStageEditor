#include "EditWindow.h"
#include "LayerWindow.h"
#include "MofGridRender.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// �R���X�g���N�^
/// </summary>
// ********************************************************************************
CEditWindow::CEditWindow()
{
}

// ********************************************************************************
/// <summary>
/// �f�X�g���N�^
/// </summary>
// ********************************************************************************
CEditWindow::~CEditWindow()
{
}

// ********************************************************************************
/// <summary>
/// �`��^�[�Q�b�g�̐���
/// </summary>
/// <param name="w">����</param>
/// <param name="h">����</param>
/// <returns>TRUE ����, ����ȊO ���s</returns>
// ********************************************************************************
MofBool CEditWindow::Create(MofU32 w, MofU32 h)
{
    Release();
    m_RenderTarget = std::make_shared<CTexture>();
    return m_RenderTarget->CreateTarget(w, h, PIXELFORMAT_R8G8B8A8_UNORM, BUFFERACCESS_GPUREADWRITE);
}

// ********************************************************************************
/// <summary>
/// �X�V
/// </summary>
/// <returns>TRUE ����, ����ȊO ���s</returns>
// ********************************************************************************
MofBool CEditWindow::Update()
{
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// �`��
/// </summary>
/// <returns>TRUE ����, ����ȊO ���s</returns>
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
    m_Scroll.x = ImGui::GetScrollX();
    m_Scroll.y = ImGui::GetScrollY();
    if (m_RenderTarget)
    {
        ImVec2 texSize(m_RenderTarget->GetWidth(), m_RenderTarget->GetHeight());
        g_pGraphics->SetRenderTarget(m_RenderTarget->GetRenderTarget(), depthTarget);
        g_pGraphics->ClearTarget();

        if (layerDataArray.size() > 0)
        {
            CMofGridRender::RenderGrid(layerDataArray[layerSelect].chipSize, texSize.x, texSize.y);
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
/// ���
/// </summary>
/// <returns>TRUE ����, ����ȊO ���s</returns>
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
