/**
 * @file BatchJobListGrid.h
 * @brief interfaces of BatchGrid class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_BATCH_JOB_LIST_GRID_H__
#define __KOME_BATCH_SERVICE_BATCH_JOB_LIST_GRID_H__



/** �o�b�`�W���u���X�g */
typedef struct{
	std::string strStatus;	// �o�b�`���݂̏��
	std::string strDate;	// �o�b�`���s���s�������� 
	std::string strName;	// 
	std::string strServer;	// 
	std::string strComment;	// 
}BatchJobList;


namespace kome {
	namespace batchservice {

		class BatchJobListGrid : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn BatchJobListGrid( wxWindow* parent )
			 * @brief �R���X�g���N�^
			 * @param parent
			 */
			BatchJobListGrid( wxWindow* parent );	

			/**
			 * @fn virtual ~BatchJobListGrid()
			 * @brief �f�X�g���N�^
			 */
			virtual ~BatchJobListGrid();  			

		protected:
			/** �o�b�`�W���u���X�g */
			std::vector<BatchJobList>  m_vectBatchJobLists;

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief �e�[�u���쐬
			 */
			virtual void onCreate();			
						
			/**
			 * @fn virtual int getInt( const long long data, const int col ) const
			 * @brief �����l�擾
			 * @param data
			 * @param col
			 */
			virtual int getInt( const long long data, const int col ) const;
			
			/**
			 * @fn virtual std::string getString( const long long data, const int col ) const
			 * @brief ������擾
			 * @param data
			 * @param col
			 */
			virtual std::string getString( const long long data, const int col ) const;
			
		};
	}
}


























#endif
