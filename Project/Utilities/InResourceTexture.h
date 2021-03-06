#pragma once
#include <Mof.h>

class CInResourceTexture : public CTexture {
public:
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