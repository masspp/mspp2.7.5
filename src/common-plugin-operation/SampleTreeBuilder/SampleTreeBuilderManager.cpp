/**
 * @file SampleTreeBuilderManager.cpp
 * @brief interfaces of sample tree builder manager functions
 *
 * @author okada_h
 * @date 2012.09.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "SampleTreeBuilderFunctions.h"
#include "SampleTreeBuilderManager.h"


// constructor
SampleTreeBuilderManager::SampleTreeBuilderManager(){
}

// destructor
// Mass++���I������ہA���̏������Ăяo�����BSampleTreeBuilderSampleSet*���폜���ꂸ�Ɏc���Ă���΁A�I�u�W�F�N�g���폜����
SampleTreeBuilderManager::~SampleTreeBuilderManager()
{
	// �v�f���t�����ɒH��
	std::set< kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* >::reverse_iterator it = m_setSsPtrs.rbegin();
	while( it != m_setSsPtrs.rend() )
	{
		kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* ptr = *it;
		delete ptr;	// ��object��delete�����ہAdestructor�o�R��unregistPtr()���Ă΂�܂�

		if( m_setSsPtrs.empty() ){
			break;
		}

		if( m_setSsPtrs.find( ptr ) == m_setSsPtrs.end() ){
			// �o�^����Ă��Ȃ�
		}else{
			m_setSsPtrs.erase( *it );	// ��object��delete�����ہAdestructor�o�R��unregistPtr()���Ă΂�邽�߁A�ʏ�A�����ɏ��������邱�Ƃ͖���
		}

		++it;
	}
	
}


// get instance
SampleTreeBuilderManager& SampleTreeBuilderManager::getInstance() {
	// create object
	static SampleTreeBuilderManager mgr;

	return mgr;
}
