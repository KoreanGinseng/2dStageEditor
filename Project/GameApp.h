/*************************************************************************//*!
					
					@file	GameApp.h
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
															@date	2014.05.14
*//**************************************************************************/

#pragma once

//INCLUDE
#include	"Mof.h"
#include    "AnimationData.h"
#include    "EditWindow.h"
#include    "LayerWindow.h"
#include    "ChipWindow.h"
#include    "MenuBar.h"

/*******************************//*!
@brief	��{�Q�[���A�v���B

@author	�_�c�@��
*//********************************/
class CGameApp : public CSingleGameApplication {
private:

	MofS32                        m_FrameCount;
	MofS32                        m_SaveCheckWait;
	MofBool                       m_bItemChenged;
	MofBool                       m_bItemPrevActived;
	MofBool                       m_bCheck;
	MofBool                       m_bSaveCheck;
	//Editor::CAnimationData        m_SaveData;
	Editor::CMenuBar              m_MenuBar;
	std::string                   m_SaveFileName;
	Editor::CEditWindow           m_EditWindow;
	Editor::CLayerWindow          m_LayerWindow;
	Editor::CChipWindow           m_ChipWindow;
public:
	/*************************************************************************//*!
			@brief			�R���X�g���N�^
			@param			None

			@return			None
	*//**************************************************************************/
	CGameApp(){}
	/*************************************************************************//*!
			@brief			�f�X�g���N�^
			@param			None

			@return			None
	*//**************************************************************************/
	virtual ~CGameApp(){	Release();	}
	/*************************************************************************//*!
			@brief			�A�v���P�[�V�����̏�����
			@param			None
				
			@return			TRUE		����<br>
							����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
	*//**************************************************************************/
	virtual MofBool Initialize(void);
	/*************************************************************************//*!
			@brief			�A�v���P�[�V�����̍X�V
			@param			None
				
			@return			TRUE		����<br>
							����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
	*//**************************************************************************/
	virtual MofBool Update(void);
	/*************************************************************************//*!
			@brief			�A�v���P�[�V�����̕`��
			@param			None
				
			@return			TRUE		����<br>
							����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
	*//**************************************************************************/
	virtual MofBool Render(void);
	/*************************************************************************//*!
			@brief			�A�v���P�[�V�����̉��
			@param			None
				
			@return			TRUE		����<br>
							����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
	*//**************************************************************************/
	virtual MofBool Release(void);
};
