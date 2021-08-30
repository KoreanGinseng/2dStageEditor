#pragma once
#include <Mof.h>

// ********************************************************************************
/// <summary>
/// 埋め込みリソースから読み込みを名前とID指定でできる画像クラス
/// </summary>
// ********************************************************************************
class CInResourceTexture : public CTexture {
public:
	// ********************************************************************************
	/// <summary>
	/// 内部リソースから読み込み
	/// </summary>
	/// <param name="id">.rcに登録しているPNG ID</param>
	/// <param name="lpName">登録している名前</param>
	/// <returns>TRUE : 成功, FALSE : 失敗</returns>
	/// <created>inoue,2021/08/30</created>
	/// <changed>inoue,2021/08/30</changed>
	// ********************************************************************************
	MofBool Load(DWORD id, LPCMofChar lpName) {
		auto hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(id), "PNG");
		if (hRsrc == NULL) {
			MOF_PRINTLOG("%s の検索に失敗\n", lpName);
			return FALSE;
		}
		auto resource = ::LoadResource(NULL, hRsrc);
		if (resource == NULL) {
			MOF_PRINTLOG("%s の読み込みに失敗\n", lpName);
			return FALSE;
		}
		auto data = ::LockResource(resource);
		if (data == NULL) {
			MOF_PRINTLOG("%s のロックに失敗\n", lpName);
			return FALSE;
		}
		auto size = ::SizeofResource(NULL, hRsrc);
		return CTexture::Load(lpName, data, size);
	}
};