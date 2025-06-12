/*********************************************************/
/*!
 @addtoGroup AB3D
 @file DbController.h
 @brief AB3D Peak Position DB Controller
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
    AB3D Position����Ă΂�āADB�̈�A�̑����㗝����B
 */
/*********************************************************/
#pragma once

//�O���錾
namespace kome {
	namespace core{
		class Buffer;
	}
	namespace objects{
		class DataSet;
		class DataGroupNode;
	}
	namespace db{
		class Statement;
	}
}

namespace AB3D {
	class TmpTblCtrl;
	class DbProxyClass;
	struct TmpPeakVal;
}

namespace AB3D {
/*!--------------------------------------------------------
    @name   DbController
    @brief  Data Base �� AB3D ����R���g���[������
    @date   2013.07.01
    @par    AB3D�{�̂���Ă΂��
            Matrix�ɐݒ肳�ꂽ�T���v���̊Ǘ�(Next��)���s���B
-----------------------------------------------------------*/
	class DbController {

	public:
		// constructor
		DbController();
		// destructor
		~DbController();
		
		// Uncopyable
		DbController(const DbController&);
		DbController& operator=(const DbController&);

	private:
		// Temp Table ����̃I�u�W�F�N�g
		std::unique_ptr<TmpTblCtrl> tblPtr_;

		// Table ����̃I�u�W�F�N�g(Proxy)
//		std::unique_ptr<DbProxyClass> pxyPtr_;

		// �g�p����Matrix��ID
		int matrixId_;
		// ���݌v�Z���̃T���v����ID
		int nowSampleId_;
		//// �T���v���S�̂̌o�ߐ�
		//unsigned int nowCount_;
		//// �T���v���̔z��(ID�Q)
		//std::vector<int>sampleIds_;

	public:
		/*!
		 * @brief  DB���Z�b�g�A�b�v�ł��邩�A��Ԃ��`�F�b�N����
		 * @retval DB���g���Ȃ��ꍇ��False��Ԃ�
		 */
		//const bool checkDBStatus();

		/*!
		 * @brief  ��Ɨp��TempTable���쐬����
		 * @retval �쐬���ɃG���[��f�����Ƃ���False��Ԃ�
		 */
		const bool setupTempTables();

		/*!
		 * @brief  Temp Table�̃X�e�[�g�����g���Z�b�g�A�b�v����
		 * @retval �o���Ȃ��ꍇ��False��Ԃ�
		 */
		const bool setupTempSTMT();

		///*!
		// * @brief  EMS/Mass++ Table�̃X�e�[�g�����g���Z�b�g�A�b�v����
		// * @retval �o���Ȃ��ꍇ��False��Ԃ�
		// * @note
		//      Mass++�ł͖{�X�e�[�g�����g�͎g�p���Ȃ�
		// */
		//const bool setupMsppSTMT();

		//// �����Ă���T���v���T�C�Y��Ԃ�(Proguress�ׂ̈�)
		//unsigned int getSampleNumbers() const {
		//	return sampleIds_.size();
		//}

		///*!
		// * @brief  ���̃T���v���ɑJ�ڂ���
		// * @retval ����������� False.
		// * @note
		//      �����I�Ƀ|�C���^��i�߂Ď����A�N�e�B�u�ɂ���
		// */
		//const bool doNextSample();

		void setSampleId(const int spId);

		///*!
		// * @brief      ���݂̃T���v����XYData���Ăі߂�
		// * @param[out] (�����I��)�A�N�e�B�u�ȃT���v��DataSet
		// */
		//const bool getNowDataGroup(
		//	kome::objects::DataSet& refDataSet);

		///*!
		// * @brief  ���݂̃T���v�����i�J���Ă���΁j����
		// */
		//void onCloseActivegroup();

		/*!
		 * @brief      XIC��TempTable�ɏ�������
		 * @param[in]  �^�[�Q�b�g�̃s�[�N
		 * @param[in]  XIC�Ƃ��Ă̎��f�[�^(XYPoints)
		 * @param[in]  XIC�̃s�[�N�̌��B
		 * @retval     TempTable�ɏ������񂾌���ID(PeakID)���߂�
		 * @note
		      AB3D��������n���ꂽ���_�ŁA�s�[�N�̌��̒���
			  �^�[�Q�b�g�̃s�[�N�����邩�Ȃ������肵�Ă��Ȃ��̂ŁA
			  �����Ŕ��肷��K�v���L��
		 */
		const int setXICData(
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
			);

		/*!
		 * @brief      Charge�̒l���A�b�v�f�[�g����
		 * @param[in]  �Ώۂ�Peak ID
		 * @param[in]  Charge
		 */
		void updateCharge(
			const int peakId,
			const int charge);

		/*!
		 * @brief      TempTable����Y���f�[�^���擾����
		 * @param[out] ��v����TempPeak���Q
		 * @result     �擾�̐���
		 */
		const bool getMatchDataFromTemp(
			std::vector<TmpPeakVal*>& tpvArr);

		///*!
		// * @brief      Table��Row(Peak)+Value����������
		// * @param[in]  M/Z
		// * @param[in]  Retention Time(Min)
		// * @param[in]  Charge
		// * @param[in]  ����ROW�ɑ�����Value�Q
		// */
		//void writeRowLine(
		//	const double mz,
		//	const double rt,
		//	const int charge,
		//	std::vector<AB3D::TmpPeakVal*>& tpvArr);

		void getDataPointsFromTemp(
			const int tmpProfId,
			kome::core::DataPoints& dps);

		/*!
		 * @brief �I���t�F�[�Y�𑖂点��
		 */
		void finishMakeMatrix(const bool ret);

	private:
		/*!
		 * @brief  ���݂̃T���v�����i�J���Ă���΁j����
		 * @retval DB�̃I�u�W�F�N�g��Ԃ�
		 */
		kome::db::Database* onGetDB();

		const int setTempProfile(
			const std::vector<std::pair<double, double>>& xicXyData
			);


		/*!
		 * @brief      XYPoints��Buffer�ɕϊ�
		 * @param[in]  XYPoints Data
		 * @param[out] Buffer �I�u�W�F�N�g
		 * @retval     �ϊ����ɃG���[�������False
		 */
		const bool tranceToBuffer(
			const std::vector<std::pair<double, double>>& xicXyData,
			kome::core::Buffer& rstBuffer);

		///*!
		// * @brief      Temp Profile��Buffer��o�^
		// * @param[in]  Buffer �I�u�W�F�N�g
		// * @retval     �o�^����temp profile ID
		// */
		//const int onSetTmpProfile(
		//	kome::core::Buffer& buffer);
	};
}
