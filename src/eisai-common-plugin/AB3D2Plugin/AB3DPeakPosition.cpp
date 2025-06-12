///*********************************************************/
///*!
// @addtoGroup AB3D
// @file AB3DPeakPosition.cpp
// @brief AB3D Peak Position Algorizhm
// 
// @date 2013.07.01
// @author M.Fukuda
// @version 1.0.0
//
// -------------------------------------------------------
// Copyright(C) 2006-2014 
// Eisai Co., Ltd. All rights reserved.
// -------------------------------------------------------
// @note
//    AB3D�I�u�W�F�N�g�Ɠ����������Ă��܂��B
//    Plugin Call ����Mass++�I�u�W�F�N�g�ϊ��̃R�X�g���傫���̂ŁA
//    �ʏ��PluginCall�Ƃ͈قȂ�t���[�ŏ������Ă��܂��B
// */
///*********************************************************/
//
////
//// ���؎����ɂ��āB
//// PeakPosition��臒l�̌v�Z�͗v�����ł��B
//// �i�荞�݌�A�����̃s�[�N������ꍇ�̏����ɂ��Ă��v�����ł��B
////
//
//#include "stdafx.h"
//#include "AB3DPeakPosition.h"
//
//// include AB3D Parts
//#include "NewAB3DLabeling.h"
//
//// include DB Controler
//#include "DBTools\DbController.h"
//#include "DBTools\tempContainer.h"
//
//// include progress controler
//#include "progress\progressMatrix.h"
//
//#define AB3D_SHORT_NAME "ab3d2"
//
//#include <fstream>
//
//// constructor
//AB3DPeakPositions::AB3DPeakPositions(
//	const double mzppm,
//	const double rttol,
//	std::map<std::string, std::string>& setmap,
//	kome::objects::SettingParameterValues* settings
//	) : mzppm_(mzppm)
//	, rttol_(rttol)
//	, setmap_(setmap)
//	, settings_(settings)
//{}
//
////********************************
//// ���s��
////********************************
//// �T�v
//// 01.AB3D�̒��ŁA���ƂȂ� XIC��XYData �f�[�^�APeak�̏����擾/�ۑ�
//// 02.AB3D�̌㔼�̏����ŁA[Peak�ł���]�f�[�^���X�V����(Flag�𗧂Ă�)
//// 03.�S�T���v���ŁAAB3D���I�������A[Peak�ł���]�f�[�^��S�Ď擾����
//// 04.�T���v������Peak�ŁA臒l���� M/Z, RT, Charge�̒l��Z�߁A
////    �m�肵���l����A���e�[�u����Matrix�@Row(Peak�l)�Ƃ��ď������ށB
//// 05.�t������eValue�̒l��TempTable����R�s�[����B
//// 06.�K�v�ȕ��̌v�Z��S�čs���ƁAMatrix���o���オ��B
//// 07.�����I������Temp Table���j�������̂ŁA�s�v�Ȓl��DB�ɂ͎c��Ȃ��B
////--------------------------------
//const bool AB3DPeakPositions::execute (
//	kome::core::Progress& progress
//) {
//	// DB Object
//	std::unique_ptr<AB3D::DbController> dbctrl(new AB3D::DbController);
//
//	// check DB Status
//	if (dbctrl->checkDBStatus() && (dbctrl->setupTempTables() != true)){
//		// DB���g���Ȃ� or TempTable�����Ȃ��̂ŏ����ł��Ȃ�
//		LOG_ERROR_CODE(FMT("Failed to access database."),ERR_DB);
//		return false;
//	}
//
//	// Execute AB3D
//	if (onExecAB3D(dbctrl.get(), progress)) {
//		// Exec Create Matrix
//		if(onCreateMatrix(dbctrl.get(), progress)) {
//			// ��������
//			LOG_INFO(FMT("Complete create matrix."));
//			return true;
//		} else {
//			// CreateMatrix�Ɏ��s
//			LOG_ERROR(FMT("Failed to create matrix table."));
//		}
//	} else {
//		//AB3D�Ɏ��s
//		LOG_ERROR(FMT("Failed to execute AB3D."));
//	}
//	return false;
//}
//
//// AB3D���s��
//const bool AB3DPeakPositions::onExecAB3D(
//	AB3D::DbController* dbctrl,
//	kome::core::Progress& progress
//){
//	//AB3D object
//	NewAB3DLabeling ab3d(setmap_);
//
//	// Progress ���X�^�[�g
//	MatrixProg prog(&progress, dbctrl->getSampleNumbers());
//
//	// �T���v�����������J��Ԃ��B
//	while(dbctrl->doNextSample() && !progress.isStopped() ) { // @date 2013.11.22 <Mod> M.Izumi
//		kome::objects::DataSet dataSet;
//		//�v�Z�p�̃I�u�W�F�N�g��DB����擾����
//		if(dbctrl->getNowDataGroup(dataSet)) {
//			kome::core::Progress* subProgress = prog.getSubProgress();
//			ab3d.bindProgress(subProgress);
//			// AB3D�����s�B
//			ab3d.doAB3D(dataSet, dbctrl);
//		} else {
//			// dataSet���擾�ł��Ȃ��ꍇ�B
//			LOG_ERROR(FMT("Faild to the dataSet"));
//			return false;
//		}
//	}
//	prog.closeMe();
//	return true;
//}
//
//// Matrix���쐬����
//// Progress�̊Ǘ��ړr�҂�
//const bool AB3DPeakPositions::onCreateMatrix(
//	AB3D::DbController* dbctrl,
//	kome::core::Progress& progress
//){
//	return this->onCreateMatrix(dbctrl);
//}
//
//// Matrix���쐬����
//const bool AB3DPeakPositions::onCreateMatrix(
//	AB3D::DbController* dbctrl
//){
//	std::vector<AB3D::TmpPeakVal*> tmpPeaks;
//	std::vector<AB3D::TmpPeakVal*> errPeaks;
//	
//	// Temp Table���� Charge > -2 ��Peak�����AValue�̍~���Ŏ擾����B
//	// TmpPeakVal�͓�����[New]����Ė߂�B
//	if(dbctrl->getMatchDataFromTemp(tmpPeaks)) {
//
//		std::vector<stRows> rowArr;
//		// Matrix Peaks Row�ׂ̈̃I�u�W�F�N�g�����B
//		std::for_each(tmpPeaks.begin(), tmpPeaks.end(), [&](AB3D::TmpPeakVal* p){
//			if(!setupMatrixPeakRow(p, rowArr)) {
//				errPeaks.push_back(p);
//			}
//		});
//		// for Debug 
//		//debugPrint(tmpPeaks,errPeaks,rowArr);
//
//		// Matrix DB�ɃI�u�W�F�N�g�̗v�f����������
//		this->onWriteToMatrix(dbctrl, rowArr);
//
//		// New���ꂽ�I�u�W�F�N�g��Delete����B
//		for each(auto tpv in tmpPeaks){delete tpv;}
//
//		//std::for_each(tmpPeaks.begin(), tmpPeaks.end(), [](const AB3D::TmpPeakVal* tpv){
//		//	delete tpv;
//		//});
//		
//	} else {
//		// �擾�Ɏ��s or ���ʂ������Ȃ�
//		LOG_ERROR(FMT("No Result"));
//		return false;
//	}
//	return true;
//}
//
//// Matrix Peaks Row�ׂ̈̃I�u�W�F�N�g�����B
//// Labeling & Marge�ɓ����鏈�����s���B
//const bool AB3DPeakPositions::setupMatrixPeakRow(
//	AB3D::TmpPeakVal* pt,
//	std::vector<stRows>& rowArr
//){
//	bool newFg = true;
//	bool ret = true;
//	std::vector<stRows>::iterator it = rowArr.begin();
//	std::vector<stRows>::iterator eit = rowArr.end();
//
//	for(; it != eit && newFg; ++it) {
//		//�o�^�ς�ROW(it)�͈͓̔����𔻒�
//		if(isInRow(pt, *it)) {
//			// �͈͓�
//			newFg = false;
//			if(!isSameIdInRow(pt->sampleId, *it)) {
//				// SampleID���d�������B�q�̗v�f�Ƃ��ēo�^����B
//				it->vals.push_back(pt);
//			} else {
//				//����Sample����臒l���̃s�[�N������ꍇ�㏑���u�o�^���Ȃ��v
//				//�ʓrPeak Row�𕪂���K�v���L��ꍇ�͋L�q�B
//				ret = false;
//			}
//		}
//	}
//	//�o�^�ς݂�ROW�ƈ�v�����B�V�K�v�f�Ƃ��ēo�^����B
//	if(newFg) {
//		rowArr.push_back(createRows(pt));
//	}
//	return ret;
//}
//
//// �Ώۂ� row��臒l�Ɠ������ۂ���Ԃ�
//const bool AB3DPeakPositions::isInRow(
//	AB3D::TmpPeakVal* p,
//	const stRows& row
//){
//	// Charge�s��v
//	if (p->charge != row.charge) return false;
//	// m/z��臒l����O���
//	if (p->mz < row.mztol.lo || row.mztol.hi < p->mz) return false;
//	// rt��臒l����O���
//	if (p->rt < row.rttol.lo || row.rttol.hi < p->rt) return false;
//	// 臒l���Ŋ���Charge�������B�̂Ɉ�v�B
//	return true;
//}
//
////�@����̒��ɁA����Sample�̃s�[�N�����邩�ۂ��T���B
//const bool AB3DPeakPositions::isSameIdInRow(
//	const int tgtId, 
//	const stRows& tgtRow
//){
//	for each(auto& t in tgtRow.vals) {
//		if(t->sampleId == tgtId) return true;
//	}
//	return false;
//}
//
//// row�𐶐�����
//stRows AB3DPeakPositions::createRows(
//	AB3D::TmpPeakVal* p
//){
//	stRows rst(p->mz, p->rt, p->charge);
//	double mztol = p->mz * 0.000001 * mzppm_;
//	// 臒l����p�͈̔͂��v�Z���ĕێ�����B
//	rst.mztol = tolerance(p->mz, mztol);
//	rst.rttol = tolerance(p->rt, rttol_);
//	rst.vals.push_back(p);
//	return rst;
//}
//
//
//// Matrix�̎��̂��쐬����
//const bool AB3DPeakPositions::onWriteToMatrix(
//	AB3D::DbController* dbctrl,
//	std::vector<stRows>& rowArr
//) {
//	// Matrix�̎��̕����쐬�ׂ̈�STMT���쐬����B
//	if (!rowArr.empty() && dbctrl->setupMsppSTMT()) {
//		std::for_each(rowArr.begin(), rowArr.end(), [&](stRows& r){
//			dbctrl->writeRowLine(r.mz, r.rt, r.charge, r.vals);
//		});
//		dbctrl->finishMakeMatrix(true);
//		return true;
//	}
//	return false;
//}
//
////-----------------------------------------------------
////For Debug Matrix only
////-----------------------------------------------------
//void AB3DPeakPositions::debugPrint(
//	std::vector<AB3D::TmpPeakVal*>& tmpPeaks,
//	std::vector<AB3D::TmpPeakVal*>& errPeaks,
//	std::vector<stRows>& rowArr
//){
//	std::string AllResults = "R:\\tmpPeaks.txt";
//	std::string AllRow = "R:\\rowvals.txt";
//	std::string ErrResults = "R:\\errPeaks.txt";
//
//	//--AB3D�̑S���ʂ������܂��B
//	this->debugOutputTmpPeaksValues(AllResults, tmpPeaks);
//	//--Matrix���\������I�u�W�F�N�g�̓��e�������܂�
//	this->debugOutputRowValues(AllRow, rowArr);
//	//--Labeling&Merge�̃A���S���Y���ɂ��u�o�^����Ȃ��v�f�[�^�������܂�
//	this->debugOutputTmpPeaksValues(ErrResults, errPeaks);
//}
//
//// Debug�p TmpPeaksValues�̒��g��S�ďo��
//void AB3DPeakPositions::debugOutputTmpPeaksValues(
//	const std::string& outputfilePath,
//	const std::vector<AB3D::TmpPeakVal*>& tmpPeaks
//){
//	std::ofstream ofs(outputfilePath);
//	ofs << getTmpPeakAllElements(nullptr) << std::endl;
//
//	std::for_each(tmpPeaks.begin(),tmpPeaks.end(),
//		[&](AB3D::TmpPeakVal* p){ofs << getTmpPeakAllElements(p) << std::endl;});
//	ofs.close();
//}
//
//// Debug Row Values�̒��g���m�F�p
//void AB3DPeakPositions::debugOutputRowValues(
//	const std::string& outputfilePath,
//	const std::vector<stRows>& rowArr
//){
//	std::ofstream ofs2(outputfilePath);
//	ofs2 << "mxId,m/z,rt,z," << getTmpPeakAllElements(nullptr) << std::endl;
//	std::string hs(""),ss("");
//	int cnt = 1;
//
//	for each(auto& r in rowArr) {
//		hs = FMT("%d,%f,%f,%d,",cnt, r.mz, r.rt, r.charge);
//		for each(auto& vt in r.vals) {
//			ss = getTmpPeakAllElements(vt);
//			ofs2 << hs << ss << std::endl;
//		}
//		cnt++;
//	}
//	ofs2.close();
//}
//
//// Debug�p
//const std::string AB3DPeakPositions::getTmpPeakAllElements(
//	AB3D::TmpPeakVal* p
//){
//	std::string ts;
//	if(p != nullptr) {
//		ts = FMT("%f,%f,%d,%d",p->mz, p->rt, p->charge, p->sampleId);
//		ts += FMT(",%f", p->mxval.value);
//		ts += FMT(",%f,%f", p->mxval.core.x, p->mxval.core.y);
//		ts += FMT(",%f,%f", p->mxval.left.x, p->mxval.left.y);
//		ts += FMT(",%f,%f", p->mxval.right.x, p->mxval.right.y);
//	} else {
//		ts = "m/z,rt,z,sampleId,value,Core.x,Core.y,Left.x,Left.y,Right.x,Right.y";
//	}
//	return ts;
//}
//
////AB3D �����̂̂���TempTable�ɓf���o���āA���s����
//const bool AB3DPeakPositions::executeDebug (
//) {
//	// DB Object
//	std::unique_ptr<AB3D::DbController> dbctrl(new AB3D::DbController);
//	dbctrl->setupTempSTMT();
//	dbctrl->checkDBStatus();
//	bool ret = true;
//	//FeedBack Results from temp table
//	ret = this->onCreateMatrix(dbctrl.get());
//	
//	return true;
//}
