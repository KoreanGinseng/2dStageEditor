#pragma once
#include <Mof.h>

struct CopyObject {
	int                 _x_count;
	std::pair<int, int> _selects;
	std::vector<int>    _chips;
};

class ClipBoardManager {
private:

	CopyObject _copy_object;

	ClipBoardManager(void);

public:

	~ClipBoardManager(void);

	static ClipBoardManager& GetInstance(void);

	void Register(const CopyObject& obj);

	const CopyObject* GetCopyObject(void) const;
};

#define theClipboardManager ClipBoardManager::GetInstance()