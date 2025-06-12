///*********************************************************/
///*!
// @addtoGroup AB3D
// @file proxyDBClass.h
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
//    Mass++��EMS��DB�̎������قȂ邽�߁A
//	proxy�ŃC���X�^���X�����킹�Ă����A
//	�p�����Mass++��EMS�Ő؂�ւ��Đ����������킹��B
// */
///*********************************************************/
//#pragma once
//
//// �O���錾
//namespace AB3D {
//	class DbAccess;
//	struct TmpMXVal;
//	struct ElmPoint;
//}
//
//namespace AB3D {
//
///*!--------------------------------------------------------
//    @name   DbProxyClass
//    @brief  Data Base Proxy Class
//    @date   2013.07.01
//    @par    �قȂ�����ɑΉ����邽�߂̋��ʃC���^�[�t�F�[�X�̃N���X
//
//	���L�ɂ��Čp�����ɖ{�̂��K�v
//		+ onInitMe
//		+ onCommitMe
//		+ onCloseActiveSample
//		+ onOpenTargtIdSample
//		+ onGetMatrixId
//		+ onGetSampleIds
//		+ onSetXYBuffer
//		+ onSetPeak
//		+ onSetValues
//
//-----------------------------------------------------------*/
//	class DbProxyClass {
//	
//	public:
//		// constructor
//		DbProxyClass();
//		// destructor
//		~DbProxyClass();
//
//	private:
//		// Uncopyable
//		DbProxyClass(const DbProxyClass&);
//		DbProxyClass& operator=(const DbProxyClass&);
//
//	protected:
//		// DB Accesser ���쐬���ĕێ�����
//		std::unique_ptr<DbAccess> ptr_;
//		// ���ݍ쐬����Matrix ID��ێ�����
//		int matrixID_;
//
//		kome::ident::PeakPositions* peakpos_;
//
//	public:
//		/*!
//		 * @brief  ���g�̏��������s��
//		 * @retval ���s���� False 
//		 * @note   onInitMe������I�ɌĂ�
//		 */
//		const bool initMe();
//
//		/*!
//		 * @brief  DB���g�p�ł��邩�`�F�b�N����
//		 * @param[out]  �A�N�e�B�u��MatrixID
//		 * @param[out]  ���̗ނ���T���v����ID�Q
//		 * @retval ���s���� False 
//		 * @note + onGetMatrixId
//		         + onGetSampleIds
//		 */
//		const bool isReadyDB(
//			int& refMatrixId,
//			std::vector<int>& refSampleIds);
//
//		/*!
//		 * @brief  �w��ID�̃T���v���f�[�^���擾
//		 * @param[in]   �T���v��ID
//		 * @param[out]  �T���v����DataSet
//		 * @retval ���s���� False 
//		 * @note   + onOpenTargtIdSample
//		 */
//		const bool openTargtIdSample(
//			const int sampleId,
//			kome::objects::DataSet& refDataSet);
//
//		/*!
//		 * @brief  DB�ŊJ�����A�N�e�B�u�T���v�������
//		 * @note   + onCloseActiveSample
//		 */
//		void closeActiveSample();
//
//		/*!
//		 * @brief  Buffer���e�[�u���ɏ�������
//		 * @param[in] ��������Buffer
//		 * @retval    ��������BufferID�B���s���� -1
//		 * @note   + onSetXYBuffer
//		 */
//		const int setXYBuffer(
//			kome::core::Buffer* buffer);
//
//		/*!
//		 * @brief  Peak�����e�[�u���ɏ�������
//		 * @param[in] MatrixID
//		 * @param[in] M/Z
//		 * @param[in] Retention time(Min)
//		 * @param[in] Charge
//		 * @param[in] Value
//		 * @retval    ��������PeakID�B���s���� -1
//		 * @note   + onSetPeak
//		 */
//		const int setPeak(
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
//		 * @note   + onSetValues
//		 */
//		const bool setValues(
//			const int peakId,
//			const int sampleId,
//			const int bufId,
//			AB3D::TmpMXVal& tmval);
//		
//		/*!
//		 * @brief  �I������
//		 */
//		void closeMe();
//
//		/*!
//		 * @brief �R�~�b�g
//		 * @note + onCommitMe
//		 */
//		void commitMe();
//
//	protected:
//		// �ȍ~�A�C���X�^���X�͋��ʂ��Ă��邪�A
//		// ���g�͌ʂɏ����������ق����ǂ������B
//
//		/*!
//		 * @brief  ���g�̏��������s��
//		 * @retval ���s���� False 
//		 */
//		virtual const bool onInitMe();
//
//		/*!
//		 * @brief  �w��ID�̃T���v���f�[�^���擾�̎�����
//		 * @param[in]   �T���v��ID
//		 * @param[out]  �T���v����DataSet
//		 * @retval ���s���� False 
//		 */
//		virtual const bool onOpenTargtIdSample(
//			const int sampleId,
//			kome::objects::DataSet& refDataSet) = 0;
//
//		/*!
//		 * @brief  DB�ŊJ�����A�N�e�B�u�T���v������������
//		 */
//		virtual void onCloseActiveSample() = 0;
//
//		/*!
//		 * @brief  �A�N�e�B�u��MatrixID���擾����
//		 * @param[out]  MatrixID
//		 * @retval ���s���� False 
//		 */
//		virtual const bool onGetMatrixId(
//			int& refMatrixId) = 0;
//
//		/*!
//		 * @brief  �w��MatrixID�̃T���v��ID�Q���擾����
//		 * @param[in]   MatrixID
//		 * @param[out]  �T���v����ID�Q
//		 * @retval ���s���� False 
//		 */
//		virtual const bool onGetSampleIds(
//			const int matrixId,
//			std::vector<int>& refSampleIds) = 0;
//
//		/*!
//		 * @brief  Buffer���e�[�u���ɏ�������
//		 * @param[in] ��������Buffer
//		 * @retval    ��������BufferID�B���s���� -1
//		 */
//		virtual const int onSetXYBuffer(
//			kome::core::Buffer* buffer) = 0;
//
//		/*!
//		 * @brief  Peak�����e�[�u���ɏ�������
//		 * @param[in] MatrixID
//		 * @param[in] M/Z
//		 * @param[in] Retention time(Min)
//		 * @param[in] Charge
//		 * @retval    ��������PeakID�B���s���� -1
//		 */
//		virtual const int onSetPeak(
//			const int mtId,
//			const double mz,
//			const double rt,
//			const int charge
//			) = 0;
//
//		/*!
//		 * @brief  Peak�̏ڍׂ��e�[�u���ɏ�������
//		 * @param[in] PeakID
//		 * @param[in] SampleID
//		 * @param[in] Buffer��ID
//		 * @param[in] Peak�̏ڍ׏��\����
//		 * @retval    ���s���� False
//		 */
//		virtual const bool onSetValues(
//			const int peakId,
//			const int sampleId,
//			const int bufId,
//			AB3D::TmpMXVal& tmval) = 0;
//
//		/*!
//		 * @brief �R�~�b�g
//		 */
//		virtual void onCommitMe() = 0;
//	};
//}
//
//
