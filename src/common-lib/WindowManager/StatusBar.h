/**
 * @file StatusBar.h
 * @brief Status Bar
 * @author OKADA, H.
 * @date 2011/01/07
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef _STATUSBAR_H_
#define _STATUSBAR_H_

#include <wx/statusbr.h> 
#include "MainFrame.h"
#include "wx/frame.h"

namespace kome {
	namespace window {
		
		/** 
		 * @class StatusBar
		 * @ brief Status Bar class
		 */
		class WINDOW_MANAGER_CLASS StatusBar : public wxFrameBase {
			
		protected:
			/**
			 * @fn StatusBar()
			 * @brief constructor
			 */
			StatusBar();
			
			/**
			 * @fn ~StatusBar()
			 * @brief constructor
			 */
			virtual ~StatusBar();
			
		public:
			/**
			 * @ typedef enum StatusBarTarget
			 * @brief status bar target type 
			 */
			typedef enum{
				STATUS_BAR_TARGET_NONE, 		// none
				STATUS_BAR_TARGET_SPECTRUM, 	// spectrum
				STATUS_BAR_TARGET_CHROMATOGRAM, // chromatogram
				STATUS_BAR_TARGET_DATAMAP 		// datamap
			}StatusBarTarget;

		public:
			/**
			 * @fn static StatusBar& getInstance( void )
			 * @brief get StatusBar object
			 * @return refer to the only StatusBar object
			 */
			static StatusBar& getInstance( void );

		protected:
			/**
			 * @fn void getStatusBarInfo( void )
			 * @brief get getStatusBarInfo / xml�t�@�C������ǂݍ��܂ꂽ�X�e�[�^�X�o�[����S�Ď擾����
			 */
			void getStatusBarInfo( void );
			
		public:
			/**
			 * @fn int setRangeStatusMsg( const char* szPos, const char* szMessage )
			 * @brief get setRangeStatusMsg / �X�e�[�^�X�o�[�ɕ\�����������������ݒ�
			 * @param[in] szPos �X�e�[�^�X�o�[�̖��O
			 * @param[in] szMessage �X�e�[�^�X�o�[�ɕ\������������������w��
			 * @return plug-in szPos�ɖ��O����v����X�e�[�^�X�o�[��������܂���ł���
			 */
			int setRangeStatusMsg( const char* szPos, const char* szMessage );

		protected:
			/**
			 * @fn void getMainWindow( void );
			 * @brief ���C���E�C���h�E�ւ̃|�C���^���擾
			 */
			void getMainWindow( void );

		protected:
			/**
			 * @fn int isDispNow( kome::plugin::StatusBarInfo& statusBarInfo )
			 * @brief �����Ŏw�肵���X�e�[�^�X�o�[��\�������邩�ۂ����ʂ���
			 * @param[in] statusBarInfo ���f����X�e�[�^�X�o�[���w��
			 * @return 0=�\�����ׂ��łȂ� 1=�\�����ׂ�
			 */
			int isDispNow( kome::plugin::StatusBarInfo& statusBarInfo );

		protected:
			/**
			 * @fn kome::plugin::StatusBarInfo::StatusBarTargetValue getActiveWindowKind( void )
			 * @brief ���݃A�N�e�B�u�ȃE�C���h�E�̎�ނ��擾����
			 * @return kome::plugin::StatusBarInfo::StatusBarTargetValue 
							VAL_SB_TGT_OTHER		�A�N�e�B�u�ȃE�C���h�E���擾�ł��Ȃ�
							VAL_SB_TGT_NONE			�A�N�e�B�u�ȃE�C���h�E�́Aspectrum, chromatogram, datamap�̂ǂ�ł��Ȃ�
							VAL_SB_TGT_SPECTRUM		�A�N�e�B�u�ȃE�C���h�E�́Aspectrum
							VAL_SB_TGT_CHROMATOGRAM	�A�N�e�B�u�ȃE�C���h�E�́Achromatogram
							VAL_SB_TGT_DATAMAP		�A�N�e�B�u�ȃE�C���h�E�́Adatamap

			 */
			kome::plugin::StatusBarInfo::StatusBarTargetValue getActiveWindowKind( void );

		public:
			/**
			 * @fn void dispAuto( void )
			 * @brief �S�ẴX�e�[�^�X�o�[���`�F�b�N���āA�\�����ׂ����̂�\��
			 */
			void dispAuto( void );

		protected:
			/**
			 * @fn int getPositionByName( const char* szName )
			 * @brief �����Ŏw�肳�ꂽ���O�̃X�e�[�^�X�o�[�̈ʒu�i�����琔���ĉ��Ԗڂ��j��Ԃ��B
			 * @param[in] szName �X�e�[�^�X�o�[��
			 * @return int �X�e�[�^�X�o�[�̈ʒu�B�����珇�ɁA0,1,2�c�Ɛ�����B
			 */
			int getPositionByName( const char* szName );

		public:
			/**
			 * @fn int setFlgDisp(  const char* szPos, int flgDisp )
			 * @brief �w�肵���X�e�[�^�X�o�[�̕\��/��\�������߂�
			 * @param[in] szPos �X�e�[�^�X�o�[��
			 * @param[in] flgDisp 0:���̃X�e�[�^�X�o�[���\�� 0�ȊO:�\��
			 * @return 0:���� -1:szPos�Ɉ�v����X�e�[�^�X�o�[�͂Ȃ�
			 */
			int setFlgDisp(  const char* szPos, int flgDisp );

			/**
			 * @fn int setStatusMsg( const char* szPos, const char* szMessage )
			 * @brief get setStatusMsg / �X�e�[�^�X�o�[�ɕ\�����������������ݒ�
			 * @param[in] szPos �X�e�[�^�X�o�[�̖��O
			 * @param[in] szMessage �X�e�[�^�X�o�[�ɕ\������������������w��
			 * @return plug-in szPos�ɖ��O����v����X�e�[�^�X�o�[��������܂���ł���
			 */
			int setStatusMsg( const char* szPos, const char* szMessage );
 		private:
			
			/** status bar */
			wxStatusBar *m_pStatbar;

			/** status bar info */
			std::vector< kome::plugin::StatusBarInfo > m_totalStatusBarInfo;

			/** MainFrame */
			kome::window::MainFrame* m_pFrame;

		};
	}
}


#endif
