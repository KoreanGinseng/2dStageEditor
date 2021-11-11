#include "EditorFunc.h"
#include "AnimationData.h"
#include "EditorUtilities.h"
#include "FileDialog.h"
#include "Exporter.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// ファイルの新規作成
/// </summary>
/// <param name="file">ファイルパス</param>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CEditorFunc::NewFile(LPCMofChar file)
{
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// ファイルの読み込み
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CEditorFunc::OpenFile()
{
    if (CloseFile() != TRUE)
    {
        return FALSE;
    }
    MofS8 file[PATH_MAX];
    bool bArray = false;
    if (!CFileDialog::Open(nullptr, CFileDialog::Mode::Open, "ファイルを開く",
        "マップデータ(*.map2d)\0*.map2d\0全てのファイル(*.*)\0*.*\0\0",
        "map2d", file, bArray))
    {
        return FALSE;
    }
   /* auto& mapData = CSingletonBlackboard<>::GetInstance()
        .Get<>("MapData");
    if (mapData.Load(file))
    {
        auto& saveData = CSingletonBlackboard<>::GetInstance()
            .Get<>("SaveData");
        saveData = mapData;
        auto& saveFile = CSingletonBlackboard<std::string>::GetInstance()
            .Get<std::string>("SaveFileName");
        saveFile = file;
        return TRUE;
    }*/
    return FALSE;
}

// ********************************************************************************
/// <summary>
/// ファイルを閉じる
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CEditorFunc::CloseFile()
{
    const MofBool& bSaveCheck = CSingletonBlackboard<MofBool>::GetInstance()
        .Get<MofBool>("SaveCheck");
    MofBool closeCheck = TRUE;
    if (bSaveCheck)
    {
        closeCheck = (MessageBox(NULL, "変更データがあります。\n保存していないデータは削除されます。よろしいですか？", "確認", MB_OKCANCEL) == MB_OK);
    }
    if (!closeCheck)
    {
        return FALSE;
    }
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// ファイルの保存
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CEditorFunc::SaveFile()
{
    char filePath[PATH_MAX];
    bool bArray = false;
    if (CFileDialog::Open(nullptr, CFileDialog::Mode::Save,
        "ファイルの保存",
        "マップファイル(*.map2d)\0*.map2d\0All(*.*)\0*.*\0\0",
        "map2d", filePath, bArray))
    {
        return SaveFile(filePath);
    }
    return FALSE;
}

// ********************************************************************************
/// <summary>
/// ファイルの保存
/// </summary>
/// <param name="file">ファイルパス</param>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CEditorFunc::SaveFile(LPCMofChar file)
{
    if (strcmp(file, "none") == 0)
    {
        return SaveFile();
    }
    auto ext = CEditorUtilities::GetInstance().GetExt(file);
    ExporterPtr exporter = nullptr;
    if (ext == ".map2d")
    {
        //exporter = std::make_shared<>();
    }
    if (exporter)
    {
        if (exporter->Export(file))
        {
            auto& saveFile = CSingletonBlackboard<std::string>::GetInstance()
                .Get<std::string>("SaveFileName");
            saveFile = file;
           /* auto& mapData = CSingletonBlackboard<>::GetInstance()
                .Get<>("MapData");
            auto& saveData = CSingletonBlackboard<>::GetInstance()
                .Get<>("SaveData");
            saveData = mapData;*/
            return TRUE;
        }
    }
    return FALSE;
}

// ********************************************************************************
/// <summary>
/// 元に戻す
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CEditorFunc::Undo()
{
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// やり直し
/// </summary>
/// <returns>TRUE 成功, それ以外 失敗</returns>
// ********************************************************************************
MofBool CEditorFunc::Redo()
{
    return TRUE;
}
