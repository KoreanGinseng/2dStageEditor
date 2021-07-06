#include <MofImGui/MofImGui.h>
#include <filesystem>
#include <functional>
#include <Windows.h>
#include <atlstr.h>
#pragma comment(lib, "version.lib")

#include "FileDialog.h"
#include "EditorUtilities.h"
#include "../Window/WindowDefine.h"
#include "../Stage/Stage.h"
#include "../EditorParam/EditorParameter.h"
#include "../Manager/ImGuiWindowManager.h"

EditorUtilities::EditorUtilities(void) 
    : _window_pos()
    , _is_modal_open(false)
    , _not_editarea_hold(false)
    , _not_mapchiparea_hold(false) {
}

EditorUtilities& EditorUtilities::GetInstance(void) {
    static EditorUtilities instance;
    return instance;
}

EditorUtilities::~EditorUtilities(void) {
}

std::optional<VersionResourceData> EditorUtilities::GetVersionResourceData(void) {
    static std::optional<VersionResourceData> instance = std::nullopt;
    if (instance) {
        return instance;
    }
    instance = VersionResourceData();
    TCHAR             fileName[MAX_PATH + 1];
    DWORD             size;
    BYTE*             pVersion;
    UINT              queryLen;
    ATL::CString      fileVersion;
    ATL::CString      subBlock;
    WORD*             pLangCode;
    TCHAR*            pStrInfo;
    ::GetModuleFileName(NULL, fileName, sizeof(fileName));
    size     = ::GetFileVersionInfoSize(fileName, NULL);
    pVersion = new BYTE[size];
    if (::GetFileVersionInfo(fileName, NULL, size, pVersion)) {
        ::VerQueryValue(pVersion, _T("\\VarFileInfo\\Translation"), (void**)&pLangCode, &queryLen);
        std::function<std::string(LPCSTR)> GetFormatData = [&](LPCSTR str) {
            subBlock.Format(str, pLangCode[0], pLangCode[1]);
            ::VerQueryValue(pVersion, (LPTSTR)(LPCTSTR)subBlock, (void**)&pStrInfo, &queryLen);
            if (0 < queryLen) return std::string(pStrInfo);
            return std::string("");
        };
        instance->CompanyName      = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\CompanyName"     ));
        instance->FileDescription  = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\FileDescription" ));
        instance->FileVersion      = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\FileVersion"     ));
        instance->InternalName     = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\InternalName"    ));
        instance->LegalCopyright   = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\LegalCopyright"  ));
        instance->OriginalFileName = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\OriginalFileName"));
        instance->ProductName      = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\ProductName"     ));
        instance->ProductVersion   = GetFormatData(_T("\\StringFileInfo\\%04X%04X\\ProductVersion"  ));
    }
    else {
        instance = std::nullopt;
    }
    delete[] pVersion;
    return instance;
}

std::optional<RECT> EditorUtilities::ChangeWindowSize(void) {
    RECT rect, pos;
    HWND hWnd = g_pMainWindow->GetWindowHandle();
    GetWindowRect(hWnd, &pos);
    GetClientRect(hWnd, &rect);
    if (rect.right <= 0) {
        rect.right = 1;
    }
    if (rect.bottom <= 0) {
        rect.bottom = 1;
    }
    int prev_window_width  = g_pGraphics->GetTargetWidth();
    int prev_window_height = g_pGraphics->GetTargetHeight();
    int now_width          = rect.right;
    int now_height         = rect.bottom;
    if (pos.left != EditorUtilities::GetInstance()._window_pos[0] ||
        pos.top  != EditorUtilities::GetInstance()._window_pos[1]) {
        EditorUtilities::GetInstance()._window_pos[0] = pos.left;
        EditorUtilities::GetInstance()._window_pos[1] = pos.top;
    }
    if (prev_window_width  != now_width ||
        prev_window_height != now_height) {
        g_pGraphics->SetScreenSize(now_width, now_height);
        g_pGraphics->SetBackBufferSize(now_width, now_height);
        return rect;
    }
    return std::nullopt;
}

