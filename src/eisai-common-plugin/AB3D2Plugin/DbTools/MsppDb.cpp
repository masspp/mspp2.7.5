///*********************************************************/
///*!
// @addtoGroup AB3D
// @file MsppDB.h
// @brief AB3D Peak Position Main DB Ctrl
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
//    Mass++��DB�C���^�t�F�[�X��������
// */
///*********************************************************/
//
//#include "stdafxwp.h"
//
//#include "MsppDB.h"
//#include "DBAccess.h"
//#include "tempContainer.h"
//
//// EMS�ł͂Ȃ��Ƃ�
//#ifndef __EMS__
//
//// TypeDef
//typedef kome::ident::IdentificationManager DBMgr;
//
//// DBManager�̃C���X�^���X�擾��Inline��
//inline DBMgr& getDBMgr() 
//{
//	return DBMgr::getInstance();
//}
//
//using namespace AB3D;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

//
//// constructor
//MsppDB::MsppDB() 
//	: AB3D::DbProxyClass()
//	, sampleInfo_()
//	, sample_(nullptr)
//{}
//
//// ������
//const bool MsppDB::onInitMe() 
//{
//	// DB�� MZ, RT, Charge��L���ɂ���
//	getDBMgr().updateMatrixFlags(matrixID_, true, true, true);
//	return true;
//}
//
//// Open TargetID Sample
//const bool MsppDB::onOpenTargtIdSample(
//	const int sampleId,
//	kome::objects::DataSet& refDataSet
//) {
//	// SampleID(COL)����SampleID�Ƃ͈قȂ�̂ŁA�ϊ�����
//	LOG_DEBUG(FMT("SampleID = %d", sampleId));
//	int keyID = getDBMgr().getMatrixSampleSample(sampleId);
//
//	LOG_DEBUG(FMT("keyID = %d  SampleID = %d", keyID, sampleId));
//
//	sample_ = getDBMgr().openSample(keyID, sampleInfo_);
//	
//	if (sample_ != nullptr) {
//		kome::objects::DataGroupNode* dgn = sample_->getRootDataGroupNode();
//		if(dgn != nullptr) {
//			dgn->getDataSet(&refDataSet);
//			return true;
//		}
////		refDataSet = sample_->getRootDataGroupNode();
//	}
//	return false;
//}
//
//// ����̃A�N�e�B�u�T���v�����J��
//void MsppDB::onCloseActiveSample()
//{
//	if (sample_ != nullptr) {
//		getDBMgr().releaseSample(sample_,sampleInfo_);
//	}
//}
//
//// IFDEF �Ő؂�ւ���BMatrix ID /Project ID���󂯎��
//const bool MsppDB::onGetMatrixId(int& refMatrixId) 
//{
//	refMatrixId = getDBMgr().getSelectedMatrix();
//	return (refMatrixId >= 0);
//}
//
//// IFDEF �Ő؂�ւ���B
//// SampleIds���󂯎��B
//const bool MsppDB::onGetSampleIds(
//	const int matrixId,
//	std::vector<int>& refSampleIds
//){
//	std::vector<int> groupIds, sampleIds;
//	DBMgr& dbmgr = getDBMgr();
//	// Group ID���󂯎��
//	dbmgr.getGroups(groupIds, matrixId);
//	for each(const int groupId in groupIds) {
//		sampleIds.clear();
//		// Group ID����SampleID(COL_ID)���󂯎��
//		dbmgr.getMatrixSamplesFromGroup(sampleIds, groupId);
//		for each(const int sampleId in sampleIds) {
//			refSampleIds.push_back(sampleId);
//		}
//	}
//	return (!refSampleIds.empty());
//}
//
//
//// Buffer-> XTData�֕ϊ����āA
//// Profile�ɏ������ށB
//const int MsppDB::onSetXYBuffer(
//	kome::core::Buffer* buffer
//){
//	// Buffer->DPS�ϊ��ւ̕W���I�Ȏ葱��
//	kome::core::DataPoints dps(kome::core::DataPoints::FLOAT);
//	dps.importData(boost::bind(&kome::core::DataAccessor::read, buffer, _1, _2 ));
//
//	return getDBMgr().setProfile(dps,true);
//}
//
//// Peak ������������
//const int MsppDB::onSetPeak(
//	const int mtId,
//	const double mz,
//	const double rt,
//	const int charge
//){
//	return getDBMgr().addMatrixPeak(rt, mz, charge, mtId);
//}
//
//// Peak�̃I�u�W�F�N�g���ꎞ�쐬���āA
//// Peak�̏�����������
//const bool MsppDB::onSetValues(
//	const int peakId,
//	const int sampleId,
//	const int bufId,
//	AB3D::TmpMXVal& tmval
//){
//	kome::objects::PeakElement pk(NULL);
//	pk.setX(tmval.core.x);
//	pk.setY(tmval.core.y);
//	pk.setLeft(tmval.left.x, tmval.left.y);
//	pk.setRight(tmval.right.x, tmval.right.y);
//
//	getDBMgr().setPeakValue(peakId, sampleId, tmval.value, &pk, bufId);
//	return true;
//}
//
//// �R�~�b�g����
//void MsppDB::onCommitMe()
//{
//	getDBMgr().commit();
//}
//
//#endif
