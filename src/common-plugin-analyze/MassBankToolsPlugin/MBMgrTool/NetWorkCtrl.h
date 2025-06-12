/**
 * @file NetWorkCtrl
 * @brief NetWorkCtrl
 *
 * @author M.Fukuda
 * @date 2013.04.24
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank {

		class NetWorkCtrl {
		public:
			NetWorkCtrl();
			~NetWorkCtrl(){}

		private:
			// WSDL
			kome::net::Wsdl m_wsdl; // wdsl
			std::string url_;       // massbank URL
			std::string proxy_;     // proxy
			bool useNetFg_;         // network���g�����ۂ�
			bool changedFg_;        // �ύX�_�����������ۂ�
			bool showNG_;           // �ڑ��s�̌x�����o�����ۂ�
			bool hasExFg_;          // Ex�n�̖��߂ɐڑ��悪�Ή����Ă��邩�ۂ�

		public:
			// Getter
			bool hasEx(void) const {return hasExFg_;}

			// �ڑ������m�F����
			const bool checkMyConnect(bool showNGFg = false);

			// method, xmlStr�̓��e�� MassBankAPI�����s����B
			// ���ʂ� res�ɋA���Ă���
			const bool execAPI(
				const std::string& method,
				std::string& xmlStr,
				std::vector<char>& refRes,
				int& errCode
				);

		private:
			// �l�b�g���[�N���(parameter.ini)�̕ύX���Ď�����B
			// ���݂� Proxy��massbank url�̂�
			const bool onCheckNewInfoChanged();

			// Wdsl�̌��ʂ��o���Ă���
			const bool onWsdlConnectControler();

			// Wdsl�� url_ �Őڑ����Ă݂�
			bool onLoadWsdl();

			// SOAP�̑Ή��֌W�𒲂ׂ�
			const bool onCheckSOAP(
				const std::string& method
				);
		};
	}
}