void EditorUtilities::HitAreaRefresh(void) {
    Vector2 mp;
    g_pInput->GetMousePos(mp);
    if (g_pInput->IsMouseKeyPush(MOFMOUSE_LBUTTON)) {
        bool col_edit = EditorUtilities::GetEditArea().CollisionPoint(mp);
        if (!col_edit && !EditorUtilities::GetInstance()._not_editarea_hold) {
            EditorUtilities::GetInstance()._not_editarea_hold = true;
        }
        bool col_mapchip = EditorUtilities::GetChipArea().CollisionPoint(mp);
        if (!col_mapchip && !EditorUtilities::GetInstance()._not_mapchiparea_hold) {
            EditorUtilities::GetInstance()._not_mapchiparea_hold = true;
        }
    }
    if (g_pInput->IsMouseKeyPull(MOFMOUSE_LBUTTON)) {
        if (EditorUtilities::GetInstance()._not_editarea_hold) {
            EditorUtilities::GetInstance()._not_editarea_hold = false;
        }
        if (EditorUtilities::GetInstance()._not_mapchiparea_hold) {
            EditorUtilities::GetInstance()._not_mapchiparea_hold = false;
        }
    }
}

bool EditorUtilities::IsNoEditAreaHold(void) {
    return EditorUtilities::GetInstance()._not_editarea_hold;
}

bool EditorUtilities::IsNoMapChipAreaHold(void) {
    return EditorUtilities::GetInstance()._not_mapchiparea_hold;
}

bool EditorUtilities::IsPopupModalOpen(void) {
    return EditorUtilities::GetInstance()._is_modal_open;
}

void EditorUtilities::OpenPopupModal(void) {
    EditorUtilities::GetInstance()._is_modal_open = true;
}

void EditorUtilities::ClosePopupModal(void) {
    EditorUtilities::GetInstance()._is_modal_open = false;
}

std::string EditorUtilities::SerchDuplicateName(const std::string& name, const std::vector<std::string>& names) {
    std::function<bool(std::string)> checkhit =
        [&](std::string s) {
        for (auto& it : names) {
            if (it != s) {
                continue;
            }
            return true;
        }
        return false;
    };
    if (!checkhit(name)) {
        return name;
    }
    // 名前がかぶった場合の番号
    int count = 1;
    std::string str;
    do {
        str = name + std::to_string(count++);
    } while (checkhit(str));
    return str;
}

std::string EditorUtilities::GetFilePath(const std::string& dir, const std::string& file) {
    std::function<std::string(std::filesystem::path, const std::string&)> f =
        [&](std::filesystem::path s, const std::string& n) {
        for (auto& it : std::filesystem::recursive_directory_iterator(s)) {
            if (std::filesystem::is_directory(it)) {
                f(it, n);
            }
            else {
                std::string filename = GetFileName(it.path().u8string()) + GetExt(it.path().u8string());
                if (file == filename) {
                    return it.path().u8string();
                }
            }
        }
        return std::string("");
    };

    return f(dir, file);
}

std::string EditorUtilities::GetExt(const std::string& path) {
    std::string ext = path;
    int         length = ext.find_last_of('.');
    ext = ext.substr(length);
    return ext;
}

std::string EditorUtilities::GetFileName(const std::string& path) {
    std::string file = path;
    int         length = file.find_last_of('.');
    int         start = file.find_last_of('/') + 1;
    if (start <= 0) {
        start = file.find_last_of('\\') + 1;
    }
    file = file.substr(start, length - start);
    return file;
}

std::string EditorUtilities::GetResourcePath(void) {
    return *theParam.GetDataPointer<std::string>(ParamKey::ResourcePath);
}

void EditorUtilities::SetCurrentPathResource(void) {
    CUtilities::SetCurrentDirectory(GetResourcePath().c_str());
}

bool EditorUtilities::IsWriteMode(void) {
    return *theParam.GetDataPointer<bool>(ParamKey::WriteMode);
}

void EditorUtilities::SetWriteMode(bool b) {
    *theParam.GetDataPointer<bool>(ParamKey::WriteMode) = b;
}

void EditorUtilities::ResetSelectPair(void) {
    auto select_chips = theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
    *select_chips     = std::pair<int, int>(0, 0);
}

