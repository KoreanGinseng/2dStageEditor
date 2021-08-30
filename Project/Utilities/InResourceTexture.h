#pragma once
#include <Mof.h>

// ********************************************************************************
/// <summary>
/// ���ߍ��݃��\�[�X����ǂݍ��݂𖼑O��ID�w��łł���摜�N���X
/// </summary>
// ********************************************************************************
class CInResourceTexture : public CTexture {
public:
	// ********************************************************************************
	/// <summary>
	/// �������\�[�X����ǂݍ���
	/// </summary>
	/// <param name="id">.rc�ɓo�^���Ă���PNG ID</param>
	/// <param name="lpName">�o�^���Ă��閼�O</param>
	/// <returns>TRUE : ����, FALSE : ���s</returns>
	/// <created>inoue,2021/08/30</created>
	/// <changed>inoue,2021/08/30</changed>
	// ********************************************************************************
	MofBool Load(DWORD id, LPCMofChar lpName) {
		auto hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(id), "PNG");
		if (hRsrc == NULL) {
			MOF_PRINTLOG("%s �̌����Ɏ��s\n", lpName);
			return FALSE;
		}
		auto resource = ::LoadResource(NULL, hRsrc);
		if (resource == NULL) {
			MOF_PRINTLOG("%s �̓ǂݍ��݂Ɏ��s\n", lpName);
			return FALSE;
		}
		auto data = ::LockResource(resource);
		if (data == NULL) {
			MOF_PRINTLOG("%s �̃��b�N�Ɏ��s\n", lpName);
			return FALSE;
		}
		auto size = ::SizeofResource(NULL, hRsrc);
		return CTexture::Load(lpName, data, size);
	}
};