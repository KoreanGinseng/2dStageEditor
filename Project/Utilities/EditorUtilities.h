#pragma once
#include <Mof.h>

#include <string>
#include <optional>

#include <sstream>
#include <iomanip>

enum class EditMode {
    Write,
    Delete,
    Select,
};

struct VersionResourceData {
    std::string CompanyName;
    std::string FileDescription;
    std::string FileVersion;
    std::string InternalName;
    std::string LegalCopyright;
    std::string OriginalFileName;
    std::string ProductName;
    std::string ProductVersion;
};

class EditorUtilities {
private:
    
    EditorUtilities(void);

    long _window_pos[2];
    bool _not_editarea_hold;
    bool _not_mapchiparea_hold;
    bool _is_modal_open;

    static EditorUtilities& GetInstance(void);

public:
    
    ~EditorUtilities(void);

    static std::optional<VersionResourceData> GetVersionResourceData(void);
    static std::optional<RECT> ChangeWindowSize(void);

    static void HitAreaRefresh(void);
    static bool IsNoEditAreaHold(void);
    static bool IsNoMapChipAreaHold(void);
    static bool IsPopupModalOpen(void);
    static void OpenPopupModal(void);
    static void ClosePopupModal(void);

    static std::string GetFilePath(const std::string& dir, const std::string& file);
    static std::string GetExt(const std::string& path);
    static std::string GetFileName(const std::string& path);

    static std::string SerchDuplicateName(const std::string& name, const std::vector<std::string>& names);

    static std::string GetResourcePath(void);
    static void SetCurrentPathResource(void);

    static bool IsWriteMode(void);
    static bool IsDeleteMode(void);
    static bool IsSelectMode(void);
    static void SetWriteMode(void);
    static void SetDeleteMode(void);
    static void SetSelectMode(void);

    static void ResetSelectPair(void);

    static std::vector<std::string> GetChipLayerNames(void);

    static CRectangle GetEditArea(void);
    static CRectangle GetChipArea(void);

    static CRectangle CalcSelectRect(int begin, int end, const Vector2& chip_size_def, const Vector2& tex_size_def, float scale);
    static CRectangle CalcSelectTextureRect(int no);
    static bool ColorPicker(MofU32& color);

    static std::vector<int> GetSelectChips(int begin, int end, int xcnt);

    static int GetImGuiDefWindowFlag(void);
    static int GetImGuiAlwaysScrollWindowFlag(void);

    static void RenderGrid(const Vector2& offset_pos, const Vector2& max_size, const Vector2& chip_size, const Vector2& scroll);

    static bool OpenTextureFileDialog(const std::string& title, char* path, bool* array_flag = nullptr);

    static std::string getDatetimeStr(void);
};

