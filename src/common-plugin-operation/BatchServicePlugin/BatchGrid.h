/**
 * @file BatchGrid.h
 * @brief interfaces of BatchGrid class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_BATCHGRID_H__
#define __KOME_BATCH_SERVICE_BATCHGRID_H__

#include <vector>
#include "BatchServicePluginManager.h"
#include "BatchProcessingMethodConfigurationDialog.h"	// @date 2012/10/17 <Add> OKADA

namespace kome {
	namespace batchservice {

		class BatchProcessingMethodConfigurationDialog;	//	@date 2012/10/17 <Add> OKADA

		class BatchGrid : public kome::window::GridListCtrl {
		public:

			/**
			 * @fn BatchGrid(	wxWindow* parent)
			 * @brief constructor
			 * @param parent parent window
			 */
			BatchGrid( wxWindow* parent );    // �R���X�g���N�^

			/**
			 * @fn virtual ~BatchGrid()
			 * @brief destructor
			 */
			virtual ~BatchGrid();    // �f�X�g���N�^

		protected:
			/** �e�E�C���h�E�̃|�C���^ */
			kome::batchservice::BatchProcessingMethodConfigurationDialog* m_pParentDlg;	// @date 2012/10/17 <Add> OKADA

		protected:
			/** �����l�擾 */
			virtual int getInt( const long long data, const int col ) const;

			/** ������擾 */
			virtual std::string getString( const long long data, const int col ) const;

			/** �s���ݒ� */
			int BatchGrid::setRows( int nRow );

			/** �A�N�V�������X�g�F�@Initialize�`�EFilterResults */
			std::vector<Action>  m_vectActions;

		protected:
			/** �Z���̍s�� */
			int m_nRows;

			/** �Z���̗� */
			int m_nCols;

		protected:
			/**
			 * @fn void init( void )
			 * @brief initialize
			 */
			void BatchGrid::init( void );	// ������

			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method (override method)
			 */
			virtual void onCreate();		// �e�[�u���쐬

		public:
			/**
			 * @fn int addAction( Action& act )
			 * @brief add action
			 * @param act new action
			 * @return result of add. -1=error(not added) 0=success(added)
			 */
			int addAction( Action act );

			/**
			 * @fn int updateAction( int index, Action act )
			 * @brief update line
			 * @param index This line is overwite by act.
			 * @param act   new action
			 * @return result of update. -1=error(not updated) 0=success(updated)
			 */
			int updateAction(  int index, Action act );

			/**
			 * @fn int deleteAction( int index )
			 * @brief delete line
			 * @param index This line is deleted.
			 * @return result of delete. -1=error(not deleted) 0=success(deleted)
			 */
			int deleteAction( int index );

			/**
			 * @fn int RefreshBatchGrid( void )
			 * @brief redraw sheet
			 * @return always 0
			 */
			int RefreshBatchGrid( void );

			/**
			 * @fn int getActionSize( void )
			 * @brief gets action size
			 * @return Action size
			 */
			int getActionSize( void ){
				return m_vectActions.size();
			}

			/**
			 * @fn Action getAction( int index )
			 * @brief gets action value
			 * @param index index of Actions
			 * @return Action value
			 */
			Action& getAction( int index );

		protected:
			/**
			 * @fn void onColClick( wxListEvent& evt )
			 * @brief This method is called when list column is clicked
			 * @param[in] evt list event information
			 */
			void onColClick( wxListEvent& evt );

			/**
			 * @fn void onItemActivated( wxListEvent& evt )
			 * @brief This method is called when list is ENTER or double clicked
			 * @param[in] evt list event information
			 */
			void onItemActivated( wxListEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};

	}
}

#endif
