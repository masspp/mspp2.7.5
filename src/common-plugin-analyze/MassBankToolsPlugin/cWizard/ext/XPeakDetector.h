/**
 * @file XPeakDetector
 * @brief Spectrum PeakPicking & Filtering 
 *
 * @author M.Fukuda
 * @date 2013.03.08
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include "../rel_stdafx.h"

namespace kome {
	namespace massbank {
		namespace wizard {

			class XPeakDetector {
			public:
				// constructor
				XPeakDetector();

				// destructor
				~XPeakDetector();

			private:
				// my Peak Detector
				kome::plugin::PluginFunctionItem* peakDetector_;
				// Peak detector param
				kome::objects::SettingParameterValues* params_;
				// Filter Info
				std::string m_peakfilters;
				std::string procStream_;
				
				std::vector<kome::objects::XYDataOperation*> opts_;

			public:
				void setProcStream(const std::string& proc){procStream_ = proc;}

				void setOPTs(std::vector<kome::objects::XYDataOperation*> opts) {
					opts_ = opts;
				}

				const bool getProcceDataPoints(
					kome::objects::Spectrum* spec,
					const bool usingProcFg,
					kome::core::DataPoints& dps
					);

				// Peak Detector�̏����Z�b�g
				void setPeakInfo(
					kome::plugin::PluginFunctionItem* _peakdetector,
					kome::objects::SettingParameterValues* _params
					);

				// Peak Filter�̏����Z�b�g
				void setFilterInto(const std::string _filter);

				// �����̃A���S���Y���Ńs�[�N�s�b�L���O�ƃt�B���^�����O����B
				// param[in]     srcArr : �ΏۂƂȂ� Spectra
				// param[in-out] dstArr : �v�Z���ꂽ Spectra
				// param[in]     using..: ���H�ς݃f�[�^���g�����ۂ�
				// retrun : �v�Z���ɃG���[�������False.
				const bool doPeakPickAndFilter(
					std::vector<kome::objects::Spectrum*>& srcArr,
					std::vector<kome::objects::Spectrum*>& dstArr,
					const bool usingProcFg
					);

				const bool doPeakPickAndFilter(
					kome::objects::Spectrum* spec,
					kome::core::DataPoints& dps,
					const bool usingProcFg
					);


			private:
				// Peak Picking���s���B
				// ���ʂ� Peaks�ɕԂ��B
				const bool execSpectrumPeakPicking (
					kome::objects::Spectrum* spec, 
					kome::core::XYData* xydata,
					kome::objects::Peaks*& peaks
					);

				const std::string onExecPeakPicking(
					std::vector<kome::objects::Spectrum*>& srcArr,
					const bool useProc,
					std::vector<kome::objects::Spectrum*>& refSpecs,
					std::vector<kome::objects::Peaks*>& refPeaks
					);

				const std::string onExecPeakFilter(
					std::vector<kome::objects::Spectrum*>& specArr,
					std::vector<kome::objects::Peaks*>& peaksArr,
					std::vector<kome::objects::Spectrum*>& refArr
					);

				const std::string toErrorMsg(
					std::vector<kome::objects::Spectrum*>& nomatch,
					const int itype
					);

				const bool takeProccessing(
					kome::core::DataPoints& src,
					kome::core::DataPoints& dst,
					kome::objects::Spectrum* spec
					);

				const bool setupMyProccessingOperations();

			};

		}// using namespace
	}
}


