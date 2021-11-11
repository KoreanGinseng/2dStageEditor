#pragma once
#include "FileLoader.h"

namespace Editor
{
    // ********************************************************************************
    /// <summary>
    /// ファイルをバイナリで読むクラス
    /// </summary>
    // ********************************************************************************
    class CFileBinaryLoader : public IFileLoader
    {
    private:
        
        /// <summary>データ</summary>
        LPMofS8 m_pBinaryArray;

        /// <summary>データサイズ</summary>
        MofS32  m_DataSize;

    public:

        // ********************************************************************************
        /// <summary>
        /// コンストラクタ
        /// </summary>
        // ********************************************************************************
        CFileBinaryLoader()
            : m_pBinaryArray(nullptr)
            , m_DataSize(0)
        {
        }

        // ********************************************************************************
        /// <summary>
        /// デストラクタ
        /// </summary>
        // ********************************************************************************
        virtual ~CFileBinaryLoader() override { Release(); }
        
        // ********************************************************************************
        /// <summary>
        /// 読み込み
        /// </summary>
        /// <param name="file">ファイルパス</param>
        /// <returns>TRUE 成功, それ以外 失敗</returns>
        // ********************************************************************************
        virtual MofBool Load(LPCMofChar file) override
        {
            FILE* fp = fopen(file, "rb");
            if (fp == NULL) { return FALSE; }
            fpos_t fSize = 0;
            fseek(fp, 0, SEEK_END);
            fgetpos(fp, &fSize);
            fseek(fp, 0, SEEK_SET);
            m_pBinaryArray = new MofS8[fSize];
            m_DataSize     = fSize;
            fread(m_pBinaryArray, m_DataSize, 1, fp);
            fclose(fp);
            return TRUE;
        }
        
        // ********************************************************************************
        /// <summary>
        /// 解放
        /// </summary>
        /// <returns>TRUE 成功, それ以外 失敗</returns>
        // ********************************************************************************
        MofBool Release()
        {
            if (m_pBinaryArray)
            {
                m_DataSize = 0;
                delete[] m_pBinaryArray;
                m_pBinaryArray = nullptr;
            }
            return TRUE;
        }

        // ********************************************************************************
        /// <summary>
        /// データの取得
        /// </summary>
        /// <returns>データ</returns>
        // ********************************************************************************
        virtual const void* GetData() const noexcept override
        {
            return m_pBinaryArray;
        }

        // ********************************************************************************
        /// <summary>
        /// データサイズの取得
        /// </summary>
        /// <returns>データサイズ</returns>
        // ********************************************************************************
        virtual const MofS32 GetDataSize() const noexcept override
        {
            return m_DataSize;
        }
    };
}

