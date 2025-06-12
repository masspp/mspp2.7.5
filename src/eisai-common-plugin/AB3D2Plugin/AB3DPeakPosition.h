///*********************************************************/
///*!
// @addtoGroup AB3D
// @file AB3DPeakPosition.h
// @brief AB3D Peak Position Algorizhm
// 
// @date 2013.07.01
// @author M.Fukuda
// @varsion 1.0.0
//
// -------------------------------------------------------
// Copyright(C) 2006-2013 
// Eisai Co., Ltd. All rights reserved.
// -------------------------------------------------------
// @note
// */
///*********************************************************/
//#pragma once
//
//// �O���錾
//namespace AB3D {
//	class DbController;
//	struct TmpPeakVal;
//}
//
//class NewAB3DLabeling;
//
//// �ŏ�/�ő��臒l��ۑ�����\����
//struct tolerance {
//	double lo;
//	double hi;
//	// �f�t�H���g�R���X�g���N�^
//	tolerance(){}
//	// �R���X�g���N�^�Ŕ͈͂��v�Z����
//	tolerance(const double val, const double tol) 
//		: lo(val - tol)
//		, hi(val + tol){}
//};
//
//// Peaks Row�̒l��ۑ�����\����
//// *TmpPeakVal��TempContiner.h�@�ŋL�q�B
//struct stRows {
//	// Row�ɕ\�������
//	double mz;
//	double rt;
//	int charge;
//	// 臒l���O�̔�r���Ɏg�p
//	tolerance mztol; 
//	tolerance rttol; 
//	// Peaks Row�̎q�̗v�f
//	std::vector<AB3D::TmpPeakVal*> vals;
//
//	//constructor(1)
//	stRows(){}
//
//	// constrctor(2)
//	stRows(
//		const double _mz, 
//		const double _rt, 
//		const int _cg
//		) : mz(_mz)
//		, rt(_rt)
//		, charge(_cg){}
//};
//
//// AB3D��Matrix�쐬�ׂ̈̃A���S���Y���B
//class AB3DPeakPositions {
//public:
//	// constructor
//	explicit AB3DPeakPositions(
//		const double mzppm,
//		const double rttol,
//		std::map<std::string, std::string>& setmap,
//		kome::objects::SettingParameterValues* settings);
//	// destructor
//	~AB3DPeakPositions(){};
//
//private:
//	// Uncopyable
//	AB3DPeakPositions(const AB3DPeakPositions&);
//	AB3DPeakPositions operator=(const AB3DPeakPositions&); 
//	
//private:
//	// Inner parameters
//	double mzppm_;
//	double rttol_;
//	std::map<std::string, std::string> setmap_;
//
//	kome::objects::SettingParameterValues* settings_;
//
//public:
//
//	/*!
//	 * @brief AB3D Peak Position �����s����
//	 * @param[in]  progress
//	 * @retval     �����Ȃ�True���Ԃ�
//	 */
//	const bool execute(kome::core::Progress& progress);
//
//	/*!
//	 * @brief AB3D Peak Position �݂̂����s����
//	 * @param[in]  progress
//	 * @retval     �����Ȃ�True���Ԃ�
//	 * @note       AB3D�̌��ʂ�TempTable�ɂ��鎖���O��
//	 */
//	const bool executeDebug ();
//
//private:
//
//	/*!
//	 * @brief      AB3D�����s����
//	 * @param[in]  DBCtrl�I�u�W�F�N�g�̃|�C���^
//	 * @param[in]  progress
//	 * @retval     �����Ȃ�True���Ԃ�
//	 */
//	const bool onExecAB3D(
//		AB3D::DbController* dbctrl,
//		kome::core::Progress& progress);
//
//	/*!
//	 * @brief      ��DB��Peak Position�̌��ʂ���������
//	 * @param[in]  DBCtrl�I�u�W�F�N�g�̃|�C���^
//	 * @param[in]  progress
//	 * @retval     �����Ȃ�True
//	 */
//	const bool onCreateMatrix(
//		AB3D::DbController* dbctrl,
//		kome::core::Progress& progress);
//
//	/*!
//	 * @brief      ��DB��Peak Position�̌��ʂ���������
//	 * @param[in]  DBCtrl�I�u�W�F�N�g�̃|�C���^
//	 * @retval     �����Ȃ�True
//	 */
//	const bool onCreateMatrix(
//		AB3D::DbController* dbctrl);
//
//	const bool setupMatrixPeakRow(
//		AB3D::TmpPeakVal* pt,
//		std::vector<stRows>& rowArr);
//
//	const bool onWriteToMatrix(
//		AB3D::DbController* dbctrl,
//		std::vector<stRows>& rowArr);
//
//	stRows createRows(AB3D::TmpPeakVal* pt);
//
//	const bool isInRow(
//		AB3D::TmpPeakVal* pt, 
//		const stRows& tgtRow);
//
//	const bool isSameIdInRow(
//		const int tgtId, 
//		const stRows& tgtRow);
//
//
//	//----------------
//	//	Debug �p�֐�
//	//----------------
//
//	void debugPrint(
//		std::vector<AB3D::TmpPeakVal*>& tmpPeaks,
//		std::vector<AB3D::TmpPeakVal*>& errPeaks,
//		std::vector<stRows>& rowArr
//	);
//
//	void debugOutputRowValues(
//		const std::string& outputfilePath,
//		const std::vector<stRows>& rowArr);
//
//	void debugOutputTmpPeaksValues(
//		const std::string& outputfilePath,
//		const std::vector<AB3D::TmpPeakVal*>& tmpPeaks);
//
//	const std::string getTmpPeakAllElements(
//		AB3D::TmpPeakVal* pt);
//
//};
