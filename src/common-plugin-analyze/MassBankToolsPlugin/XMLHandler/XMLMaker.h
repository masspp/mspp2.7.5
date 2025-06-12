/**
 * @file XMLMaker
 * @brief MassBank��XML�𐶐�����
 *
 * @author M.Fukuda
 * @date 2013.04.24
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank {

		class XMLMaker {
		public:
			// constructor
			XMLMaker();

			// descructor
			~XMLMaker();
		private:
			// onAppendFixed���g���ۂɁA�ꎞ�I�Ɏg���B
			kome::objects::SettingParameterValues* settings_;

			// PrivateMethod
			std::string xml_;
			std::string method_;
			bool hasExFg_;       // ver 2.6.0 ��� searchSpectrumEx�� Ex�n�ɑΉ����ۂ�

		public:
			// ��������XML���擾����
			std::string getXML() const { return xml_;}

			// ��������method���擾����
			std::string getMethod() const { return method_;}

			void setHasExFg(const bool fg) {hasExFg_ = fg;}

			// Instruments���擾����XML�����
			void setGetInstrumentTypes();

			// spectrum search���擾����XML�����
			void setSimilarSpectraXML(
				kome::core::XYData& peaks,
				kome::objects::SettingParameterValues* settings,
				const std::string& tgtinstruments);

			// Batch Search���s��XML�����
			void setBatchSearchXML(
				std::vector<kome::objects::Spectrum*> spectra,
				kome::objects::SettingParameterValues* settings,
				const std::string& tgtinstruments);

			void setBatchSearchXML(
				const std::string& specQuerys,
				kome::objects::SettingParameterValues* settings);


			// search peaks / diff.. ���s�� XML�����
			void setSearchPeaksXML(
				kome::objects::SettingParameterValues* settings,
				const std::string& tgtinstruments);

			void setDiffSearchPeaksXML(
				kome::objects::SettingParameterValues* settings,
				const std::string& tgtinstruments);


			void setGetJobStatus(const std::string& jobId);

			void setGetJobResult(const char* jobId);

			void setGetRecordInfo(const char* jobId);

			void setGetPeak(const char* jobId);

			const std::string getQueryStringsTag(
				const char* scanname,
				kome::objects::Peaks* peaks);

			const std::string onGetInstrumentTypesTag();

			const std::string onGetInstrumentTypesTag(
				const std::vector<std::string>& instrumenttypes);

		private:
			// Job ID�̃^�O���쐬����
			const std::string onSetJB_ID(const char* jobId);

			const std::string onAppendFixedDouble(
				const std::string& tgt,
				const double value
				);

			// ���[���ɑ�����(sType) XML��Ԃ�
			const std::string onAppendFixedType(
				const std::string& tgt,
				const int sType);

			const std::string onAppendFixedTypes(
				const int sType,
				const std::string& tagPtr,
				const std::string& tgt,
				const bool noParamFg = false
				);


			const std::string onSetMZSArray(const std::string& listKey);

			// MassBank XML���쐬����
			const std::string onSetMSBKFormat();

			// MassBank XML���쐬����icontent..���Ԃɋ��ݍ���)
			const std::string onSetMSBKFormat(const std::string& contentReq);

			// Job..�n��XML���쐬����(method���Ⴄ�j
			void onSetJobStatus(
				const std::string& method,
				const char* jobId);

			// Test�ׂ̈�XML�R�[�h�쐬
			const std::string onMakeTestXML_searchSpectrum(const bool exFg);
			const std::string onMakeTestXML_searchPeak(const bool exFg);
			const std::string onMakeTestXML_searchPeakDiff(const bool exFg);

		};
	}
}
