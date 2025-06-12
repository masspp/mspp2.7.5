/**
 * @file Records Elements
 * @brief MassBank Record�ŏo�͂����v�f���ꌳ�Ǘ�
 *
 * @author M.Fukuda
 * @date 2013.03.12
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "RecordElements.h"
#include "objectsName.h"

// suport functions
#include "calcFunc\StrFunc.h"
#include "calcFunc\getNowDate.h"

#include "ext\XPeakDetector.h"
#include "ext\AnnotationElement.h"

#include "rel_stdafx.h"
#include "IdentMgrAcc.h"

const std::string kBlank("");
const std::string kNBA("##");
const std::string kNoneVanish("NOTHING#SO#VANISH#THIS");
const std::string errorConvert("ERRORCONVERTED");

const std::string kNA("N/A");
const std::string kCH("CH");
const std::string kSP("SP");
const std::string kAC("AC");
const std::string kMS("MS");
const std::string kPK("PK");

const std::string kcT("$");
const std::string kcCS(": ");
const std::string kcKS("; ");
const std::string kcB(" ");
const std::string kcN("\n");

const std::string kSUBSPEC("MASS_SPECTROMETRY");
const std::string kSUBCHORM("CHROMATOGRAHPY");
const std::string kSUBFCION("FORCUSED_ION");
const std::string kSUBDTPRC("DATA_PROCESSING");

namespace kome {
	namespace massbank {
		namespace wizard {
			
			// Sub Tag�̏��@EX) MS_TYPE MS2
			struct subTags {
				std::string tag; // subtag Name EX) MS_TYPE
				std::string val; // Value       EX) MS2
				bool isHold;     // if checked, not Alphabetiacl sort

				// constructor
				subTags(
					const std::string& _tag,
					const std::string& _val, 
					bool ishold = false
					) : tag(_tag)
					, val(_val)
					, isHold(ishold){}
			};

			// Tag�̏�� EX) CH$FORMULA
			struct tagInfo {
				const std::string topTag;  // TopTag  EX) CH$
				const std::string tagName; // TagName EX) FORMULA
				const bool multi;          // if true, value is multi.
				std::string value;         // Value   EX) H2O

				// constructor
				tagInfo(
					const std::string& _top,
					const std::string& _name,
					const bool _multi = false
					) : topTag(_top)
					, tagName(_name)
					, multi(_multi)
					, value(){}
			};

			// subtaginfo (Subtag��i����TAG�̐擪����)
			// EX) AC$MASS_SPECTROMETORY
			struct subtagInfo {
				const std::string topTag;        // Toptag  EX) AC$
				const std::string subTopName;    // subtag  EX) MASS_SPECTROMETORY
				std::vector<subTags> subtagArr;  // subtag's Elements.

				// constructor
				subtagInfo(
					const std::string& _top,
					const std::string& _name
					) : topTag(_top)
					, subTopName(_name)
					, subtagArr(){}
			};
		}
	}
}

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
RecordElements::RecordElements() 
	: pkPtr_(new XPeakDetector)
	, anoPtr_(new AnnotationElement)
	, tiMSType_(nullptr)
	, tiPolarity_(nullptr)
{}

// destructor
RecordElements::~RecordElements()
{
	if (tiMSType_ != nullptr) {delete tiMSType_;}
	if (tiPolarity_!= nullptr) {delete tiPolarity_;}
}

// call init.
// �t�H���[���o���Ȃ��Ȃ�̂ŃR���X�g���N�^���ŌĂ΂Ȃ����ƁB
void RecordElements::Init() 
{
	onInit();
}

const std::string RecordElements::createAccession(
	const std::string& prefix, 
	int basenum
){
	return this->onCreationAccession(prefix, basenum);
}

// Batch�쐬���ׂ̈̃A�_�v�^
void onValuesForBatch(
	std::vector<std::string>& names,
	std::vector<std::string>& values,
	const std::string& hdd,
	const std::string& val
){
	std::string name = ext::trancetoMassBankTag(hdd + val);
	names.push_back(name);
	values.push_back(val);
}


// Batch Search����AppendedValue�Ƃ��Ēl��n��

void RecordElements::getAppendedValuesForBatch(
	std::vector<std::string>& names,
	std::vector<std::string>& values
){
	std::string hdd("");
	hdd = kBlank;
	onValuesForBatch(names, values, hdd, kDate);
	onValuesForBatch(names, values, hdd, kAuthors);
	onValuesForBatch(names, values, hdd, kPublication);
	onValuesForBatch(names, values, hdd, k_list_Comment);
	
	hdd = kCH + kcT;
	onValuesForBatch(names, values, hdd, kName);
	onValuesForBatch(names, values, hdd, kCompoundClass);
	onValuesForBatch(names, values, hdd, kLink);
	onValuesForBatch(names, values, hdd, kFormula);
	onValuesForBatch(names, values, hdd, kExactMass);
	onValuesForBatch(names, values, hdd, kSMILES);
	onValuesForBatch(names, values, hdd, kIupac);

	// $SP -- Information of Biological Sample
	hdd = kSP + kcT;
	onValuesForBatch(names, values, hdd, kScientificName);
	//����������O
	std::string splink = "sp" + kLink;
	names.push_back(ext::trancetoMassBankTag(hdd + kLink));
	values.push_back(splink);

	onValuesForBatch(names, values, hdd, k_list_Sample);

	// $AC -- Analyical Methods and Conditions

	std::vector<std::string> okarr, ngarr;

	okarr.clear();
	onSetUpACSpecSubTags(okarr, ngarr);
	hdd = kAC + kcT + kSUBSPEC + kcCS;
	
	onValuesForBatch(names, values, hdd, kMSStage);
	onValuesForBatch(names, values, hdd, kMSType);

	for each(auto& okstr in okarr) {
		if (okstr.compare(kCanInputSelf) != 0) {
			onValuesForBatch(names, values, hdd, okstr);
		}
	}

	okarr.clear();
	onSetUpACChromSubTags(okarr, ngarr);
	hdd = kAC + kcT + kSUBCHORM + kcCS;
	for each(auto& okstr in okarr) {
		if (okstr.compare(kCanInputSelf) != 0) {
			onValuesForBatch(names, values, hdd, okstr);
		}
	}

	okarr.clear();
	onSetUpMSIonSubTags(okarr, ngarr);
	hdd = kMS + kcT + kSUBFCION + kcCS;
	for each(auto& okstr in okarr) {
		if (okstr.compare(kCanInputSelf) != 0) {
			onValuesForBatch(names, values, hdd, okstr);
		}
	}
}

// init (Wizard���ŌĂ΂ꂽ���ɁA
// MassBankRecord�Ƃ��ďo�͂���ׂ̃I�u�W�F�N�g�̏�����
void RecordElements::onInit() 
{
	// Record Specific Information
	topTags_.push_back(kBlank);
	topTags_.push_back(kCH);

	addTagInfo(kBlank,kDate);
	addTagInfo(kBlank,kAuthors);
	addTagInfo(kBlank,kLicense);
	addTagInfo(kBlank,kCopyright);
	addTagInfo(kBlank,kPublication);
	addTagInfo(kBlank,k_list_Comment, true);

	// $CH -- Information of Chemical Compound Analyzed
	addTagInfo(kCH,kName, true);
	addTagInfo(kCH,kCompoundClass);
	addTagInfo(kCH,kLink,true);
	addTagInfo(kCH,kFormula);
	addTagInfo(kCH,kExactMass);
	addTagInfo(kCH,kSMILES);
	addTagInfo(kCH,kIupac);

	// $SP -- Information of Biological Sample
	addTagInfo(kSP,kScientificName);
	addTagInfo(kSP,kLink);
	addTagInfo(kSP,k_list_Sample, true);

	// $AC -- Analyical Methods and Conditions
	addTagInfo(kAC,kInstrument);
	addTagInfo(kAC,kInstrumentType);
	
	// Extends.
	tiMSType_ = new tagInfo(kAC, kSUBSPEC);
	tiPolarity_ = new tagInfo(kAC,kSUBSPEC);

	addSubTagInfo(kAC,kSUBSPEC);

	addSubTagInfo(kAC,kSUBCHORM);
	// $MS -- Description mass spectral data
	addSubTagInfo(kMS,kSUBFCION);

	// $PK -- Peak Information
	addSubTagInfo(kMS,kSUBDTPRC);

	// SetupInner Elements
	onSetUpACSpecSubTags(subSpecArr_  , subSpecNGArr_);
	onSetUpACChromSubTags(subChromArr_, subChromNGArr_);
	onSetUpMSIonSubTags(subIonArr_, subIonNGArr_);
	onSetUpMSIonElems();
}

// create accession
void RecordElements::setAccessions(const std::string& prefix, int basenum)
{
	prefix_ = ext::tranceLowerToUpper(prefix);
	basenum_ = basenum;
} 

// processing�̒l���Z�b�g����
void RecordElements::setProcStream(const std::string& procstr)
{
	pkPtr_->setProcStream(procstr);
}

const bool RecordElements::setAveSpectra(
	const std::vector<std::vector<kome::objects::Spectrum*>>& aveSpecArr
){
	for each(auto p in avespecArr_) {
		delete p;
	}
	avespecArr_.clear();
	avespecElmArr_.clear();

	avespecElmArr_ = aveSpecArr;

	if(!avespecElmArr_.empty()){	
		for each(auto& vspec in avespecElmArr_) {
			kome::objects::Spectrum* spec = vspec.at(0);
			kome::objects::AveragedSpectrum* av = 
				new kome::objects::AveragedSpectrum(spec->getGroup());
			for each(auto p in vspec) {
				av->addSpectrum(p);
			}
			avespecArr_.push_back(av);
		}
	}
	return (!avespecArr_.empty());
}



void RecordElements::setOPT(std::vector<kome::objects::XYDataOperation*>& opts)
{
	pkPtr_->setOPTs(opts);
}


// Peak Detector���Z�b�g����B
// XPeakDetecoter�֌o�R����B
// Pane10�̃s�[�N�s�b�L���O�̏����\�����Ă����B
void RecordElements::setPeakDetector(
	const std::string& detectStr,
	kome::objects::SettingParameterValues* param
){
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* pkFunc = plgMgr.getFunctionItem("SPEC_PEAK_PICKING", detectStr.c_str());
	// Set Function
	pkPtr_->setPeakInfo(pkFunc, param);

	std::string xval = detectStr;

	if (param != nullptr) {
		unsigned int snum = param->getNumberOfParameters();
		if (snum > 0) {
			xval += "; ";
		}
		std::string tmpPval, tmpNval;
		for (unsigned int idx = 0; idx < snum; idx++) {
			tmpPval = param->getParameterName(idx);
			tmpNval = param->getParameterValue(tmpPval.c_str());
			if(idx > 0) {
				xval += ", ";
			}
			xval += tmpPval + "=" + tmpNval;
		}
	}
	peakDetectStream_ = xval;
}

//�@�t�B���^�̏����Z�b�g����
void RecordElements::setPeakFilter(const char* peakFilter)
{
	if (peakFilter != nullptr) {
		pkPtr_->setFilterInto(peakFilter);
	}
}

// set Record Info
void RecordElements::setRecordInfo(
	const std::string& _title,
	const std::string& _date,
	const std::string& _authors,
	const std::string& _license,
	const std::string& _copyright,
	const std::string& _publications
){
	Titles_ = _title;
	setTagInfoVal(kDate,        _date,         kNoneVanish);
	setTagInfoVal(kAuthors,     _authors,      kNoneVanish);
	setTagInfoVal(kLicense,     _license,      kBlank);
	setTagInfoVal(kCopyright,   _copyright,    kNoneVanish);
	setTagInfoVal(kPublication, _publications, kNoneVanish);
}

// set Record Info Comment
void RecordElements::setComments(const std::string& _comments) 
{
	setTagInfoVal(k_list_Comment, _comments, kNoneVanish);
}

//*** AC Tag *****************************************************

// set AC Name
void RecordElements::setCHName(const std::string& _name) 
{
	setTagInfoVal(kName, _name, kNA);
}

// set Category with Role
const bool RecordElements::setCategory(
	const std::string& _product, 
	const std::string& _cname
){
	std::string setval;
	if(_cname.empty()) {
		if (_product.compare(kUnknown) == 0) {
			setval = kNA;
		} else {
			return false;
		}
	} else {
		setval = _product + kcKS + _cname;
	}
	setTagInfoVal(kCompoundClass, setval, kNA);
	return true;
}

void RecordElements::setLinks(const std::string& _val) 
{
	setTagInfoVal(kLink, _val, kNoneVanish);
}

// set ChemInfo
void RecordElements::setChemcalStructures(
	const std::string& _formula, 
	const std::string& _exactmass, 
	const std::string& _similes, 
	const std::string& _inchi
){
	setTagInfoVal(kFormula,   _formula,   kNA);
	setTagInfoVal(kExactMass, _exactmass, kNA);
	setTagInfoVal(kSMILES,    _similes,   kNA);
	setTagInfoVal(kIupac,     _inchi,     kNA);
}

//*** SP Tag *****************************************************
void RecordElements::setSPStatus(
	const std::string& _scentName,
	const std::string& spDb, 
	const std::string& spAcc,
	const std::string& sample
) {
	setTagInfoVal(kScientificName, _scentName, kNoneVanish);

	std::string spLink = spDb;
	if(!spDb.empty()){
		spLink += kcB + spAcc;
	}
	setTagInfoVal(kSP, kLink, spLink, kNoneVanish);
	setTagInfoVal(k_list_Sample, sample, kNoneVanish);
}

//*** AC Tag *****************************************************
const bool RecordElements::setACInstruments(
	const std::string& _vender, 
	const std::string& _instName,
	const std::string& _instType
) {
	if (_vender.empty() || _instName.empty() || _instType.empty())
	{ 
		return false;
	}
	std::string inst = _instName + ", " + _vender;
	setTagInfoVal(kInstrument, inst,kBlank);
	setTagInfoVal(kInstrumentType, _instType,kBlank);
	return true;
}

// MS Type, Ion Mode(Polarity)�̃Z�b�g
void RecordElements::setMSStage_Polarity(
	const std::string& sval,
	const std::string& pval
) {
	std::string rstM, rstP;
	if(sval == kNone) {
		rstM = kNA;
	} else if(sval == kAuto) {
		rstM = "MS%Stage%";
	} else {
		rstM = sval;
	}

	tiMSType_->value = rstM;

	if(pval == kNone) {
		rstP = kNA;
	} else if(pval == kAuto) {
		rstP = "%Polarity%";
	} else {
		rstP = pval;
	}
	tiPolarity_->value = rstP;
}

// Sub Tag : MASS SPEC ���Z�b�g
void RecordElements::setSubSpec(const std::string& massSubtags) 
{
	onSplitSetSubtags(kAC,kSUBSPEC,massSubtags);
}

// Sub Tag : MASS Chrom ���Z�b�g
void RecordElements::setSubChrom(const std::string& valStr) 
{
	onSplitSetSubtags(kAC, kSUBCHORM, valStr);
}

//*** MS Tag ****************************************************
// Ion Type ���Z�b�g
void RecordElements::setIonType(const std::string& val) 
{
	std::string rstStr = val;
	if (rstStr.empty() || rstStr == kNone) {
		rstStr = kNoneVanish;
	}
	addSubTagElem(kMS, kSUBFCION, kIonType, rstStr, false);
}

//Precursor Type���Z�b�g
void RecordElements::setPrcType(const std::string& val) 
{
	std::string rstStr = val;
	if (rstStr.empty() || rstStr == kNone){
		rstStr = kNoneVanish;
	}
	addSubTagElem(kMS, kSUBFCION, kPrecursorType, rstStr, false);
}

// MS ION�@�̃T�u�^�O���Z�b�g
void RecordElements::setSubFcIon(const std::string& valStr) 
{
	onSplitSetSubtags(kMS, kSUBFCION, valStr);
}


//*** PK Tag ****************************************************
// Data Process���Z�b�g
void RecordElements::setDataProc(const std::string& vals) 
{
	STGInItr it = onSearchSubTagInfoVal(kMS, kSUBDTPRC);
	// �������Ƃ�Ȃ��Ȃ�̂ň�U�N���A�B�C���������B
	it->subtagArr.clear();
	std::vector<std::string> rstArr;
	onSplitParams(vals,rstArr);
	std::for_each(rstArr.begin(),rstArr.end(),[&](const std::string& tstr) { 
		addSubTagElem(kMS, kSUBDTPRC, tstr, kBlank, false);
	});
}

// Soft �Ɓ@Peak Detector�̏����Z�b�g
void RecordElements::setSoft_Peaks(
	const std::string& soft,
	const std::string& peakStr
){
	if (!peakStr.empty()) {
		addSubTagElem(kMS, kSUBDTPRC, kFindPeak, peakStr, false);
	}
	if (!soft.empty()) {
		addSubTagElem(kMS, kSUBDTPRC, kWhole, soft, false);
	}
}

// �A�m�e�[�V���������Z�b�g
const bool RecordElements::setAnnoDatas(const std::string& val)
{
	return anoPtr_->getSplitAnnoInfo(val,false);
}


// �ŏ��ɐ��������A�Z�b�V������Ԃ�
const std::string RecordElements::getFirstAccession() 
{
	std::string rstStr("");
	if (!specArr_.empty()) {
		rstStr = onMakeAccession(specArr_[0]);
	}
	return rstStr;
}


void RecordElements::AttachAppendValues(
	const std::string& title,
	const std::string& license,
	const std::string& vendor,
	const std::string& instname,
	const std::string& insttype,
	const std::string& apvalue
) {
	// �f�t�H���g�̒l��ݒ肷��
	std::string nowdate = ext::getNowDate();

	this->setRecordInfo("", nowdate, "", license, "", "");
	this->setCHName("");
	this->setCategory(kUnknown,"");
	this->setLinks("");
	this->setChemcalStructures("","","","");
	this->setACInstruments(vendor, instname, insttype);

	// Auto �ŏ����l�Ƃ��Đݒ肷��
	this->setMSStage_Polarity(kAuto, kAuto);

	this->addSubTagElem(kAC, kSUBSPEC, k_AC_Spec_ColEnergy, kDefCollesionEnergy, false);
	this->addSubTagElem(kAC, kSUBCHORM, k_AC_CH_RetentionTime, kDefRetenstionTime, false);
	this->addSubTagElem(kMS, kSUBFCION, k_MS_BasePeak, kDefBPM, false);
	this->addSubTagElem(kMS, kSUBFCION, k_MS_PrecMZ, kDefPrecursor, false);

	// Append Value�𕪊�
	std::vector<std::pair<std::string, std::string>> apvals;
	this->onSplitParamsToPair(apvalue, apvals);

	bool ret(false);
	std::string splink = "sp" + kLink;
	for each(auto& val in apvals) {
		if (splink == val.first) {
			this->setTagInfoVal(kSP,kLink,val.second, kNoneVanish);
			continue;
		}
		ret = this->setTagInfoVal(val.first, val.second, kNoneVanish);
		if (!ret) {
			if(std::find(subSpecArr_.begin(), subSpecArr_.end(), val.first) != subSpecArr_.end()) {
				this->addSubTagElem(kAC, kSUBSPEC, val.first, val.second,false);
			} else {
				if(std::find(subChromArr_.begin(), subChromArr_.end(), val.first) != subChromArr_.end()) {
					this->addSubTagElem(kAC, kSUBCHORM, val.first, val.second,false);
				} else {
					if(std::find(subIonArr_.begin(), subIonArr_.end(), val.first) != subIonArr_.end()) {
						this->addSubTagElem(kMS, kSUBFCION, val.first, val.second, false);				
					} else {
						//// ... unknown ????
//						LOG_WARN(FMT("Error??").c_str());
					}
				}
			}
		}
	}
}

const bool RecordElements::getMassBankPreText(
	kome::objects::Spectrum* srcspec,
	kome::objects::Spectrum*& dstspec,
	std::string& refRecordStr
) {
	std::vector<kome::objects::Spectrum*> src,dst;
	src.push_back(srcspec);
	pkPtr_->doPeakPickAndFilter(src, dst, procFg_);

	if(!dst.empty()) {
		onSortABCSubTags();
		std::string myAcc = onMakeAccession(dst[0]);
		refRecordStr = onCreateSpectrumMassBankRecord(myAcc, dst[0]);
		dstspec = dst[0];
		return true;
	}
	return false;
}

// ******* INNER
const std::string RecordElements::getMassBankPreTexts(kome::objects::Spectrum* spec) 
{
	std::string rstStr("");
	// temp
	std::vector<kome::objects::Spectrum*> src,dst;
	src.push_back(spec);
	pkPtr_->doPeakPickAndFilter(src, dst, procFg_);

	if(!dst.empty()) {
		onSortABCSubTags();
		std::string myAcc = onMakeAccession(dst[0]);
		rstStr = onCreateSpectrumMassBankRecord(myAcc, dst[0]);
	}
	return rstStr;
}

//�@�Y��Spectrem���烌�R�[�h�����쐬����
const std::string RecordElements::onCreateSpectrumMassBankRecord(
	const std::string& myAccession, 
	kome::objects::Spectrum* spec
){
	// Accession���쐬
	std::string rstStr = onGetMassBankLine(kBlank, kAccession, myAccession);
	// Title���쐬
	rstStr += onGetMassBankLine(kBlank, kRecordTitle, onMakeTitle(spec));

	// CH-SP �^�O�̓��e���o�͂���
	std::for_each(tagInfoArr_.begin(), tagInfoArr_.end(), [&](const tagInfo& taginfo) {
		rstStr += onGetMassBankLine(taginfo, spec);
	});
	// AC$ MSType, IonMode���o�͂���
	rstStr += onGetExtraMsType(spec);
	rstStr += onGetExtraIonMode(spec);

	// �c��̃T�u�^�O���o�͂���
	std::for_each(subtagInfoArr_.begin(), subtagInfoArr_.end(), [&](const subtagInfo& st) {
		rstStr += onGetMassBankLine(st, spec);
	});
	// �A�m�e�[�V�����A�s�[�N�̏����o�͂���
	rstStr += onGetPeak_MassBank_Line(spec);
	return rstStr;
}

const bool RecordElements::makeMassBankRecords(
	std::string& errmsg
) {
	// �f�B���N�g��IO�̊m�F�E�E��Mass++��Folder�̐ݒ��M����B

	// �s�[�N�s�b�L���O�ƃt�B���^�����O���s���B
	std::vector<kome::objects::Spectrum*> dstSpectra;

	bool ret = pkPtr_->doPeakPickAndFilter(this->specArr_, dstSpectra, procFg_);

	if(!this->avespecArr_.empty()) {
		std::vector<kome::objects::Spectrum*> ave1Spectra,ave2Spectra;

		for each(auto p in this->avespecArr_) {
			ave1Spectra.push_back(p);
		}
		bool ret = pkPtr_->doPeakPickAndFilter(ave1Spectra, ave2Spectra, procFg_);

		for each(auto p in ave2Spectra) {
			dstSpectra.push_back(p);
		}
	}

	//dstSpectra�ɂ���ߒ��Ō���������Ώ�L�֐����Ŕ���B

	//���g�p�A�m�e�[�V�������������ǂ����`�F�b�N����B
	anoPtr_->doSpectrumFitAnnotation(dstSpectra);

	//����ꍇ�́@�₢���킹�������߂��B
	if (!anoPtr_->isNotUseAnnotationElements()) return false;

	onSortABCSubTags();
	std::vector<kome::objects::Spectrum*>::iterator it  = dstSpectra.begin();
	std::vector<kome::objects::Spectrum*>::iterator eit = dstSpectra.end();

	for(; it != eit; ++it) {
		std::string myAcc = onMakeAccession(*it);
		std::string fileName = getpath(this->folderpath_.c_str(), myAcc.c_str());
		fileName += ".txt";
		std::string rstStr = makeMassBankRecord(*it, myAcc);
		if(!writeMassBankRecordtoText(rstStr, fileName, errmsg)) {
			return false;
		}
	}
	int lastnum = basenum_ + specArr_.size() + avespecArr_.size();
	Ident::setPrefixAndNum(prefix_, lastnum);
	return true;
}

const std::string RecordElements::makeMassBankRecord(
	kome::objects::Spectrum* spec,
	std::string& myAccession
){
	return onCreateSpectrumMassBankRecord(myAccession, spec);
}

const bool RecordElements::writeMassBankRecordtoText(
	const std::string& recordDataStr,
	const std::string& writeFilePath,
	std::string& errmsg
){
	FILE* fp = fileopen(writeFilePath.c_str(), "w");
	if (fp == NULL) {
		// TargetFile Write Error so Break.
		errmsg = "An error occurred creating a MassBank spectrum record\n";
		errmsg +=	FMT("(%s). Select a different folder.", writeFilePath.c_str());
		return false;
	} else {
		try {
			fprintf(fp, "%s\n//", recordDataStr.c_str()); // File Write And Add End Line //
		} catch( ... ) {
			errmsg = "An error occurred creating a MassBank spectrum record\n";
			errmsg += "Catch fprintf or system error.";
			return false;
			// write Error?
		}
		fflush(fp);
		fclose(fp);
	}
	return true;
}

//--------------------------------------------------------------------------
// �}�X�o���NText�ւ̏o�͕���
//--------------------------------------------------------------------------

// MassBank���[���Ł@Text��r�o����
const std::string RecordElements::onGetMassBankLine(
	const tagInfo& taginfo,
	kome::objects::Spectrum* spec
//	std::vector<std::string>& vals
) {
	std::vector<std::string> vals;
	onSplitVal(taginfo, vals);
	
	std::string rstHead, rstStr;

	// Header���� Ex) Ac$�̕������쐬
	if (! taginfo.topTag.empty()) {
		rstHead = taginfo.topTag + kcT;
	}
	
	// Header���� Ex) Ac$INSTRUMENT: �̕������쐬
	rstHead += ext::trancetoMassBankTag(taginfo.tagName);
	rstHead += kcCS;

	rstStr.clear();
	std::for_each(vals.begin(),vals.end(),[&](const std::string& s) {
		if (s != kNoneVanish) {
			// TAG + " " + Value + ���s ���o��
			rstStr += rstHead + onTryGetSpecReplaceProp(spec, s) + kcN;
		}
	});
	return rstStr;
}

// �����s���ASpec�u�����Ȃ��P���ȃp�^�[���B
const std::string RecordElements::onGetMassBankLine(
	const std::string& _top, 
	const std::string& _tag, 
	const std::string& val
){
	std::string trTag = ext::trancetoMassBankTag(_tag);
	if (_top.empty()) {
		// EX) LICENSE: CC BY
		return trTag + kcCS + val + kcN;
	} else {
		// EX) CH$NAME: hoge
		return _top + kcT + _tag + kcCS + val + kcN;
	}
}

// SubTag
const std::string RecordElements::onGetMassBankLine(
	const subtagInfo& subtaginfo, 
	kome::objects::Spectrum* spec
){
	// Header���� Ex) MS$FORCUSED_ION: �̕������쐬
	std::string rstHead = subtaginfo.topTag + kcT;
	rstHead += ext::trancetoMassBankTag(subtaginfo.subTopName) + kcCS;
	std::string rstStr;

	// �㔼�������쐬
	std::for_each(subtaginfo.subtagArr.begin(),subtaginfo.subtagArr.end(),[&](const subTags& st) {
		if (st.val != kNoneVanish) {
			// kNonVanish�͋󗓎��͏o�͂��Ȃ��̑�֕�����
			// SubTag Ex) ion type ���@ION_TYPE �ɂ���
			rstStr += rstHead + ext::trancetoMassBankTag(st.tag);
			// %RT% �̂悤�ȃv���p�e�B�l��u������B
			rstStr += kcB + onTryGetSpecReplaceProp(spec, st.val) + kcN;
		}
	});
	return rstStr;
}

// Peak�ƃA�m�e�[�V�����̏��� Mass Bank �`���ɂ��ĕԂ��B
const std::string RecordElements::onGetPeak_MassBank_Line(
	kome::objects::Spectrum* spec
){
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.getPeaks(spec);
	if (peaks == nullptr) return kBlank;

	std::string pkStr, anoStr;
	std::vector<double> mzArr; //Peak��Mz�̃��X�g
	pkStr = onGetPeakLines(peaks, mzArr);

	if (!mzArr.empty()) {
		anoStr = onGetAnnotations(mzArr,spec->getRt(), spec->getPrecursor());
	}
	std::string rstStr = anoStr + pkStr;
	return rstStr;
}

// Peak�̏��� Mass Bank�@�`���ɂ��ĕԂ��B(Inner)
const std::string RecordElements::onGetPeakLines(
	kome::objects::Peaks* peaks, 
	std::vector<double>& mzArr
){
	std::string rstStr;

	unsigned int pnum = peaks->getLength();
	rstStr = onGetMassBankLine(kPK, kNumPeak, ext::toStr(pnum));

	// �ړ���������B
	rstStr += onGetMassBankLine(kPK, kPeak, "m/z int. rel.int.");

	const double maxInt = peaks->getMaxY();
	const double intRatio = 999.0 / maxInt;
	double tgtMz, tgtInt;
	int relInt;
	mzArr.reserve(pnum);
	for( unsigned int i = 0; i < pnum; i++ ) {
		tgtMz = peaks->getX(i);
		tgtInt = peaks->getY(i);
		relInt = floor(tgtInt * intRatio + 0.5);
		if (relInt < 1) { relInt = 1;} // �Œ�l�ɂP��ۏ؂���B
		else if(relInt > 999) { relInt = 999;}//�ő�l��999��ۏ؂���B

		// Peak �̏o�͂��쐬����
		rstStr += FMT("  %f %f %d\n", tgtMz, tgtInt, relInt);
		mzArr.push_back(tgtMz);
	}
	return rstStr;
}

// �A�m�e�[�V�����̏���MassBank�`���ɂ��ĕԂ��B(Inner)
const std::string RecordElements::onGetAnnotations(
	std::vector<double>& mzArr, 
	const double rt, 
	const double prec
){
	// mzArr�̔z��ART,Precursor�Ɉ�v����Annotation��Ԃ�
	std::string bufStr = anoPtr_->checkFitAnnoInfo(mzArr, rt, prec, false);
	std::string rstStr("");
	if (!bufStr.empty()) {
		rstStr = onGetMassBankLine(kPK, kAnnotation, "m/z num (type mass error(ppm) formula)");
		rstStr += bufStr;
	}
	return rstStr;
}

// ----------------------------------------------------
//  <<inner>>
//  setting param Val ���𕪊�����֐��Q
//
// ------------------------------------------------------

// params value Split.
void RecordElements::onSplitVal(
	const tagInfo& taginfo, 
	std::vector<std::string>& vals
) {
	vals.clear();
	if(taginfo.multi) {
		onSplitParams(taginfo.value, vals);
	} else {
		vals.push_back(taginfo.value);
	}
}

// onSplit List Value
void RecordElements::onSplitParams(
	const std::string& src, 
	std::vector<std::string>& vals
) {
	std::vector<std::pair<std::string, std::string>> rstArr;
	bool doSort = this->onSplitParamsToPair(src, rstArr);
	if (rstArr.empty()) return;

	for each (auto& tmp in rstArr) {
		if(doSort) {
			vals.push_back(tmp.first + kcB + tmp.second);
		} else {
			vals.push_back(tmp.first);
		}
	}

	if(doSort) {
		std::sort(vals.begin(),vals.end());
	}
}

// Split And Add Sub Tag
void RecordElements::onSplitSetSubtags(
	const std::string& _top,
	const std::string& _sub,
	const std::string& src
) {
	std::vector<std::pair<std::string, std::string>> rstArr;
	this->onSplitParamsToPair(src, rstArr);
	if (rstArr.empty()) return;
	for each(auto& val in rstArr) {
		addSubTagElem(_top,_sub, val.first, val.second, false);
	}
}

// Patameter Setting�@����l���擾���APair<Str,Str>�̔z��ɂ���
// List�̒l�͑S�ĕ�����Ŏ擾�����̂ŁA�����悭�W�J����K�v������B
const bool RecordElements::onSplitParamsToPair(
	const std::string& src,
	std::vector<std::pair<std::string, std::string>>& rstArr
){
	std::vector<std::string> lineTokens, innerTokens;
	std::string keyStr, valStr;
	bool hasVal(false);

	kome::plugin::SettingsValue::separateListValue(src.c_str(), lineTokens);
	std::for_each(lineTokens.begin(), lineTokens.end(), [&](const std::string& tmp) {
		innerTokens.clear();
		kome::plugin::SettingsValue::separateListValue(tmp.c_str(), innerTokens);
	
		keyStr = kome::plugin::SettingsValue::convertToDisplayingString(innerTokens[0].c_str());
		if(innerTokens.size() > 1) {
			valStr = kome::plugin::SettingsValue::convertToDisplayingString(innerTokens[1].c_str());
			hasVal = true;
		} else {
			valStr = "";
		}
		rstArr.push_back(std::make_pair(keyStr,valStr));
	});
	return hasVal;
}

// -- inner ---------------------------------------------
//    �o�͒��O�ɍ������K�v�Ȓl��
// ------------------------------------------------------
// Accession�𕶎��{�����i0��)��8�����ɂ���
const std::string RecordElements::onMakeAccession(
	kome::objects::Spectrum* spec
){
	unsigned int spi, spidx = specArr_.size();
	for(spi = 0; spi < spidx; spi++) {
		if (specArr_.at(spi) == spec) break;
	}
	unsigned int spai, spaidx = avespecArr_.size();
	for(spai = 0; spai < spaidx; spai++) {
		if (avespecArr_.at(spai) == spec) {
			spi += spai;
			break;
		}
	}

	return onCreationAccession(prefix_, (basenum_ + spi));
}

const std::string RecordElements::onCreationAccession(
	const std::string& prefix,
	const unsigned int num	
) {
	unsigned int numLen = 8 - prefix.length();
	return FMT("%s%0*d", prefix.c_str(), numLen, num);
}


// Title ��������B
const std::string RecordElements::onMakeTitle(
	kome::objects::Spectrum* spec
){
	std::string rstStr;
	// Set [Chemical Name];
	std::vector<std::string> chvals;
	TGInItr it = onSearchTagInfoVal(kName);
	onSplitParams(it->value, chvals);
	rstStr = (!chvals.empty()) ? chvals[0] : kBlank;

	// Set [Inst Type]:
	it = onSearchTagInfoVal(kInstrumentType);
	rstStr += kcKS + it->value + kcKS;

	// Set [Ms Stage]
	std::string MsStr = onTryGetSpecReplaceProp(spec, tiMSType_->value);
	if (MsStr == "MS1") {
		MsStr = "MS";
	}

	rstStr += MsStr;
	rstStr += kcKS + Titles_;
	rstStr = onTryGetSpecReplaceProp(spec,rstStr);

	return rstStr;
}

// MS Type�̐���������镔��
const std::string RecordElements::onGetExtraMsType(kome::objects::Spectrum* spec) 
{
	tagInfo* ti = tiMSType_;
	std::string rstStr = onTryGetSpecReplaceProp(spec, ti->value);
	if (rstStr == "MS1" || rstStr == "MS") rstStr = "MS";
	rstStr = kMSType + kcB + rstStr;
	return onGetMassBankLine(ti->topTag, ti->tagName, rstStr);
}

// polarity�̐���������镔��
const std::string RecordElements::onGetExtraIonMode(kome::objects::Spectrum* spec)
{
	tagInfo* ti = tiPolarity_;
	std::string rstStr = onTryGetSpecReplaceProp(spec, ti->value);
	if (rstStr == "+") {
		rstStr = kPositive;
	} else if(rstStr == "-") {
		rstStr = kNegative;
	}
	rstStr = ext::tranceLowerToUpper(rstStr);
	rstStr = kIonMode + kcB + rstStr;
	return onGetMassBankLine(ti->topTag, ti->tagName, rstStr);
}

// Spectrum�̒��g��u������
const std::string RecordElements::onTryGetSpecReplaceProp(
	kome::objects::Spectrum* spec,
	const std::string& src
){
	std::string dst;
	if (spec != NULL) {
		// %�̃u���b�N����������
		if (src.find("%") != std::string::npos) {
			// %�ň͂܂ꂽ �v���p�e�B��u������B�ُ�u��������΁AerrorConvert��������
			dst = spec->getProperties().replaceString(src.c_str(),"%","%",errorConvert.c_str());
			// ��ϊ����������Ă��Ȃ� or �u�����s�v�B
			if (dst.find(errorConvert) == std::string::npos) {
				return dst;
			} else {
				// �u���������ǁA�ςɂȂ����Ƃ������Ƃ́A���C���̉\��������B
				dst = spec->getGroup()->getProperties().replaceString(src.c_str(),"%","%",errorConvert.c_str());
				// ���C�����Œu�������B
				if (dst.find(errorConvert) == std::string::npos) {
					return dst;
				}
			}
		}
	}
	return src;
}

//--------------------------------------------------------------------------
//
//   Get / Set Inner Tags.(�����p�����[�^�̃Z�b�g��Q�b�g��T�[�`�j
//
//--------------------------------------------------------------------------

// Function Operator s (for search tag And toptag)
class FindTag {
	const std::string tp;
	const std::string tg;
	bool useTp;
public:
	FindTag(
		const std::string& _top,
		const std::string& _tag
		): tp(_top)
		,tg(_tag) {
			useTp = (tp != kNBA);}

	bool operator()(const tagInfo& c) const {
		if(tg == c.tagName) {
			if (!useTp) return true;
			else return (tp == c.topTag);
		}
		return false;
	}
};

class FindSubTag {
	const std::string tp;
	const std::string sb;
public:
	FindSubTag(
		const std::string& _top,
		const std::string& _sub
		) :tp(_top)
		  ,sb(_sub){}
	bool operator()(const subtagInfo& c) const{
		return (tp == c.topTag && sb == c.subTopName);
	}
};

class FindSub {
	const std::string tg;
public:
	FindSub(const std::string& _tag) : tg(_tag){}
	bool operator()(const subTags& c) const{
		return (tg == c.tag);
	}
};

// Search Tag
TGInItr RecordElements::onSearchTagInfoVal(
	const std::string& _tag, 
	const std::string& _top
) {
	std::string t_top = ext::trancetoMassBankTag(_top);
	std::string t_tag = ext::trancetoMassBankTag(_tag);
	return std::find_if(tagInfoArr_.begin(), tagInfoArr_.end(), FindTag(t_top, t_tag));
}

// Search
STGInItr RecordElements::onSearchSubTagInfoVal(
	const std::string& _top, 
	const std::string& _sub
){
	std::string t_top = ext::trancetoMassBankTag(_top);
	std::string t_sub = ext::trancetoMassBankTag(_sub);
	return std::find_if(subtagInfoArr_.begin(), subtagInfoArr_.end(), FindSubTag(t_top, t_sub));
}

// Search
SubItr RecordElements::onSearchSubTag(
	STGInItr& it, 
	const std::string& _tag
) {
	std::string t_tag = ext::trancetoMassBankTag(_tag);
	return 	std::find_if(it->subtagArr.begin(), it->subtagArr.end(), FindSub(t_tag));
}

// TAGINFO��ǉ�����
void RecordElements::addTagInfo(
	const std::string& _top, 
	const std::string& _tag, 
	const bool _multi
) {
	std::string t_top = ext::trancetoMassBankTag(_top);
	std::string t_tag = ext::trancetoMassBankTag(_tag);
	tagInfoArr_.push_back(tagInfo(t_top, t_tag, _multi));
}

// SUB TAG INFO ��ǉ�����
void RecordElements::addSubTagInfo(
	const std::string& _top, 
	const std::string& _sub
) {
	std::string t_top = ext::trancetoMassBankTag(_top);
	std::string t_sub = ext::trancetoMassBankTag(_sub);
	subtagInfoArr_.push_back(subtagInfo(t_top, t_sub));
}

// SubTag Arr ����@SubTag���������ā@�ǉ� or �ύX����B
void RecordElements::addSubTagElem(
	const std::string& _top, 
	const std::string& _subtop, 
	const std::string& _tagname, 
	const std::string& _val, 
	const bool ishold
){
	STGInItr it = onSearchSubTagInfoVal(_top,_subtop);
	if (it != subtagInfoArr_.end()) {
		SubItr sit = onSearchSubTag(it, _tagname);
		if (sit == it->subtagArr.end()) {
			// �����̂Œǉ�
			std::string sname = ext::trancetoMassBankTag(_tagname);
			it->subtagArr.push_back(subTags(sname, _val, ishold));
		} else {
			// ����̂ŏ㏑��
			sit->val = _val;
		}
	}
}

// TAG��T��
const bool RecordElements::setTagInfoVal(
	const std::string& _tag, 
	const std::string& val, 
	const std::string& defval
) {
	return setTagInfoVal(kNBA, _tag, val, defval); 
}

// TAG��T���ā@����΁@�l��ύX���邩�A�f�t�H���g�i�K���l�j�������B
const bool RecordElements::setTagInfoVal(
	const std::string& _toptag, 
	const std::string& _tag, 
	const std::string& val, 
	const std::string& defval
) {
	TGInItr it = onSearchTagInfoVal(_tag, _toptag);
	if (it != tagInfoArr_.end()) {
		it->value = (val.empty()) ? defval : val;
		return true;
	}
	// ��������
	return false;
}

struct subtagSortOp {
	bool operator()(const subTags& l, const subTags& r) const {
		return l.tag < r.tag;
	}
};

// onSort!
void RecordElements::onSortABCSubTags()
{
	STGInItr it = subtagInfoArr_.begin();
	STGInItr eit = subtagInfoArr_.end();

	for(; it != eit; ++it) {
		SubItr x_it = it->subtagArr.begin();
		SubItr x_eit = it->subtagArr.end();
		for(; x_it != x_eit; ++x_it) {
			if(! x_it->isHold) break;
		}
		if (x_it != x_eit) {
			std::sort(x_it, x_eit, subtagSortOp());
		}
	}
}