std::vector<std::string> EditorUtilities::GetChipLayerNames(void) {
    std::vector<std::string> names;
    for (const auto& it : *theParam.GetDataPointer<std::vector<MapChip>>(ParamKey::MapChipArray)) {
        names.push_back(it.GetName());
    }
    return names;
}

CRectangle EditorUtilities::GetEditArea(void) {
    auto      mapchip_array =  theParam.GetDataPointer<std::vector<MapChip>>(ParamKey::MapChipArray);
    int       select_layer  = *theParam.GetDataPointer<int>(ParamKey::MapChipLayerSelect);
    float     scale         = *theParam.GetDataPointer<float>(ParamKey::EditScale);
    if (mapchip_array->size() <= 0) {
        return CRectangle(-1, -1, -1, -1);
    }
    auto mapchip = &(*mapchip_array)[select_layer];
    Vector2    chip_size = Vector2(mapchip->GetChipSize().x * scale, mapchip->GetChipSize().y * scale);
    Vector2    max_size  = chip_size * mapchip->GetArraySize();
    Vector2    scroll    = *theParam.GetDataPointer<Vector2>(ParamKey::EditScroll);
    CRectangle edit_rect = *theImGuiWindowManager.Find(ParamKey::EditWindowChild);
    return CRectangle(
        edit_rect.Left, edit_rect.Top,
        edit_rect.Left + min(max_size.x - scroll.x, edit_rect.GetWidth()  - k_scrollbar_size),
        edit_rect.Top  + min(max_size.y - scroll.y, edit_rect.GetHeight() - k_scrollbar_size)
    );
}

CRectangle EditorUtilities::GetChipArea(void) {
    auto      mapchip_array         =  theParam.GetDataPointer<std::vector<MapChip>>(ParamKey::MapChipArray);
    auto      mapchip_texture_array =  theParam.GetDataPointer<std::vector<CTexture>>(ParamKey::MapChipTextureArray);
    auto      texture_arrays        =  theParam.GetDataPointer<std::vector<TextureArray>>(ParamKey::TextureArrays);
    int       select_layer          = *theParam.GetDataPointer<int>(ParamKey::MapChipLayerSelect);
    float     scale                 = *theParam.GetDataPointer<float>(ParamKey::ChipScale);
    if (mapchip_array->size() <= 0) {
        return CRectangle(-1, -1, -1, -1);
    }
    auto mapchip = &(*mapchip_array)[select_layer];
    int  tex_no  = mapchip->GetTextureNo();
    if (tex_no < 0) {
        return CRectangle(-1, -1, -1, -1);
    }
    Vector2 tex_size = Vector2(0, 0);
    if (mapchip->IsTextureArray()) {
        for (auto& texture : (*texture_arrays)[tex_no]) {
            tex_size.x += (texture.GetWidth() * scale);
            tex_size.y  = (max(texture.GetHeight(), tex_size.y) * scale);
        }
    }
    else {
        CTexture* texture  = &(*mapchip_texture_array)[tex_no];
        tex_size = Vector2((texture->GetWidth() * scale), (texture->GetHeight() * scale));
    }
    Vector2    scroll     = *theParam.GetDataPointer<Vector2>(ParamKey::ChipScroll);
    CRectangle rect_child = *theImGuiWindowManager.Find(ParamKey::ChipWindowChild);
    return CRectangle(
        rect_child.Left + k_mapchip_offset, rect_child.Top + k_mapchip_offset,
        rect_child.Left + min(tex_size.x - scroll.x, rect_child.GetWidth()  - k_scrollbar_size) + k_mapchip_offset,
        rect_child.Top  + min(tex_size.y - scroll.y, rect_child.GetHeight() - k_scrollbar_size) + k_mapchip_offset
    );
}

CRectangle EditorUtilities::CalcSelectRect(int begin, int end, const Vector2& chip_size_def, const Vector2& tex_size_def, float scale) {
    const auto  chip_size   = chip_size_def * scale;
    const int   tmp_x_count = (int)(tex_size_def.x / chip_size_def.x);
    const float tmp_x       = (begin % tmp_x_count) * chip_size.x;
    const float tmp_y       = (begin / tmp_x_count) * chip_size.y;
    const float tmp_x_end   = (end   % tmp_x_count) * chip_size.x;
    const float tmp_y_end   = (end   / tmp_x_count) * chip_size.y;
    return CRectangle(tmp_x, tmp_y, tmp_x_end + chip_size.x, tmp_y_end + chip_size.y);
}

