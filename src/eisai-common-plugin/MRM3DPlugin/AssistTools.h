/**********************************************************
/*! @addtogroup ANYPLUGINS(FUTURE)
    @file       AssistTool.h
    @brief      PluginCallTools Helper Functions Header
    @date       2012.10.16
	@author     M.Fukuda
	@version    1.0.0
	@note 
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
**********************************************************
	@par   
	Mass++��PluginCall�����l�����N���X�E�J�v�Z��������B
	����^�����Ă���PluginCall��A�̏����𓝍�����ׁB

	�N���X��ParamName�Ƌ��ɍ쐬����B
	SetUp�t�F�[�Y�� setting����͂��ē��e���擾�B
	���s�̉ۂ𔻒肷��(�R���X�g���N�^���ł͂��ׂ��ł͂Ȃ���)
	invoke�Ŏ��s������B
**********************************************************/
#pragma once
#include <string>

namespace mspp {
namespace sprt {

/*!--------------------------------------------------------
    @name   PeakDetector
    @breif 	PluginCallTool �́@Peak Detector�@�́@�ȗ����x���N���X�B
	@date   2012.10.23
	@par	pluginCallTool���g��������^�����ȗ�����N���X
	        Setup �� SeetingParameterValues�̒��g��؂�o���B
			inovke�� ���s����B
			�e�X�s�K�؂ł���� false��Ԃ��B
-----------------------------------------------------------*/
/*! MASSPP UNIZON */
const std::string specFunc("SPEC_PEAK_PICKING");
const std::string chromFunc("CHROM_PEAK_PICKING");

class PeakDetector {
private:
	typedef kome::objects::SettingParameterValues tSetParam;

	const std::string paramName_; 	            /*! param name(From setting.xml data) */
	kome::plugin::PluginFunctionItem* m_func; 	/*! assigned peak detect function */
	tSetParam* m_setParams; 	                /*! peak detect function's param */
public:	
	/*! @breif constructor
	 *  @param[in] string : xml setting value name 
	 */
	explicit PeakDetector(const std::string& XMLparamname);

	/*! destructor */
	~PeakDetector();

	/*!
	 * @brief setup Class inner status.
	 * @param[in]  kome::objects::SettingParameterValues* : setting parameter object pointer
	 * @param[in]  bool : Call Peak Detector Type T/Chomatogram F/Spectrum
	 * @retval true Successed setup.
	 */
	const bool setUp(tSetParam* settings, bool isChrom);

	/*!
	 * @brief invoke set up peak detector
	 * @param[in]  kome::objects::XYData& : peaks XYData object
	 * @param[out] kome::ocjects::Peaks&  : detected result peaks objects  
	 * @retval true Successed peak picking
	 */
	const bool invokeMe(kome::core::XYData& xyData, kome::objects::Peaks& peaks);
};

/*!--------------------------------------------------------
    @name   PeakSmoother
    @breif 	Peak Smoothing�@�́@�ȗ����x���N���X�B
	@date   2012.10.24
	@par	pluginCallTool���g��������^�����ȗ�����N���X
	        Setup �� SeetingParameterValues�̒��g��؂�o���B
			inovke�� ���s����B
			�e�X�s�K�؂ł���� false��Ԃ��B
-----------------------------------------------------------*/
/*! MASSPP UNIZON */
const std::string kXYDataSmooth("XYDATA_FILTER");

class PeakSmoother {
private:
	typedef kome::objects::SettingParameterValues tSetParam;

	const std::string paramName_; 	            /*! param name(From setting.xml data) */
	kome::plugin::PluginFunctionItem* m_func; 	/*! assigned peak detect function */
	tSetParam* m_setParams; 	                /*! peak detect function's param */
public:
	/*! @breif constructor
	 *  @param[in] string : xml setting value name 
	 */
	explicit PeakSmoother(const std::string& XMLparamname);
	
	/*! destructor */
	~PeakSmoother();

	/*!
	 * @brief setup Class inner status.
	 * @param[in]  kome::objects::SettingParameterValues* : setting parameter object pointer
	 * @retval true Successed setup.
	 */
	const bool setUp(tSetParam* settings);

	/*!
	 * @brief invoke smoothing to XYData
	 * @param[in.out]  kome::objects::XYData& : peaks XYData object
	 * @retval true Successed smoothing
	 * @note replace orginal data.
	 */
	const bool invokeMe(kome::core::XYData*& srcXY);

	/*!
	 * @brief invoke smoothing from src to dst
	 * @param[in]  kome::objects::XYData& : src peaks XYData object
	 * @param[out] kome::ocjects::XYData& : dst peaks XYData object
	 * @retval true Successed smoothing
	 */
	const bool invokeMe(kome::core::XYData& srcXY, kome::core::XYData& dstXY);
};

/*!--------------------------------------------------------
    @name   progress Keeper
    @breif 	Progress�@�́@�ȗ����x���N���X�B
	@date   2012.10.23
	@par	Renge�쐬����B
	        nextStep �Ł@Position&Status�ύX����B
			�f�X�g���N�^��Progress��Fill���āY��B
-----------------------------------------------------------*/
class ProgressKeeper {
private:
	kome::core::Progress& progress_; /*! �Ǘ�����Progress */
	std::string status_;             /*! �\�����b�Z�[�W */
	unsigned int maxcount_;          /*! �J�E���g�̍ő�l */
	unsigned int totalcount_;        /*! ���J�E���g�� */
	unsigned int clockcount_;        /*! �T�C�N���P�ʂ̃J�E���g */
	unsigned int clockbasecount_;    /*! �T�C�N���̍ő�J�E���g */
	bool isEnd_;
public:
	ProgressKeeper (
		kome::core::Progress& progress, 
		unsigned int maxcount, 
		const std::string& statusMsg);
	// �I�u�W�F�N�g�I������FILL���ăN���[�Y������
	~ProgressKeeper();
	// ���ɐi�߂�B
	void showNext();
	// �����I���B
	void doFinish();
};

} // end of namespace
}
