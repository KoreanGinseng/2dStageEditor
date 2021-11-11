#include "LayerWindow.h"
#include "ChipWindow.h"
#include "EditorUtilities.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// コンストラクタ
/// </summary>
// ********************************************************************************
CLayerWindow::CLayerWindow()
    : m_SelectLayer(0)
    , m_LayerDataArray()
{
    LayerData data;
    data.name = "newLayer";
    m_LayerDataArray.push_back(data);
    m_SelectLayer = m_LayerDataArray.size() - 1;
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
// ********************************************************************************
CLayerWindow::~CLayerWindow()
{
}

// ********************************************************************************
/// <summary>
/// 更新
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CLayerWindow::Update()
{
    ImGui::Begin("LayerWindow");

    ImVec2 windowSize = ImGui::GetWindowSize();

    ImGui::BeginChild("LayerChild", ImVec2(0, windowSize.y * 0.5f), true);

    for (MofS32 i = 0; i < m_LayerDataArray.size(); i++)
    {
        auto& layerData = m_LayerDataArray[i];
        if (ImGui::Selectable(layerData.name.c_str(), i == m_SelectLayer))
        {
            m_SelectLayer = i;
        }
    }

    ImGui::EndChild();

    if (ImGui::ArrowButton("##LayerUp", ImGuiDir_Up))
    {
    }ImGui::SameLine();
    if (ImGui::ArrowButton("##LayerDown", ImGuiDir_Down))
    {
    }ImGui::SameLine();
    if (ImGui::Button("Add##Layer"))
    {
        static MofS32 layerCount = 1;
        LayerData data;
        data.name = "newLayer" + std::to_string(layerCount++);
        m_LayerDataArray.push_back(data);
        m_SelectLayer = m_LayerDataArray.size() - 1;
    } ImGui::SameLine();
    if (ImGui::Button("Delete##Layer"))
    {
        if (m_LayerDataArray.size() > 0)
        {
            ImGui::OpenPopup("Delete Layer##popup");
        }
    }

    if (ImGui::BeginPopupModal("Delete Layer##popup"))
    {
        ImGui::Text("Delete Layer : %s\nOK?", m_LayerDataArray[m_SelectLayer].name.c_str());

        if (ImGui::Button("OK!!!!"))
        {
            auto it = m_LayerDataArray.begin() + m_SelectLayer;
            m_LayerDataArray.erase(it);
            if (m_SelectLayer >= m_LayerDataArray.size())
            {
                m_SelectLayer = m_LayerDataArray.size() - 1;
            }
            ImGui::CloseCurrentPopup();
        }ImGui::SameLine();
        if (ImGui::Button("Cancelllllll"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::BeginChild("editData", ImVec2(0, 105), true);

    if (m_LayerDataArray.size() > 0)
    {
        auto& layerData = m_LayerDataArray[m_SelectLayer];

        MofS8 name[MAX_PATH];
        strcpy(name, layerData.name.c_str());
        if (ImGui::InputText("LayerName", name, MAX_PATH, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            layerData.name = name;
        }

        MofInt chipSize = layerData.chipSize;
        if (ImGui::InputInt("ChipSize", &chipSize))
        {
            layerData.chipSize = chipSize;
        }

        auto& chipWindow = CSingletonBlackboard<CChipWindow>::GetInstance()
            .Get<CChipWindow>("ChipWindow");
        MofInt textureNo = layerData.textureNo + 1;
        std::string comboItem = "Dummy";
        comboItem.push_back('\0');
        for (const auto& texName : chipWindow.GetTextureNameArray())
        {
            comboItem += texName;
            comboItem.push_back('\0');
        }
        comboItem.push_back('\0');
        comboItem.push_back('\0');
        if (ImGui::Combo("Textures", &textureNo, comboItem.c_str()))
        {
            layerData.textureNo = textureNo - 1;
        }
        if (ImGui::Button("Load##Texture"))
        {
            bool bArray = false;
            MofS8 filePath[PATH_MAX];
            if (CFileDialog::Open(nullptr, CFileDialog::Mode::Open, "画像の読み込み",
                "image file(*.png,*.jpeg,*.bmp,*.dds)\0*.png;*.jpeg;*.bmp;*.dds\0All(*.*)\0*.*\0\0",
                "png\0jpeg\0bmp\0dds", filePath, bArray))
            {
                if (bArray)
                {
                    std::vector<std::string> files;
                    CFileDialog::SeparatePath(filePath, files);
                    for (const auto& file : files)
                    {
                        chipWindow.Load(file.c_str());
                    }
                }
                else
                {
                    chipWindow.Load(filePath);
                }
            }
        } ImGui::SameLine();
        if (ImGui::Button("Release##Texture"))
        {

        }
    }

    ImGui::EndChild();

    ImGui::End();
    return TRUE;
}