CRectangle EditorUtilities::CalcSelectTextureRect(int no) {
    auto  select         = theParam.GetDataPointer<std::pair<int, int>>(ParamKey::MapChipSelect);
    auto  texture_arrays = theParam.GetDataPointer<std::vector<TextureArray>>(ParamKey::TextureArrays);
    auto  texture_array  = &(*texture_arrays)[no];
    float scale          = *theParam.GetDataPointer<float>(ParamKey::ChipScale);
    float offset_x = 0;
    for (int i = 0; i < texture_array->size(); i++) {
        auto texture = &(*texture_array)[i];
        Vector2 tex_size = Vector2((texture->GetWidth() * scale), (texture->GetHeight() * scale));
        if (i == select->first) {
            return CRectangle(offset_x, 0, offset_x + tex_size.x, tex_size.y);
        }
        offset_x += tex_size.x;
    }
    return CRectangle();
}

bool EditorUtilities::ColorPicker(MofU32& color) {
    Vector4 col;
    CVector4Utilities::SetU32Color(color, col);
    if (ImGui::ColorPicker4("", col.fv)) {
        color = CVector4Utilities::ToU32Color(col);
        return true;
    }
    return false;
}

std::vector<int> EditorUtilities::GetSelectChips(int begin, int end, int xcnt)  {
    std::vector<int> v;
    if (begin == end) {
        v.push_back(begin);
        return v;
    }
    int offset = begin % xcnt;
    int diff   = end   % xcnt;
    for (int i = begin; i <= end; i += xcnt) {
        for (int n = i; n <= i + (diff - offset); n++) {
            v.push_back(n);
        }
    }
    return v;
}

int EditorUtilities::GetImGuiDefWindowFlag(void) {
    const int flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove          |
        ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoSavedSettings ;
    return flags;
}

int EditorUtilities::GetImGuiAlwaysScrollWindowFlag(void) {
    int flags = GetImGuiDefWindowFlag();
    flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
    flags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
    return flags;
}

void EditorUtilities::RenderGrid(const Vector2& offset_pos, const Vector2& max_size, const Vector2& chip_size, const Vector2& scroll) {
    if (chip_size.x <= 0 || chip_size.y <= 0) {
        return;
    }
    float end_y = max_size.y + offset_pos.y - scroll.y;
    float end_x = max_size.x + offset_pos.x - scroll.x;
    for (float y = offset_pos.y - scroll.y; y <= end_y; y += chip_size.y) {
        CGraphicsUtilities::RenderLine(offset_pos.x, y, end_x, y, MOF_COLOR_WHITE);
    }
    for (float x = offset_pos.x - scroll.x; x <= end_x; x += chip_size.x) {
        CGraphicsUtilities::RenderLine(x, offset_pos.y, x, end_y, MOF_COLOR_WHITE);
    }
}

bool EditorUtilities::OpenTextureFileDialog(const std::string& title, char* path, bool* array_flag) {
    bool b = false;
    return FileDialog::Open(g_pMainWindow->GetWindowHandle(), FileDialog::Mode::Open,
        title.c_str(),
        "画像 ファイル\0*.png;*.bmp;*.dds\0all file(*.*)\0*.*\0\0",
        "png\0bmp\0dds", path, (array_flag == nullptr ? b : *array_flag));
}

std::string EditorUtilities::getDatetimeStr(void) {
    time_t t = time(nullptr);
    const tm* localTime = localtime(&t);
    std::stringstream s;
    s << "20" << localTime->tm_year - 100;
    // setw(),setfill()で0詰め
    s << std::setw(2) << std::setfill('0') << localTime->tm_mon + 1;
    s << std::setw(2) << std::setfill('0') << localTime->tm_mday;
    s << std::setw(2) << std::setfill('0') << localTime->tm_hour;
    s << std::setw(2) << std::setfill('0') << localTime->tm_min;
    s << std::setw(2) << std::setfill('0') << localTime->tm_sec;
    // std::stringにして値を返す
    return s.str();
}