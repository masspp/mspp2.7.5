/**
* @file SampleReaderWrapper.cpp
* @brief Sample Reader Wrapper
* @author OKADA, H.
* @date 2012/07/18
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
* <�⑫����>
*/


#include "SampleReaderWrapper.h"
#include "ClrObjectTool.h"

using kome::clr::SampleReaderWrapper;

// constructor
SampleReaderWrapper::SampleReaderWrapper( kome::objects::SampleReader& sample_reader ) : m_sampleReader(sample_reader) {
}


// destructor
SampleReaderWrapper::~SampleReaderWrapper(){
}


// sampleReader��Ԃ��܂�
kome::objects::SampleReader&  SampleReaderWrapper::getSampleReader(){
	return m_sampleReader;
}


// SampleReader �I�u�W�F�N�g�� selectKeys ���Ăяo���B
System::String^ SampleReaderWrapper::selectKeys( void ){
	return ClrObjectTool::convertString( m_sampleReader.selectKeys().c_str(), "" );
};


// SampleReader �I�u�W�F�N�g�� openData ���Ăяo���B
kome::clr::SampleWrapper^ SampleReaderWrapper::openData( System::String^ strKey ){
	return ClrObjectTool::createSampleWrapper(m_sampleReader.openData( ClrObjectTool::convertString( strKey )	));
};

