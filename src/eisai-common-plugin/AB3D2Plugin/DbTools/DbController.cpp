/*********************************************************/
/*!
 @addtoGroup AB3D
 @file DbController.cpp
 @brief AB3D Peak Position DB Controller
 
 @date 2013.07.01
 @author M.Fukuda
 @version 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
    AB3D Position����Ă΂�āADB�̈�A�̑����㗝����B
	TempTable�͑S�ēƎ���SQL�������Ă���̂œƗ������Ă���B

	DB�̖{�̂֎��f�[�^�̏������݂�
	PeakPositions�N���X�����������̂�ProxyDB�N���X�͔p�~����B
 */
/*********************************************************/

// AB3D����Ă΂�����S������B
#include "stdafxwp.h"

#include "DbController.h"
#include "TempTableCtrl.h"
#include "tempContainer.h"

using namespace AB3D;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DbController::DbController() 
	: tblPtr_(new TmpTblCtrl)
//	, pxyPtr_(new kTargetDBType)
	, matrixId_(-1)
//	, nowCount_(0)
	, nowSampleId_(-1)
//	, sampleIds_()
{}

// destructor
DbController::~DbController(){}

//// �\�z�ɂ��A�Œ����TB�̏�Ԃ��`�F�b�N����
//const bool DbController::checkDBStatus()
//{
//	bool ret = pxyPtr_->isReadyDB(matrixId_, sampleIds_);
//	nowCount_ = 0;
//	nowSampleId_ = -1;
//	return ret;
//}

//Temp�e�[�u�����쐬����
const bool DbController::setupTempTables()
{
	if (tblPtr_->createTempTable() == kDBERROR) return false;
	bool ret = tblPtr_->createTempStatements();
	return ret;
}

// TempTable�̃X�e�[�g�����g���Z�b�g���Ă���
const bool DbController::setupTempSTMT()
{
	return tblPtr_->createTempStatements();
}


void DbController::setSampleId(const int spId)
{
	nowSampleId_ = spId;
}

////***** SampleGetter�N���X������̂ŁA����ɔC����B
////***** �A���A�d�l��AnowSmapleId�����͔c�����Ă����K�v���L��̂ŁA
////***** ���̎擾�p�̃C���^�t�F�[�X�ɕύX������(setSampleId��ǉ�)
//// ���̃T���v���Z�b�g�ɋ��i�߂�B
//// ���������Ȃ�����False��Ԃ��B
//const bool DbController::doNextSample() 
//{
//	this->onCloseActivegroup();
//
//	if (nowCount_ < sampleIds_.size()) {
//		nowSampleId_ = sampleIds_[nowCount_];
//		nowCount_++;
//		return true;
//	}
//	// nothing more sample.
//	return false;
//}

////***** SampleGetter�N���X������̂ŁA����ɔC����B
////�A�N�e�B�u��Ԃ̃T���v�����N���[�Y(�����[�X)����
//void DbController::onCloseActivegroup() 
//{
//	pxyPtr_->closeActiveSample();
//}

////***** SampleGetter�N���X������̂ŁA����ɔC����B
////���ݎw��q�ɂ���T���v���̃f�[�^�Z�b�g���擾����
//const bool  DbController::getNowDataGroup(
//	kome::objects::DataSet& refDataSet
//){
//	return pxyPtr_->openTargtIdSample(nowSampleId_, refDataSet);
//}

// Get DataBase Pointer(Interface)
kome::db::Database* DbController::onGetDB() 
{
	return kome::db::DbManager::getInstance().getDb();
}

// TempTable��XIC�̃f�[�^���Z�b�g����
const int DbController::setXICData(
	const std::vector<std::pair<double, double>>& xicXyDatas,
	const double rt,
	const double mz,
	const double value,
	const double peak_top_x,
	const double peak_top_y,
	const double peak_left_x,
	const double peak_left_y,
	const double peak_right_x,
	const double peak_right_y
){
	// temp table�Ɂ@profile(xydata�̎���)����������
	int profId = setTempProfile(xicXyDatas);

	//kome::core::Buffer rstBuf;
	//this->onTranceToBuffer(xicXyDatas, rstBuf);
	//int profId = tblPtr_->setXYBuffer(&rstBuf);

	// temp Table �Ɂ@�s�[�N������������
	int mxvId = tblPtr_->setTmpMXValue(
		nowSampleId_,
		profId,
		peak_top_x,
		peak_top_y,
		peak_left_x,
		peak_left_y,
		peak_right_x,
		peak_right_y,
		value
		);
	if (mxvId > 0) {
		// temp Table �� M/Z, RT�@���������ށB
		return tblPtr_->setTmpPeak(nowSampleId_, mxvId, rt, mz);
	} else {
		// �쐬���ꂸ�B
		return -1;
	}
}

