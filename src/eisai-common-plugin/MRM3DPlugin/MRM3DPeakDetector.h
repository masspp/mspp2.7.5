/**********************************************************/
/*! @addtogroup MRM3DPlugin 
    @file       MRM3DPeakDetector.h
    @brief      Filter Parser Functions Header
    @date       2012.10.24
	@author     M.Fukuda
	@version    1.0.0
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
 **********************************************************
2012.10.24
MRM3D Peak Picking�̃��C���N���X
*/
/**********************************************************/
#pragma once

// �O���錾
namespace mspp{
	namespace sprt{
		class PeakDetector;
		class PeakSmoother;
	}
}

namespace kome{
namespace mrm3d{
	
class MRMChromKeeper;
class MRMChrom;
class ParamParser;

/*!--------------------------------------------------------
    @name   MRM3DCore
    @breif 	MRM3DPeakPicking main Class
	@date   2012.10.23
	@par	Chromatogram�Ƃ���MRM�f�[�^�𒊏o
	        Smoothing, Peak Picking, Filtering���s���A
			Peaks2d�@�Ƃ��ā@���ʂ�Ԃ��B
-----------------------------------------------------------*/
class MRM3DCore {

private:
	/*! @emum
	 *  @brief MRM3D Step Number
	 */
	enum MRM3DStep {
		CONVERT,   // Convert step
		PEAKPICK,  // PeakPicking step
		STEP_NUM,
	};
	/*! type def */
	typedef std::shared_ptr<MRMChrom> tMRMChromPtr;
	typedef std::vector<tMRMChromPtr> tvMRMPtr; /* t(ypedef) v(ector) �̐ړ���*/

	/*! my Parameters*/
	kome::objects::SettingParameterValues* m_settings;
	ParamParser* m_parser;

public:
	explicit MRM3DCore( kome::objects::SettingParameterValues* settings );
	~MRM3DCore();
private:
	// copy�̗ދ֎~�E�h�~
	MRM3DCore(const MRM3DCore& m) {}
	MRM3DCore& operator=(const MRM3DCore& rhs){}

public:
	/*!
	 * @brief execute mrm3d Peak Picking
	 * @param[in]  kome::objects::DataSet& dataSet Objects
	 * @param[in]  kome::core::Progress& progress objects pointer
	 * @param[out] kome::objects::Peaks2D& results 2D Peaks
	 */
	void executeLabeling (
		kome::objects::DataSet& dataSet,
		kome::core::Progress& progress,
		kome::objects::Peaks2D& peaks);

private:

	/*!
	 * @brief check having mrm filter infomations.
	 * @param[in]  kome::objects::DataSet& dataSet Objects
	 * @retval true Is Having MRM Infomation
	 * @note check spectrum[0] having filter info only.
	 */
	const bool isHavingMRMType (kome::objects::DataSet& dataSet);

	/*!
	 * @brief get MRMData From DataObjects
	 * @param[in]  kome::objects::DataSet& �T���v���̃f�[�^�Z�b�g
	 * @param[in]  kome::core::Progress& �@getMRMData�t�F�[�Y�̃v���O���X�|�C���^
	 * @param[out] std::vector<std::shared_ptr<MRMChrom>> MRMData�擾���ʂ�MRMChrom�z��
	 * @retval true Success get MRM Infomation
	 * @par  �espectrum�����o���AMRMChrom�Ƀf�[�^���W�ς���i���L�֐��ōs��)
	 *       MRMChrom�̔z��Ƃ��ĕԂ��B
	 */
	const bool getMRMDatas (
		kome::objects::DataSet& dataSet,
		kome::core::Progress& progress,
		tvMRMPtr& allChroms);

	/*!
	 * @brief divide xydata to any MRMchroms
	 * @param[in]     kome::objects::Spectrum* : parse Spectrum
	 * @param[in out] MRMChromKeeper*& : set MRMChrom XYData from Spectrum
	 * @par MRMChromKeeper�̊eMRMChrom�����@Q3�@�͈͓̔���
	 *      Spectrum ������XYData�̊Y���|�C���g������Θa�Z����B
	 *      �eMRMChrom�� Spec��RT : X, Q3�͈͓��̍��v intensity : Y �ƂȂ�B
	 */
	void divideXYDataPoints (
		kome::objects::Spectrum* spectrum,
		MRMChromKeeper*& stocker);

	/*!
	 * @brief Detect Chromatgram Peaks
	 * @param[in]  std::shared_ptr<MRMChrom>&
	 * @param[in]  mspp::sprt::PeakDetector*
	 * @param[in]  mspp::sprt::PeakSmoother*
	 * @param[out] kome::objects::Peaks2D& results 2D Peaks
	 * @par MRMChrom����XYData���\�����A
	 *      Smoothing, PeakPicking���s�� 
	 *      Peaks2D�Ɍ��ʂ�ǉ�����B
	 */
	void detectAndConvertPeaks (
		tMRMChromPtr& chromPtr,
		mspp::sprt::PeakDetector* pkFunc,
		mspp::sprt::PeakSmoother* pksmth,
		kome::objects::Peaks2D& peaks2d);

	/*!
	 * @brief convert from MRMChromData to xydata
	 * @param[in]  std::shared_ptr<MRMChrom>&
	 * @param[out] kome::core::XYData*
	 * @retval true Success convert
	 * @note Mass++Plugin��smoothing,PeakPick������ׁA
	 *       �W���̃f�[�^�`���ɃR���o�[�g����B
	 */
	const bool convertToXYData (
		tMRMChromPtr& chromPtr,
		kome::core::XYData* xydata);

	/*!
	 * @brief convert from Peaks to Peaks2D
	 * @param[in]  std::shared_ptr<MRMChrom>&
	 * @param[in]  kome::core::XYData&
	 * @param[out] kome::objects::Peaks2D& results 2D Peaks
	 * @retval true Success convert
	 * @note Peaks(���ʂ̌���)���@Q1,Q3�̏�����������
	 *       2DPeaks�̃f�[�^�`���ɃR���o�[�g����B
	 */
	const bool convertTo2DPeaks (
		tMRMChromPtr& chromPtr,
		kome::objects::Peaks& tmpPeaks,
		kome::objects::Peaks2D& peaks2d);

	/*!
	 * @brief convert from Peaks to Peaks2D
	 * @param[in-out] kome::objects::Peaks2D& results 2D Peaks
	 * @retval true Success Filtered
	 */
	const bool FilterPeak2D (
		kome::objects::Peaks2D& peaks2d);

};


}
}
