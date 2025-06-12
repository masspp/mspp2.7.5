/**
 * @file VerticalBoxChromatogramCanvas.h
 * @brief VerticalBoxChromatogramCanvas.cpp�p�w�b�_
 * @author OKADA, H.
 * @date 2010.11
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 * 
 */

// >>>>>>	@Date:2013/05/23	<Modify>	A.Ozaki
//
// supported delete object
//
// �I���I�u�W�F�N�g�̍폜�A���ёւ��̋@�\��ǉ����܂���
//
// <<<<<<	@Date:2013/05/29	<Modify>	A.Ozaki

#ifndef _VERTICALBOXCHROMATOGRAMCANVAS_H_
#define _VERTICALBOXCHROMATOGRAMCANVAS_H_

#include <vector>
#include "VerticalBoxChromatogram.h"

namespace kome {

	namespace window {

		/**
		 * @class VerticalBoxChromatogramCanvas
		 * @brief �N���}�g�O�������p�l���ȏ�ɏc�ɕ��ׂĕ\������N���X
		 */
		class VerticalBoxChromatogramCanvas : public ChromatogramCanvas {	
		public:
			/**
			 * @fn VerticalBoxChromatogramCanvas( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param parent parent frame window
			 * @param width window width
			 * @param height window height
			 */
			VerticalBoxChromatogramCanvas( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~VerticalBoxChromatogramCanvas()
			 * @brief destructor
			 */
			virtual ~VerticalBoxChromatogramCanvas();
		protected:
			/** canvas set */
			static std::set< VerticalBoxChromatogramCanvas* > m_canvasSet;

		protected:
			/** 
			 * @struct ChromInfo
			 * @brief chromatogram drawing information
			 */
			struct ChromInfo
			{
				kome::operation::TransformedChromatogram*	chrom;
				COLOR	insColor;	// color
				bool	bFlg;		// draw flag
				int		nNum;		// stack number on canvas
			};

			/** chromatograms */
			std::vector< ChromInfo >	m_verticalBoxChroms;

			/** dummy chromatogram */
			kome::objects::VerticalBoxChromatogram	m_dummy;

			/** selected chromatograms */
			kome::objects::Chromatogram*	m_pSelectedChrom;

			/** �e�E�C���h�E */
			wxWindow*	m_parentLocal;

			/** Option�Őݒ肵���\����̍���(pixel)���i�[ */
			int	m_iHeightLocal;

			/** �X�N���[���ʒu�ipixel�P�ʁj */
			int m_iScrollPos;

			int m_chromPos;

			// selected list index
			int	m_nIndex;

			// object number on canvas
			int	m_nIndexDisp;
		public:
			/**
			 * @fn virtual kome::objects::Chromatogram* onAddChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief This method is called when a chromatogram is added.
			 * @param chrom chromatogram
			 * @return chromatogram object to be added
			 */
			kome::objects::Chromatogram* onAddChromatogram( kome::objects::Chromatogram* chrom );

			/** 
			 * @fn virtual void onRemoveChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief This method is called when a chromatogram is removed.
			 * @param[in] chrom chromatogram
			 */
			virtual void onRemoveChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void setSelectedChrom( kome::objects::Chromatogram* chrom )
			 * @brief adds selected chromatogram
			 * @param[in] chrom chromatogram to be selected
			 */
			void setSelectedChrom( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void updateActiveObject()
			 * @brief updates active object
			 */
			void updateActiveObject();

			/**
			 * @fn void update( void )
			 * @brief updates canvas
			 */
			void	update( void );

			/**
			 * @fn void onDrawWindowFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foreground (override method)
			 * @param g graphics
			 */
			void onDrawWindowFg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawProfile( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing profile (override method)
			 * @param g graphics
			 */
			void onDrawProfile( kome::window::DCGraphics& g );

			/**
			 * @fn void refreshHeight( void )
			 * @brief This method is reflesh Height
			 */
			void refreshHeight( void );	// 2011/03/09

			/**
			 * @fn void onSize( const unsigned int width, const unsigned int height )
			 * @brief This method is called when window size is changed (override method)
			 * @param width window width
			 * @param height window height
			 */
			void onSize( const unsigned int width, const unsigned int height );

			/**
			 * @fn int getHeightLocal( void )
			 * @brief Option�Őݒ肵���X�y�N�g��/�N���}�g�O�����\����̍������擾
			 * @return �����FOption�Őݒ肵���X�y�N�g��/�N���}�g�O�����\����̍��� ���s�F���̐�
			 */
			int getHeightLocal( void );

			/** 
			 * @fn virtual void onDrawChromatogramProfile(
						kome::window::DCGraphics& g,
						kome::objects::Chromatogram& chrom
					)
			 * @brief draws Chromatogram profile
			 * @param g graphics
			 * @param chrom Chromatogram
			 */
			virtual void onDrawChromatogramProfile(
				kome::window::DCGraphics& g,
				kome::objects::Chromatogram& chrom
				, int iVBCnt=0
			);

			/**
			 * @fn virtual void onDrawGraphics( wxDC& dc )
			 * @brief this method is called when drawing
			 * @param dc device context to be drawed
			 */
			virtual void onDrawGraphics( wxDC& dc );

			/**
			 * @fn void onScroll( wxScrollWinEvent& evt )
			 * @brief �X�N���[���o�[���������ꍇ�̏���
			 * @param[in] evt mouse event information
			 */			
			void onScroll( wxScrollWinEvent& evt );

			/**
			 * @fn void onScrollWinLineup( wxScrollWinEvent& evt )
			 * @brief �X�N���[���o�[��1�s�オ�����ꍇ�̏���
			 * @param[in] evt mouse event information
			 */			
			void onScrollWinLineup( wxScrollWinEvent& evt );

			/**
			 * @fn void onScrollWinLinedown( wxScrollWinEvent& evt )
			 * @brief �X�N���[���o�[��1�s���������ꍇ�̏���
			 * @param[in] evt mouse event information
			 */			
			void onScrollWinLinedown( wxScrollWinEvent& evt );
			/**
			 * @fn void onScrollWinTop( wxScrollWinEvent& evt )
			 * @brief �X�N���[���o�[����[�ɒB�����ꍇ�̏���
			 * @param[in] evt mouse event information
			 */			
			void onScrollWinTop( wxScrollWinEvent& evt );

			/**
			 * @fn void onScrollWinBottom( wxScrollWinEvent& evt )
			 * @brief �X�N���[���o�[�����[�ɒB�����ꍇ�̏���
			 * @param[in] evt mouse event information
			 */			
			void onScrollWinBottom( wxScrollWinEvent& evt );

			/**
			 * @fn void onScrollWinPageup( wxScrollWinEvent& evt )
			 * @brief �X�N���[���o�[�̏�̗̈���N���b�N�����ꍇ�̏���
			 * @param[in] evt mouse event information
			 */			
			void onScrollWinPageup( wxScrollWinEvent& evt );

			/**
			 * @fn void onScrollWinPagedown( wxScrollWinEvent& evt )
			 * @brief �X�N���[���o�[�̉��̗̈���N���b�N�����ꍇ�̏���
			 * @param[in] evt mouse event information
			 */
			void onScrollWinPagedown( wxScrollWinEvent& evt );

			/**
			 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foregorund (override method)
			 * @param g graphics
			 */
			void onDrawGraphFg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphBg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph backgorund (override method)
			 * @param g graphics
			 */
			void onDrawGraphBg( kome::window::DCGraphics& g );

			/**
			 * @fn void onDrawWindowBg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing window background (override method)
			 * @param g graphics
			 */
			void onDrawWindowBg( kome::window::DCGraphics& g );

			/**
			 * @fn SetScrollbarsVB( int pixelsPerUnitY, int noUnitsY, int yPos = -1, bool noRefresh = false)
			 * @brief �X�N���[���o�[�\���ʒu�� ���� �������ʒu�ɐݒ肷��
			 * @param[in] pixelsPerUnitY	Pixels per scroll unit in the vertical direction.
			 * @param[in] noUnitsY			Number of units in the vertical direction.
			 * @param[in] yPos				Position to initialize the scrollbars in the vertical direction, in scroll units.
			 * @param[in] noRefresh			Will not refresh window if true. (Default false)
			 */
			void SetScrollbarsVB( int pixelsPerUnitY, int noUnitsY, int yPos = -1, bool noRefresh = false);

			/**
			 * @fn AdjustScrollbars( void )
			 * @brief �X�N���[���o�[�\���ʒu�� m_iScrollPos �������ʒu�ɐݒ肷��
			 */
			void AdjustScrollbars( void );

			/**
			 * @fn kome::objects::Chromatogram* getBaseChromatogram( void )
			 * @brief gets base Chromatogram object. �ʂ̃X�y�N�g����`�悷��ۂ́A�Y������spectrum object��Ԃ��B����ȊO�̏ꍇ�͑S�ẴX�y�N�g������������object��Ԃ��B
			 * @return base Chromatogram object
			 */
			kome::objects::Chromatogram* getBaseChromatogram( void );

			/**
			 * @fn virtual void onPrepareDraw( kome::window::DCGraphics& g )
			 * @brief this method is called when prepareing (override method). �擾���Ă���X�y�N�g����transformed Spectrum�ŁA�s�[�N��񂪊܂܂�Ȃ����߃s�[�N�l���\���ł��Ȃ��B�{�֐���transformed Spectrum���I���W�i����Spectrum�ɕϊ�����B
			 * @param g graphics object to draw
			 */
			void onPrepareDraw( kome::window::DCGraphics& g );
			/**
			 * @fn void onMouseWhell( wxMouseEvent& evt )
			 * @brief �}�E�X�z�C�[�����̏���
			 * @param[in] evt mouse event information
			 */
			void onMouseWhell( wxMouseEvent& evt );

			/**
			 * @fn virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is down (override method)
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is up (override method)
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseDoubleClick( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is double clicked. (override method)
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseDoubleClick( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor is moved. (override method)
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt );

			/**
			 * @fn void SetScrollPosVB( void )
			 * @brief iScrollPos�̒l�ɏ]���ăX�N���[���ʒu��ݒ肵�܂��B
			 * @param iScrollPos �X�N���[���ʒu���w�肵�܂�
			 * @return �Ȃ�
			 */
			void SetScrollPosVB( int iScrollPos=0 );

			/**
			 * @fn void DispGraphTitle( kome::window::DCGraphics& g )
			 * @brief �e�O���t�Ƀ^�C�g����\�����܂�
			 * @param g graphics
			 * @return �Ȃ�
			 */
			void DispGraphTitle( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void getChromDispCanvas( std::vector< kome::objects::Chromatogram* >& chroms )
			 * @brief Gets the chromatogram displayed on the canvas
			 * @param chrm �L�����o�X�ɕ\�����̃N���}�g�O����
			 */
			virtual void getChromDispCanvas( std::vector< kome::objects::Chromatogram* >& chroms );

		protected:
			/**
			 * @fn virtual wxMenu* createMenu()
			 * @brief creates menu (override method)
			 * @return created menu
			 */
			virtual	wxMenu*	createMenu( );

		public:
			/**
			 * @fn void onVerticalboxSettings( wxUpdateUIEvent& evt )
			 * @brief Stacked View Settings ���N���b�N���ꂽ�Ƃ��Ă΂��
			 * @param[in] evt Mouse Event
			 */
			void onVerticalboxSettings( wxCommandEvent& evt );

			/**
			 * @fn void onUpdatePopupMenu( wxUpdateUIEvent& evt )
			 * @brief sets the status of popup-menu
			 * @param[in] evt update UI event information
			 */
			void onUpdatePopupMenu( wxUpdateUIEvent& evt );

			/**
			* @fn int GetVerticalBoxChromatogramNum( int nNum )
			* @brief gets chromatogram number
			* @param[in] nNum positon of number
			* @return chromatogram number
			*/
			int	GetVerticalBoxChromatogramNum( unsigned int nNum );

			/**
			 * @fn void clearSelectedChroms()
			 * @brief clears selected chromatograms
			 */
			void	clearSelectedChroms( void );

			/**
			 * @fn void addSelectedChrom( kome::objects::Chromatogram* chrom )
			 * @brief adds selected chromatogram
			 * @param[in] chrom chromatogram to be selected
			 */
			void	addSelectedChrom( kome::objects::Chromatogram* pChrom );

			/**
			 * @fn void removeSelectedChrom( kome::objects::Chromatogram* chrom )
			 * @brief removes selected chromatogram
			 * @param[in] chrom chromatogram to be unselected
			 */
			void	removeSelectedChrom( kome::objects::Chromatogram* pChrom );

			/**
			 * @fn unsigned int getNumberOfSelectedChroms()
			 * @brief gets the number of selected chromatograms
			 * @return the number of selected chromatograms
			 */
			unsigned int	getNumberOfSelectedChroms( void );

			/**
			* @fn kome::objects::Chromatogram* getSelectedChrom( const unsigned int idx )
			* @brief gets selected chromatogram
			* @param[in] idx selected chromatogram index
			* @return selected chromatogram
			*/
			kome::objects::Chromatogram* getSelectedChrom( const unsigned int idx );

			/**
			 * @fn void setIndex( const int nIndex )
			 * @brief set the number of selected list
			 * @param[in] the number of selected list
			 */
			void	setIndex( const int nIndex );

			/**
			 * @fn int getIndex( void )
			 * @brief get the number of selected list
			 * @return the number of selected list
			 */
			int	getIndex( void );
			
			/**
			 * @fn void setIndexDisp( const int nIndex )
			 * @brief set the number of position on canvas
			 * @param[in] the number of position on canvas
			 */
			void	setIndexDisp( const int nIndexDisp );

			/**
			 * @fn int getIndexDisp( void )
			 * @brief get the number of position on canvas
			 * @retrun the number of position on canvas
			 */
			int	getIndexDisp( void );

			/**
			 * @fn int searchSelectedChrom( kome::objects::Chromatogram* chrom )
			 * @brief searches selected chromatogram
			 * @param[in] chrom chromatogram
			 * @return selected chromatogram index (If negative value, specified chromatogram is not selected.)
			 */
			int	searchSelectedChrom( kome::objects::Chromatogram* pChrom );

			/**
			 * @fn void setCallFunctionSelectedChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief set call function selected Chromatogram
			 * @param chrom chromatogram objects
			 */
			void	setCallFunctionSelectedChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void exchange(int nS,int ne)
			 * @brief exchange data
			 * @param[in] nS data exchange position 1
			 * @param[in] nE data exchange position 2
			 */
			void	exchange( const int nS, const int nE );

			/**
			 * @fn int getClieckObjectNumber( kome::evt::MouseEvent& evt )
			 * @brief seek a position of an spectrum from the depressed position on the canvas
			 * @param[in] evt Mouse Event
			 * @retrun position on canvas
			 */
			int		getClickObjectNumber( kome::evt::MouseEvent& evt );

			/**
			* @fn kome::objects::Chromatogram* getTransformedChrom( )
			* @brief get selected Chromatogram ( index is m_nIndexDisp )
			* @return selected chromatogram
			*/
			kome::objects::Chromatogram*	getTransformedChrom( );

			/**
			 * @fn int onMouseCommonAction( kome::evt::MouseEvent& evt )
			 * @brief common action for mouse event
			 * @param[in] evt Mouse Event
			 * @retrun selected chromatogram
			 */
			kome::objects::Chromatogram*	onMouseCommonAction( kome::evt::MouseEvent& evt );	

		private:
			DECLARE_EVENT_TABLE( )
		};

	}
};

#endif
