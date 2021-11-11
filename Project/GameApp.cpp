/*************************************************************************//*!

					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include    "EditorDefine.h"
#include    <filesystem>

using namespace Editor;

std::filesystem::path def_path;

void ImGui_IsWindow()
{
	ImGui::Begin("bools");
	ImGui::Text("IsAnyItemActive            : %s", ImGui::IsAnyItemActive()            ? "TRUE" : "FALSE");		
	ImGui::Text("IsAnyItemFocused           : %s", ImGui::IsAnyItemFocused()           ? "TRUE" : "FALSE");		
	ImGui::Text("IsAnyItemHovered           : %s", ImGui::IsAnyItemHovered()           ? "TRUE" : "FALSE");		
	ImGui::Text("IsAnyMouseDown             : %s", ImGui::IsAnyMouseDown()             ? "TRUE" : "FALSE");		
	ImGui::Text("IsItemActivated            : %s", ImGui::IsItemActivated()            ? "TRUE" : "FALSE");		
	ImGui::Text("IsItemActive               : %s", ImGui::IsItemActive()               ? "TRUE" : "FALSE");		
	ImGui::Text("IsItemClicked              : %s", ImGui::IsItemClicked()              ? "TRUE" : "FALSE");		
	ImGui::Text("IsItemDeactivated          : %s", ImGui::IsItemDeactivated()          ? "TRUE" : "FALSE");
	ImGui::Text("IsItemDeactivatedAfterEdit : %s", ImGui::IsItemDeactivatedAfterEdit() ? "TRUE" : "FALSE");
	ImGui::Text("IsItemEdited               : %s", ImGui::IsItemEdited()               ? "TRUE" : "FALSE");
	ImGui::Text("IsItemFocused              : %s", ImGui::IsItemFocused()              ? "TRUE" : "FALSE");
	ImGui::Text("IsItemHovered              : %s", ImGui::IsItemHovered()              ? "TRUE" : "FALSE");
	ImGui::Text("IsItemToggledOpen          : %s", ImGui::IsItemToggledOpen()          ? "TRUE" : "FALSE");
	ImGui::Text("IsItemVisible              : %s", ImGui::IsItemVisible()              ? "TRUE" : "FALSE");
	ImGui::Text("IsMousePosValid            : %s", ImGui::IsMousePosValid()            ? "TRUE" : "FALSE");
	ImGui::Text("IsWindowAppearing          : %s", ImGui::IsWindowAppearing()          ? "TRUE" : "FALSE");
	ImGui::Text("IsWindowCollapsed          : %s", ImGui::IsWindowCollapsed()          ? "TRUE" : "FALSE");
	ImGui::Text("IsWindowDocked             : %s", ImGui::IsWindowDocked()             ? "TRUE" : "FALSE");
	ImGui::Text("IsWindowFocused            : %s", ImGui::IsWindowFocused()            ? "TRUE" : "FALSE");
	ImGui::Text("IsWindowHovered            : %s", ImGui::IsWindowHovered()            ? "TRUE" : "FALSE");
	ImGui::End();
}

/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {

	def_path = std::filesystem::current_path();

	m_SaveFileName     = "none";
	m_bItemChenged     = FALSE;
	m_bItemPrevActived = FALSE;
	m_bCheck           = FALSE;
	m_bSaveCheck       = FALSE;
	m_FrameCount       = 0;
	m_SaveCheckWait    = 60;

	CMofImGui::Setup();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	//CSingletonBlackboard<>::GetInstance()
	//	.Get<>()->Add("AnimationData", m_AnimationData);
	//CSingletonBlackboard<>::GetInstance()
	//	.Get<>()->Add("SaveData", m_SaveData);
	CSingletonBlackboard<MofBool>::GetInstance()
		.Get<MofBool>()->Add("ItemChenge", m_bItemChenged);
	CSingletonBlackboard<MofBool>::GetInstance()
		.Get<MofBool>()->Add("Check", m_bCheck);
	CSingletonBlackboard<MofBool>::GetInstance()
		.Get<MofBool>()->Add("SaveCheck", m_bSaveCheck);
	CSingletonBlackboard<std::string>::GetInstance()
		.Get<std::string>()->Add("SaveFileName", m_SaveFileName);
	CSingletonBlackboard<CEditWindow>::GetInstance()
		.Get<CEditWindow>()->Add("EditWindow", m_EditWindow);
	CSingletonBlackboard<CLayerWindow>::GetInstance()
		.Get<CLayerWindow>()->Add("LayerWindow", m_LayerWindow);
	CSingletonBlackboard<CChipWindow>::GetInstance()
		.Get<CChipWindow>()->Add("ChipWindow", m_ChipWindow);

	//m_SaveData = m_MapData;
	
	m_ChipWindow.DummyCreate();
	m_EditWindow.Create(k_SceneW, k_SceneH);
	m_ChipWindow.Initialize();

	CGraphicsUtilities::Get2DSpriteBind()->SetSamplerType(0, SAMPLER_POINT);
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void) {
	//キーの更新
	g_pInput->RefreshKey();
	CMofImGui::Refresh();

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("#DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar(3);
	m_MenuBar.Update();
	ImGui::DockSpace(ImGui::GetID("MyDockSpace"));


	//アイテム変更の監視
	m_bItemChenged = FALSE;
	if (ImGui::IsAnyItemActive())
	{
		m_bItemPrevActived = TRUE;
	}
	else if (m_bItemPrevActived)
	{
		m_bItemChenged = TRUE;
		m_bItemPrevActived = FALSE;
	}

	//ImGui::ShowDemoWindow();
	//ImGui_IsWindow();

	m_LayerWindow.Update();
	m_ChipWindow.Update();

	ImGui::End();

	m_FrameCount++;
	if (m_FrameCount >= m_SaveCheckWait)
	{
		m_FrameCount = 0;
		//m_bSaveCheck = ((m_MapData != m_SaveData) ? TRUE : FALSE);
	}

	m_bCheck = FALSE;
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void) {
	//描画開始
	g_pGraphics->RenderStart();
	//画面のクリア
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);

	m_ChipWindow.Render();
	
	m_EditWindow.Render();
	
	CMofImGui::RenderSetup();
	CMofImGui::RenderGui();

	//描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void) {

	CUtilities::SetCurrentDirectory(def_path.string().c_str());
	//m_MapData.Release();
	//m_SaveData.Release();

	m_ChipWindow.Release();
	m_EditWindow.Release();

	/*CSingletonBlackboard<>::GetInstance()
		.Get<>()->Delete("MapData");
	CSingletonBlackboard<>::GetInstance()
		.Get<>()->Delete("SaveData");*/
	CSingletonBlackboard<MofBool>::GetInstance()
		.Get<MofBool>()->Delete("ItemChenge");
	CSingletonBlackboard<MofBool>::GetInstance()
		.Get<MofBool>()->Delete("Check");
	CSingletonBlackboard<MofBool>::GetInstance()
		.Get<MofBool>()->Delete("SaveCheck");
	CSingletonBlackboard<std::string>::GetInstance()
		.Get<std::string>()->Delete("SaveFileName");
	CSingletonBlackboard<CEditWindow>::GetInstance()
		.Get<CEditWindow>()->Delete("EditWindow");
	CSingletonBlackboard<CLayerWindow>::GetInstance()
		.Get<CLayerWindow>()->Delete("LayerWindow");
	CSingletonBlackboard<CChipWindow>::GetInstance()
		.Get<CChipWindow>()->Delete("ChipWindow");

	CMofImGui::Cleanup();

	return TRUE;
}