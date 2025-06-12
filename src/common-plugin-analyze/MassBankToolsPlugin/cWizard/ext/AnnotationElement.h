/**
 * @file AnnotationElement
 * @brief Annotation Element Control
 *
 * @author M.Fukuda
 * @date 2013.03.08
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <string>
#include <vector>

// -- Forward declaration --
namespace kome {
	namespace objects {
		class Spectrum;
	}
}

// -----
namespace kome {
namespace massbank {
namespace wizard {

const double kPPMVal = 0.0000001;

class AnnotationElement {

	struct AnnoElem {
		double mz,rt,prc;
		double lmz,hmz,lrt,hrt,lprc,hprc;
		int nCnt; // �����J�E���^�B�Y�����邽�тɃC���N�������g�B
		std::string sType,sFormula;
		AnnoElem() {
			mz = lmz = hmz = -1.0;
			rt = prc = -1.0;
			lrt = lprc = -1.0;    // nega infinity
			hrt = hprc = DBL_MAX; // true infinity
			nCnt = 0;
		}
	};
public:
	// constructor
	AnnotationElement(){}
	// destructor
	~AnnotationElement(){}

	// String�Ƃ��ăA�m�e�[�V���������擾�A��������B
	// checkonly (T/F) T : ���͋K���̃`�F�b�N���� / F : �`�F�b�N���ʂ�����o�^�B
	const bool getSplitAnnoInfo(
		const std::string& src, 
		const bool checkonly
		);

	// mz�̔z��ART, Precursor �������Ƃ�
	// �����̃A�m�e�[�V�������ƈ�v���邩�ۂ����ׂ�B
	// checkonly (T/F) T : �Y���A�m�e�[�V�����̗L���𒲂ׂ�B/ F �{MassBankRecord�Ƃ��ďo�͂𓾂�B
	const std::string checkFitAnnoInfo(
		const std::vector<double>& mzArr,
		const double rt, 
		const double prc, 
		const bool checkonly
		);

	// Spectra����AAnnotation��K��������
	void doSpectrumFitAnnotation(const std::vector<kome::objects::Spectrum*>& specArr);

	// �g���Ȃ��A�m�e�[�V���������邩�ǂ������ׂ�B
	// ����ꍇ�͏����J���B�₢���킹��B
	const bool isNotUseAnnotationElements();

private:

	// �����J�E���^���N���A����B
	void onClearCnt();
	// �A�m�e�[�V�������(Formula���j�̕����𐶐����ĕԂ��B
	const std::string onGetAnnotationBlock(
		const double rt, 
		const AnnoElem& elm
		);

	// dstL = src - tol, dstH = src + tol �Ƃ��ĕԂ��Bppm = true���� tol��PPM�̗v�f�Ƃ��Ĉ����B
	void toTolval(
		const double src, 
		const double _tol, 
		double& dstL, 
		double& dstC, 
		double& dstH, 
		bool ppm = false
		);

	const std::string onNotUsedInfo(const AnnoElem& elm);

	std::vector<AnnoElem> annoArr_;
};

} // end of namespace
}
}