// TempTable��Profile����������
const int DbController::setTempProfile(
	const std::vector<std::pair<double, double>>& xicXyDatas
){
	kome::core::Buffer rstBuf;
	tranceToBuffer(xicXyDatas, rstBuf);
	return tblPtr_->setXYBuffer(&rstBuf);
}

// Trance Buffers.
// X,Y����Buffer�̌`���ɕϊ�����
const bool DbController::tranceToBuffer(
	const std::vector<std::pair<double, double>>& xicXyData,
	kome::core::Buffer& rstBuffer
) {
	rstBuffer.clear();
	unsigned long bufLen = xicXyData.size();
	rstBuffer.write(&bufLen, sizeof(bufLen));

	if (bufLen > 0) {
		double* arr = new double[bufLen*2];
		for(unsigned int i = 0; i < bufLen; i++) {
			arr[i*2] = xicXyData[i].first;
			arr[i*2+1] = xicXyData[i].second;
		}
		rstBuffer.write(arr, sizeof(double) * bufLen *2);
		delete arr;
	}
	return true;
}


// �w��PeakId��Charge���X�V����
// (Peak�쐬���ɂ�Charge���s���Ȃ̂ŁA��t���ŏC������)
void DbController::updateCharge(
	const int peakId,
	const int charge
){
	tblPtr_->updateCharge(peakId, charge);
}

// Temp Table���� Peak�f�[�^���擾����B
const bool DbController::getMatchDataFromTemp(std::vector<TmpPeakVal*>& tpvArr)
{
	bool ret = tblPtr_->getTemporaryDatas(tpvArr);
	if(ret) ret = tpvArr.size() > 0;
	return ret;
}

//// Buffer(XYData)��TempProfile�ɏ�������
//// �߂�l�� Temp Profile Id.
//const int DbController::onSetTmpProfile(
//	kome::core::Buffer& buffer
//) {
//	return tblPtr_->setXYBuffer(&buffer);
//}

// Temp Table���� Profile�f�[�^���擾����.
void DbController::getDataPointsFromTemp(
	const int tmpProfId,
	kome::core::DataPoints& dps
){
	kome::core::Buffer buffer;
	tblPtr_->getXYBuffer(tmpProfId, buffer);
	dps.importData(boost::bind(&kome::core::DataAccessor::read, buffer, _1, _2 ));
}

// Matrix�쐬�̏I���t�F�[�Y�𑖂点��(Commit��)
void DbController::finishMakeMatrix(
	const bool ret
) {
	if (ret) {
//		pxyPtr_->commitMe();
	}
	// ���X��DB�̃N���[�W���𑖂点��B
//	pxyPtr_->closeMe();
	tblPtr_->closeMe();
}

//// Mass++�̃X�e�[�g�����g�̏����iMass++����ƕs�v����)
//const bool DbController::setupMsppSTMT()
//{
//	return pxyPtr_->initMe();
//}

//// �s�[�N�̗�̏��ƁA����ɏ�����Value�̃f�[�^��
//// ���e�[�u���ɏ������ށB
//void DbController::writeRowLine(
//	const double mz,
//	const double rt,
//	const int charge,
//	std::vector<AB3D::TmpPeakVal*>& tpvArr
//){
//	int peakId = pxyPtr_->setPeak(matrixId_, mz, rt, charge);
//	// Insert Values.
//
//	for each(auto& tpv in tpvArr) {
//		kome::core::Buffer rstBuf;
//		tblPtr_->getXYBuffer(tpv->mxval.profId, rstBuf);
//
//		int bufId = pxyPtr_->setXYBuffer(&rstBuf);
//		AB3D::TmpMXVal& tmv = tpv->mxval;
//
//		pxyPtr_->setValues(peakId, tpv->sampleId, bufId, tmv);
//	}
//}
