#include "EditorFunc.h"
#include "AnimationData.h"
#include "EditorUtilities.h"
#include "FileDialog.h"
#include "Exporter.h"

using namespace Editor;

// ********************************************************************************
/// <summary>
/// �t�@�C���̐V�K�쐬
/// </summary>
/// <param name="file">�t�@�C���p�X</param>
/// <returns>TRUE ����, ����ȊO ���s</returns>
// ********************************************************************************
MofBool CEditorFunc::NewFile(LPCMofChar file)
{
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// �t�@�C���̓ǂݍ���
/// </summary>
/// <returns>TRUE ����, ����ȊO ���s</returns>
// ********************************************************************************
MofBool CEditorFunc::OpenFile()
{
    if (CloseFile() != TRUE)
    {
        return FALSE;
    }
    MofS8 file[PATH_MAX];
    bool bArray = false;
    if (!CFileDialog::Open(nullptr, CFileDialog::Mode::Open, "�t�@�C�����J��",
        "�}�b�v�f�[�^(*.map2d)\0*.map2d\0�S�Ẵt�@�C��(*.*)\0*.*\0\0",
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
/// �t�@�C�������
/// </summary>
/// <returns>TRUE ����, ����ȊO ���s</returns>
// ********************************************************************************
MofBool CEditorFunc::CloseFile()
{
    const MofBool& bSaveCheck = CSingletonBlackboard<MofBool>::GetInstance()
        .Get<MofBool>("SaveCheck");
    MofBool closeCheck = TRUE;
    if (bSaveCheck)
    {
        closeCheck = (MessageBox(NULL, "�ύX�f�[�^������܂��B\n�ۑ����Ă��Ȃ��f�[�^�͍폜����܂��B��낵���ł����H", "�m�F", MB_OKCANCEL) == MB_OK);
    }
    if (!closeCheck)
    {
        return FALSE;
    }
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// �t�@�C���̕ۑ�
/// </summary>
/// <returns>TRUE ����, ����ȊO ���s</returns>
// ********************************************************************************
MofBool CEditorFunc::SaveFile()
{
    char filePath[PATH_MAX];
    bool bArray = false;
    if (CFileDialog::Open(nullptr, CFileDialog::Mode::Save,
        "�t�@�C���̕ۑ�",
        "�}�b�v�t�@�C��(*.map2d)\0*.map2d\0All(*.*)\0*.*\0\0",
        "map2d", filePath, bArray))
    {
        return SaveFile(filePath);
    }
    return FALSE;
}

// ********************************************************************************
/// <summary>
/// �t�@�C���̕ۑ�
/// </summary>
/// <param name="file">�t�@�C���p�X</param>
/// <returns>TRUE ����, ����ȊO ���s</returns>
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
/// ���ɖ߂�
/// </summary>
/// <returns>TRUE ����, ����ȊO ���s</returns>
// ********************************************************************************
MofBool CEditorFunc::Undo()
{
    return TRUE;
}

// ********************************************************************************
/// <summary>
/// ��蒼��
/// </summary>
/// <returns>TRUE ����, ����ȊO ���s</returns>
// ********************************************************************************
MofBool CEditorFunc::Redo()
{
    return TRUE;
}
