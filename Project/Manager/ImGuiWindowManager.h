#pragma once
#include "../EditorParam/ParameterKey.h"
#include <MofImGui/MofImGui.h>

struct GuiWindowData {
    ParamKey::Type name          { };
    CRectangle     rect          { Mof::Rectangle() };
    bool*          active_pointer{ nullptr };
};

using WindowRectArray   = std::vector<GuiWindowData>;
using LPWindowRectArray = std::vector<GuiWindowData>*;

class ImGuiWindowManager {
private:

    WindowRectArray _data_array;

    ImGuiWindowManager(void);

public:

    ~ImGuiWindowManager(void);

    void Register(const ParamKey::Type& name);

    CRectangle* Find(const ParamKey::Type& name);

    static ImGuiWindowManager& GetInstance(void);

    bool IsUseGuiItem(void) const;
};

#define theImGuiWindowManager ImGuiWindowManager::GetInstance()