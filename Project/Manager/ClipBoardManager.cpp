#include "ClipBoardManager.h"

ClipBoardManager::ClipBoardManager(void) {
}

ClipBoardManager::~ClipBoardManager(void) {
}

ClipBoardManager& ClipBoardManager::GetInstance(void) {
    static ClipBoardManager instance;
    return instance;
}

void ClipBoardManager::Register(const CopyObject& obj) {
    _copy_object = obj;
}

const CopyObject* ClipBoardManager::GetCopyObject(void) const {
    return &_copy_object;
}
