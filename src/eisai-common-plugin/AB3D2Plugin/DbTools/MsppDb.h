///*********************************************************/
///*!
// @addtoGroup AB3D
// @file MsppDB.h
// @brief AB3D Peak Position Main DB Ctrl
// 
// @date 2013.07.01
// @author M.Fukuda
// @varsion 1.0.0
//
// -------------------------------------------------------
// Copyright(C) 2006-2014
// Eisai Co., Ltd. All rights reserved.
// -------------------------------------------------------
// @note
//    Mass++��DB�C���^�t�F�[�X��������
// */
///*********************************************************/
//#pragma once
//#include "proxyDBClass.h"
//
//#ifndef __EMS__
//
//namespace AB3D {
//
///*!--------------------------------------------------------
//    @name   MsppDB
//    @brief  AB3D�̌��ʂ�Mass++ DB�ɏ�������
//    @date   2013.07.01
//	@par    DbProxyClass ����p���B
//-----------------------------------------------------------*/
//	class MsppDB : public DbProxyClass {
//
//	public:
//		MsppDB();
//	private:
//		// Sample���Ăяo���������ꎞ�ۊ�
//		kome::ident::SampleReadInfo sampleInfo_;
//		// ���݃I�[�v�����̃T���v���̃|�C���^
//		kome::objects::Sample* sample_;
//	private:
//
//		/*!
//		 * @brief  ���g�̏��������s��
//		 * @retval ���s���� False 
//		 */
//		const bool onInitMe();
//
//		/*!
//		 * @brief  �w��ID�̃T���v���f�[�^���擾�̎�����
//		 * @param[in]   �T���v��ID
//		 * @param[out]  �T���v����DataSet
//		 * @retval ���s���� False 
//		 */
//		const bool onOpenTargtIdSample(
//			const int sampleId,
//			kome::objects::DataSet& refDataSet);
//
//		/*!
//		 * @brief  DB�ŊJ�����A�N�e�B�u�T���v������������
//		 */
//		void onCloseActiveSample();
//
//		/*!
//		 * @brief  �A�N�e�B�u��MatrixID���擾����
//		 * @param[out]  MatrixID
//		 * @retval ���s���� False 
//		 */
//		const bool onGetMatrixId(
//			int& refMatrixId);
//
//		/*!
//		 * @brief  �w��MatrixID�̃T���v��ID�Q���擾����
//		 * @param[in]   MatrixID
//		 * @param[out]  �T���v����ID�Q
//		 * @retval ���s���� False 
//		 */
//		const bool onGetSampleIds(
//			const int matrixId,
//			std::vector<int>& refSampleIds);
//
//		/*!
//		 * @brief  Buffer���e�[�u���ɏ�������
//		 * @param[in] ��������Buffer
//		 * @retval    ��������BufferID�B���s���� -1
//		 */
//		const int onSetXYBuffer(
//			kome::core::Buffer* buffer);
//
//		/*!
//		 * @brief  Peak�����e�[�u���ɏ�������
//		 * @param[in] MatrixID
//		 * @param[in] M/Z
//		 * @param[in] Retention time(Min)
//		 * @param[in] Charge
//		 * @retval    ��������PeakID�B���s���� -1
//		 */
//		const int onSetPeak(
//			const int mtId,
//			const double mz,
//			const double rt,
//			const int charge
//			);
//
//		/*!
//		 * @brief  Peak�̏ڍׂ��e�[�u���ɏ�������
//		 * @param[in] PeakID
//		 * @param[in] SampleID
//		 * @param[in] Buffer��ID
//		 * @param[in] Peak�̏ڍ׏��\����
//		 * @retval    ���s���� False
//		 */
//		const bool onSetValues(
//			const int peakId,
//			const int sampleId,
//			const int bufId,
//			AB3D::TmpMXVal& tmval
//		);
//		
//		/*!
//		 * @brief �R�~�b�g
//		 */
//		void onCommitMe();
//	};
//}
//#endif
