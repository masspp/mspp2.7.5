///*********************************************************/
///*!
// @addtoGroup AB3D
// @file proxyDBClass.h
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
//    Mass++��EMS��DB�̎������قȂ邽�߁A
//	proxy�ŃC���X�^���X�����킹�Ă����A
//	�p�����Mass++��EMS�Ő؂�ւ��Đ����������킹��B
// */
///*********************************************************/
//#include "stdafxwp.h"
//#include "proxyDBClass.h"
//#include "DBAccess.h"
//#include "tempContainer.h"
//
//#include <boost/assign/list_of.hpp>
//
//using namespace AB3D;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

//
//// constructor
//DbProxyClass::DbProxyClass() : ptr_(new DbAccess){}
//
//// destructor
//DbProxyClass::~DbProxyClass(){}
//
//// �㏈���B�f�X�g���N�^�Ƃ͕ʂɍs��
//void DbProxyClass::closeMe() 
//{
//}
//
////������
//const bool DbProxyClass::initMe()
//{
//	return onInitMe();
//}
//
////����������
//const bool DbProxyClass::onInitMe()
//{
//	return true;
//}
//
//// Check Ready DataBase.
//const bool DbProxyClass::isReadyDB(
//	int& refMatrixId,
//	std::vector<int>& refSampleIds
//) {
//	if(this->onGetMatrixId(refMatrixId)) {
//		matrixID_ = refMatrixId;
//		return this->onGetSampleIds(refMatrixId,refSampleIds);
//	}
//	return false;
//}
//
//// Open TargetID Sample
//const bool DbProxyClass::openTargtIdSample(
//	const int sampleId,
//	kome::objects::DataSet& refDataSet
//) {
//	return this->onOpenTargtIdSample(sampleId, refDataSet);
//}
//
//// ����̃A�N�e�B�u�T���v�����J��
//void DbProxyClass::closeActiveSample(
//){
//	this->onCloseActiveSample();
//}
//
////���ʂ��R�~�b�g����
//void DbProxyClass::commitMe()
//{
//	this->onCommitMe();
//}
//
//// �s�[�N�̏�����������
//const int DbProxyClass::setPeak(
//	const int mtId,
//	const double mz,
//	const double rt,
//	const int charge
//){
//	return onSetPeak(mtId, mz, rt, charge);
//}
//
//// Buffer����������
//const int DbProxyClass::setXYBuffer(
//	kome::core::Buffer* buffer
//){
//	return onSetXYBuffer(buffer);
//}
//
//// Value���Z�b�g����
//const bool DbProxyClass::setValues(
//	const int peakId,
//	const int sampleId,
//	const int bufId,
//	AB3D::TmpMXVal& tmval
//) {
//	return onSetValues(peakId, sampleId, bufId, tmval);
//}
