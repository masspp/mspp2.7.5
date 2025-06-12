/*********************************************************/
/*!
 @addtoGroup AB3D
 @file TempTableCtrl.h
 @brief Temp Table Accesser Base
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
   Peak Position���ɂ�����AAB3D�ł̌��ʂ�
   �ۑ��A�ǂݏo�����߂̃e�[�u�����쐬����
 */
/*********************************************************/
#pragma once

// �O���錾
namespace AB3D {
	class DbAccess;
	struct TmpPeakVal;
	struct TmpMXVal;
}

namespace AB3D {

/*!--------------------------------------------------------
    @name   TmpTblCtrl
    @brief  AB3D�̌��ʂ��ꎞ�ۊǂ���TempTable�̑���
    @date   2013.07.01
    @par    TempTable�̈�A�̑����S��
-----------------------------------------------------------*/
	class TmpTblCtrl {
		
	public:
		// constructor
		TmpTblCtrl();
		// destructor
		~TmpTblCtrl();

	private:
		// Uncopyable
		TmpTblCtrl(const TmpTblCtrl&);
		TmpTblCtrl& operator=(const TmpTblCtrl&);

	private:
		std::unique_ptr<DbAccess> ptr_;

		// STMT���쐬���ĕێ����Ă����B
		// ���j�[�N�|�C���^�ŊǗ�����̂ŁA�j���͎����I�ɂ����B
		// // Temp�e�[�u���ւ̃C���T�[�g(Set)
		std::unique_ptr<kome::db::Statement> stmt_tmpPeak_;
		std::unique_ptr<kome::db::Statement> stmt_tmpVal_;
		std::unique_ptr<kome::db::Statement> stmt_tmpProf_;

		// // Temp�e�[�u���ւ̃A�b�v�f�[�g
		std::unique_ptr<kome::db::Statement> stmt_UpdtCrg_;

		// // Temp�e�[�u������ Select(Get)
		std::unique_ptr<kome::db::Statement> stmt_SlctVal_;
		std::unique_ptr<kome::db::Statement> stmt_SlctProf_;

	public:
		const int createTempTable();

		const bool createTempStatements();

		// �e�[�u�������֐��BDBAccess�Ɋ֐��Ƃ��ēn��
		static void createTempTableAB3DPeak(kome::db::Table& table, const char* tname);
		static void createTempTableAB3DVal(kome::db::Table& table, const char* tname);
		static void createTempTableAB3DProf(kome::db::Table& table, const char* tname);

		// XYBuffer��Tmp�ɋL�^�B�������͂O�ȏ��ID��߂�
		const int setXYBuffer(kome::core::Buffer* buffer);

		// Temp Matrix Value ��Insert
		const int setTmpMXValue(
			const int sampleid,   
			const int bufid,
			const double peak_x,  
			const double peak_y,
			const double left_x,  
			const double left_y,
			const double right_x, 
			const double right_y,
			const double value
			);

		// Temp Peak ��Insert
		const int setTmpPeak(
			const int sampleid,
			const int mxvid,
			const double rt,
			const double mz
			);

		// �O����XYBuffer�����o�����
		const bool getXYBuffer(
			const int bufId,
			kome::core::Buffer& refBuffer
			);

		// Temp�ɓf���o���� �L���Ȓl�݂̂��擾����
		const bool getTemporaryDatas(
			std::vector<TmpPeakVal*>& refTmpPeakVals);

		// Temp Peak �́@Charge��Update.
		void updateCharge(const int peakId, const int charge);

		void closeMe();

	private:
		// Insert��Stmt�̐����B��x����Ă��ׂĎg���܂킷�B
		const bool onCreateStmt_Insert_TmpPEAKS();
		const bool onCreateStmt_Insert_TmpMXVALUES();
		const bool onCreateStmt_Insert_TmpPROFILES();

		// Update��Stmt�̐����B��x����Ă��ׂĎg���܂킷�B
		const bool onCreateStmt_UpdateCharge();

		// Select��Stmt�̐����B��x����Ă��ׂĎg���܂킷�B
		const bool onCreateStmt_SelectTmpMXVALUES();
		const bool onCreateStmt_SelectTmpPROFILES();

		//BUFFER��TEMPTABLE�ɓo�^�B
		const bool onDoInsertBuffer(kome::core::Buffer* buffer);

		// Temp Profile �e�[�u���̍ŐVID���擾
		const int onGetMaxID_TmpPROFILES();

		//MatrixValue��TEMPTABLE�ɓo�^�B
		const bool onSetTmpMXValue(
			const int sampleid,
			const int bufid,
			const double peak_x,
			const double peak_y,
			const double left_x,
			const double left_y,
			const double right_x,
			const double right_y,
			const double value
			);
		
		// Temp Matrix Value �e�[�u���̍ŐVID���擾
		const int onGetMaxID_TmpMXVALUES();

		//PeakValue��TEMPTABLE�ɓo�^�B
		const bool onSetTmpPeak(
			const int sampleid,
			const int mxvid,
			const double rt,
			const double mz
			);

		// TempPeak�e�[�u���̍ŐVID���擾
		const int onGetMaxID_TmpPEAKS();

		// ���ʎ擾�p��SQL�����\�����ĕԂ�
		const std::string onCreateSQL_TempResults();

		const bool onGetTmpMXValue(
			const int mxvId,
			TmpMXVal& refMxval
			);


	};

}
