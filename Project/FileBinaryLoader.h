#pragma once
#include "FileLoader.h"

namespace Editor
{
    // ********************************************************************************
    /// <summary>
    /// �t�@�C�����o�C�i���œǂރN���X
    /// </summary>
    // ********************************************************************************
    class CFileBinaryLoader : public IFileLoader
    {
    private:
        
        /// <summary>�f�[�^</summary>
        LPMofS8 m_pBinaryArray;

        /// <summary>�f�[�^�T�C�Y</summary>
        MofS32  m_DataSize;

    public:

        // ********************************************************************************
        /// <summary>
        /// �R���X�g���N�^
        /// </summary>
        // ********************************************************************************
        CFileBinaryLoader()
            : m_pBinaryArray(nullptr)
            , m_DataSize(0)
        {
        }

        // ********************************************************************************
        /// <summary>
        /// �f�X�g���N�^
        /// </summary>
        // ********************************************************************************
        virtual ~CFileBinaryLoader() override { Release(); }
        
        // ********************************************************************************
        /// <summary>
        /// �ǂݍ���
        /// </summary>
        /// <param name="file">�t�@�C���p�X</param>
        /// <returns>TRUE ����, ����ȊO ���s</returns>
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
        /// ���
        /// </summary>
        /// <returns>TRUE ����, ����ȊO ���s</returns>
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
        /// �f�[�^�̎擾
        /// </summary>
        /// <returns>�f�[�^</returns>
        // ********************************************************************************
        virtual const void* GetData() const noexcept override
        {
            return m_pBinaryArray;
        }

        // ********************************************************************************
        /// <summary>
        /// �f�[�^�T�C�Y�̎擾
        /// </summary>
        /// <returns>�f�[�^�T�C�Y</returns>
        // ********************************************************************************
        virtual const MofS32 GetDataSize() const noexcept override
        {
            return m_DataSize;
        }
    };
}

