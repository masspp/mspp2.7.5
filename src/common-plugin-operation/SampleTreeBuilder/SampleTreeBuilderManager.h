/**
 * @file SampleTreeBuilderManager.h
 * @brief interfaces of sample tree builder manager functions
 *
 * @author okada_h
 * @date 2012.09.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_SAMPLE_TREE_BUILDER_MANAGER_H__
#define __KOME_SAMPLE_TREE_BUILDER_MANAGER_H__


namespace kome{
	namespace SampleTreeBuilder{
		
		class SampleTreeBuilderManager{
		protected:
			/**
			 * @fn SampleTreeBuilderManager()
			 * @brief constructor
			 */
			SampleTreeBuilderManager();

			/**
			 * @fn virtual ~SampleTreeBuilderManager()
			 * @brief destructor
			 */
			virtual ~SampleTreeBuilderManager();

		private:
			// ���݃V�X�e�����ɂ��� SampleTreeBuilderSampleSet* �̈ꗗ�B�e�X�R���X�g���N�^��this�|�C���^��o�^�Adestructor�œo�^����
			std::set< kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* > m_setSsPtrs;

		public:
			/**
			 * @fn void registPtr( kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* ptr )
			 * @brief SampleTreeBuilderSampleSet* ��new����ہA���̏������Ăяo���A�I�u�W�F�N�g�̃|�C���^��o�^����
			 * @param ptr
			 */
			void registPtr( kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* ptr ){
				if( m_setSsPtrs.find( ptr ) == m_setSsPtrs.end() ){
					// �o�^����Ă��Ȃ�
					m_setSsPtrs.insert( ptr );	// �o�^
				}
			}

			/**
			 * @fn void unregistPtr( kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* ptr )
			 * @brief SampleTreeBuilderSampleSet* ���j�������ہA���̏������Ăяo���A�I�u�W�F�N�g�̃|�C���^�̓o�^����������
			 * @param ptr
			 */
			void unregistPtr( kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* ptr ){
				if( m_setSsPtrs.find( ptr ) == m_setSsPtrs.end() ){
					// �o�^����Ă��Ȃ�
				}else{
					m_setSsPtrs.erase( ptr );	// �o�^������
				}
			}

		public:

			/**
			 * @fn static SampleTreeBuilderManager& getInstance()
			 * @brief gets SampleTreeBuilderManager object
			 * @return SampleTreeBuilderManager object. (This is the only object.)
			 */
			static SampleTreeBuilderManager& getInstance();
		};
	}
}

using namespace kome::SampleTreeBuilder;


#endif
