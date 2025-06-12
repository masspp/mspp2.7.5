/*********************************************************/
/*!
 @addtoGroup AB3D
 @file AB3DPeakPosDetector.h
 @brief AB3D peak position detect Functions.
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/

// AB3DPeakDetectors....

#include "stdafx.h"
#include "AB3DPeakPosDetector.h"

// include AB3D Parts
#include "NewAB3DLabeling.h"

// include DB Controler
#include "DBTools\DbController.h"
#include "DBTools\tempContainer.h"

// include progress controler
#include "progress\progressMatrix.h"

#define AB3D_SHORT_NAME "ab3d2"

// -------------------------------------------------------*
//    Inner Structs
// -------------------------------------------------------*
// �ŏ�/�ő��臒l��ۑ�����\����
namespace AB3D{
struct tolerance {
	double lo;
	double hi;
	// �f�t�H���g�R���X�g���N�^
	tolerance(){}
	// �R���X�g���N�^�Ŕ͈͂��v�Z����
	tolerance(const double val, const double tol) 
		: lo(val - tol)
		, hi(val + tol){}
};

// Peaks Row�̒l��ۑ�����\����
// *TmpPeakVal��TempContiner.h�@�ŋL�q�B
struct stRows {
	// Row�ɕ\�������
	double mz;
	double rt;
	int charge;
	// 臒l���O�̔�r���Ɏg�p
	tolerance mztol; 
	tolerance rttol; 
	// Peaks Row�̎q�̗v�f
	std::vector<AB3D::TmpPeakVal*> vals;

	//constructor
	stRows(){}

	//constructor
	stRows(
		const double _mz, 
		const double _rt, 
		const int _cg
		) : mz(_mz)
		, rt(_rt)
		, charge(_cg){}
};
}
// -------------------------------------------------------*
//    Main Class & Functions
// -------------------------------------------------------*
// constructor
Ab3dPeakPosDetect::Ab3dPeakPosDetect(
	const double mzppm,
	const double rttol,
	std::map<std::string, std::string>& setmap,
	kome::objects::SettingParameterValues* settings
) : kome::ident::PeakPositionsDetector(true, false)
	, mzppm_(mzppm)
	, rttol_(rttol)
	, setmap_(setmap)
	, settings_(settings)
	, peaksPtr_(nullptr)
{
}

// destructor
Ab3dPeakPosDetect::~Ab3dPeakPosDetect(){
}

int Ab3dPeakPosDetect::onDetectPeakPositions(
	kome::ident::SampleGetter& samplegetter,
	kome::ident::PeakPositions& peakPos,
	kome::core::Progress& progress
){
	// DB Object
	std::unique_ptr<AB3D::DbController> dbctrl(new AB3D::DbController);
	std::string errmsg = "";

	// check DB Status
//	if (dbctrl->checkDBStatus() && (dbctrl->setupTempTables() != true)){
	if (!dbctrl->setupTempTables()){
		// TempTable�����Ȃ��̂ŏ����ł��Ȃ�
		errmsg = "Failed to access database.";
	}
	
	// AB3D�̎��s
	int fx = executeAB3D(dbctrl.get(), samplegetter, progress, errmsg);

	// Matrix�쐬�̎��s
	fx = makePeakPosData(dbctrl.get(), peakPos, progress,errmsg);

	setLastError(errmsg.c_str());
	return (HAS_RT | HAS_MZ | HAS_CHARGE);
}

kome::objects::SettingParameterValues* Ab3dPeakPosDetect::getSettings()
{
	return settings_;
}

// ------------------------------------------------------*
//   Execute AB3D at Matrix Mode
// ------------------------------------------------------*
int Ab3dPeakPosDetect::executeAB3D(
	AB3D::DbController* dbctrl,
	kome::ident::SampleGetter& samplegetter,
	kome::core::Progress& progress,
	std::string& errmsg
){
	int ret = 0;//(����)

	//AB3D object
	NewAB3DLabeling ab3d(setmap_);

	// Progress ���X�^�[�g
	MatrixProg prog(&progress, samplegetter.getNumberOfSamples());

	kome::objects::Sample* sample = nullptr;
	kome::operation::Alignment* align = nullptr;

	while((sample = samplegetter.next()) != NULL && !progress.isStopped()) {

		const bool isSupportedAPI = sample->isSupportedAPIPeaks();

		kome::operation::Alignment* align = samplegetter.getAlignment();
		kome::objects::DataSet dataSet;
		kome::objects::DataGroupNode* dgn = sample->getRootDataGroupNode();
		// DataSet���s���ȏꍇ���`�F�b�N�ł���Ȃ炱���ōs���B
		if(dgn != nullptr) {
			dgn->getDataSet(&dataSet);
		}
		
		unsigned int cnum = dgn->getNumberOfSpectra();
		for(unsigned int i = 0; i < cnum; i++) {
			kome::objects::Spectrum* spec = dgn->getSpectrum(i);
			double rt = spec->getRt();
			if(align != NULL) {
				double convertedRt = align->convertRt(rt,spec->getMinX());
				spec->setRt(convertedRt);
			}
		}

		// Matrix�ł�SampleId���L�^����
		dbctrl->setSampleId(samplegetter.getMatrixSampleId());

		// Progress���Z�b�g����
		kome::core::Progress* subProgress = prog.getSubProgress();
		ab3d.bindProgress(subProgress);

		// AB3D�����s�B
		ab3d.doAB3D(dataSet, dbctrl);
	}
	if(progress.isStopped()) {
		errmsg = "Progress stopped.";
		ret = 1;
	}

	prog.closeMe();

	return ret;
}

// Peak PositionData �Ɂ@AB3D�̉�����͂���
int Ab3dPeakPosDetect::makePeakPosData(
	AB3D::DbController* dbctrl,
	kome::ident::PeakPositions& peakPos,
	kome::core::Progress& progress,
	std::string& errmsg
){
	std::vector<AB3D::TmpPeakVal*> tmpPeaks;
	std::vector<AB3D::TmpPeakVal*> errPeaks;
	
	// Temp Table���� Charge > -2 ��Peak�����AValue�̍~���Ŏ擾����B
	// TmpPeakVal�͓�����[New]����Ė߂�B
	if(dbctrl->getMatchDataFromTemp(tmpPeaks)) {

		std::vector<AB3D::stRows> rowArr; 
		// ������U�蕪����
		for each(auto p in tmpPeaks) {
			if(!parsePeaksToMatrix(p, rowArr)) {
				errPeaks.push_back(p);
			}
		}
		peaksPtr_.reset(new kome::objects::Peaks);

		for each(auto sr in rowArr) {
			int peakIdx = peakPos.addPeakPosition(sr.rt, sr.mz, sr.charge);

			for each(auto p in sr.vals) {
				AB3D::TmpMXVal* mx = &p->mxval;
				// TempTable����XIC��DATAPOINT���擾
				kome::core::DataPoints dps;
				dbctrl->getDataPointsFromTemp(mx->profId, dps);
				// Peak���̐���
				kome::objects::PeakElement* peak = peaksPtr_->createPeak(mx->core.x,mx->core.y);
				peak->setLeft(mx->left.x, mx->left.y);
				peak->setRight(mx->right.x, mx->right.y);

				// Test Code
				kome::core::XYData* xydata = &dps;
				long dpsnum = dps.getLength();
				xydata->getMaxX();
				peakPos.setPeakValueInfo(peakIdx, p->sampleId, mx->value, *xydata ,peak,true);
				// ��̎��_��DB@Profile�ɏ������܂��̂� dps�̎����͂����Ős���ėǂ�
			}
		}
	}
	// New���ꂽ�I�u�W�F�N�g��Delete����B
	for each(auto tpv in tmpPeaks){delete tpv;}
	return true;
}

// AB3D�̌��ʂ� Row - Peak Value�̍\���ɂȂ�悤�ɐU�蕪����
const bool Ab3dPeakPosDetect::parsePeaksToMatrix(
	AB3D::TmpPeakVal* pt,
	std::vector<AB3D::stRows>& rowArr
){
	std::vector<AB3D::stRows>::iterator it = rowArr.begin();
	std::vector<AB3D::stRows>::iterator eit = rowArr.end();
	for(; it != eit; ++it) {
		// �o�^�ς�Peak��臒l�𔻒�
		if(isInRowTolerance(pt, *it)) {
			//臒l���ɗv�f������
			if(isSameIdInRow(pt->sampleId, *it)) {
				// ���ɓ���SampleID������:�u�o�^���Ȃ��v
				return false;
			} else {
				// �܂��q�ɖ���SampleID�B�q�Ƃ��āu�o�^�v
				it->vals.push_back(pt);
				return true;
			}
		}
	}
	//�o�^�ς݂�ROW�ƈ�v�����B�V�K�v�f�Ƃ��ēo�^����B
	rowArr.push_back(createStRow(pt));
	return true;
}

// �Ώۂ� row��臒l�Ɠ������ۂ���Ԃ�
const bool Ab3dPeakPosDetect::isInRowTolerance(
	AB3D::TmpPeakVal* p,
	const AB3D::stRows& row
){
	if (p->charge == row.charge) {
		// m/z��臒l��
		if (row.mztol.lo <= p->mz && p->mz <= row.mztol.hi) {
			// rt��臒l��
			if (row.rttol.lo <= p->rt && p->rt <= row.rttol.hi) {
				return true;
			}
		}
	}
	return false;

	//// Charge��
	//if (p->charge != row.charge) {
	//	return false;
	//}
	//// m/z��臒l����O��邩�𔻒�
	//if (p->mz < row.mztol.lo || row.mztol.hi < p->mz) {
	//	return false;
	//}
	//// rt��臒l����O��邩�𔻒�
	//if (p->rt < row.rttol.lo || row.rttol.hi < p->rt) {
	//	return false;
	//}
	//// 臒l���Ŋ���Charge�������B�̂Ɉ�v�B
	//return true;
}

//�@���ɓ���SampleID������� True
const bool Ab3dPeakPosDetect::isSameIdInRow(
	const int tgtId, 
	const AB3D::stRows& tgtRow
){
	for each(auto& t in tgtRow.vals) {
		if(t->sampleId == tgtId) return true;
	}
	return false;
}

// stRow�𐶐�����
AB3D::stRows Ab3dPeakPosDetect::createStRow(
	AB3D::TmpPeakVal* p
){
	AB3D::stRows rst(p->mz, p->rt, p->charge);
	double mztol = p->mz * 0.000001 * mzppm_;
	// 臒l����p�͈̔͂��v�Z���ĕێ�����B
	rst.mztol = AB3D::tolerance(p->mz, mztol);
	rst.rttol = AB3D::tolerance(p->rt, rttol_);
	rst.vals.push_back(p);
	return rst;
}
