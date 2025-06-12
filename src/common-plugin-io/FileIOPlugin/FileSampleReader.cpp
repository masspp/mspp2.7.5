/**
* @file FileSampleReader.cpp
* @brief File Sample Reader
* @author OKADA, H.
* @date 2012/07/20
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
* <�⑫����>
*/

#include "stdafx.h"
#include "FileSampleReader.h"
#include "FileIOPluginOperation.h"


using kome::io::FileSampleReader;



// constructor
FileSampleReader::FileSampleReader(){
}


// destructor
FileSampleReader::~FileSampleReader(){
}


// RawDataFile �_�C�A���O���J���t�@�C���p�X��1�ȏ�I������B�e�X�̃L�[���t�@�C���p�X�Ƃ���B
std::string FileSampleReader::onSelectKeys( void ){
//	RawDataFileDialog

	std::string strFilePaths = "";

	kome::window::RawDataFileDialog dlg( kome::window::WindowTool::getMainWindow() );
	if( dlg.ShowModal() == wxID_OK ) {
		for( unsigned int i = 0; i < dlg.getNumberOfPaths(); i++ ) {
			if( i == 0 ){
				strFilePaths  = "";
			}else{
				strFilePaths += STR_FILE_STRINGS_SEPARATOR;	// 2�����ڈȍ~��SEPARATOR�����ŋ�؂�
			}

			std::string strTemp = dlg.getPath( i );
			strFilePaths += strTemp;
		}
	}

	return strFilePaths;
};


// �L�[���t�@�C���p�X�Ƃ��ăT���v�����擾����B
kome::objects::Sample* FileSampleReader::onOpenData( std::string strKey ){	// strKey�ɂ�file path��1��������

	// path
	std::string fileName;
	fileName = getfilename( strKey.c_str() );
		
	wxBeginBusyCursor();

	// Operation �쐬
	FileIoOperation* opt = new FileIoOperation();
	opt->setOperationType( kome::operation::Operation::TYPE_INPUT );
	opt->setShortName( "file_open" );
	opt->setOperationName( "File Open" );

	opt->setFilePath( strKey.c_str() );

	if( opt->execute() ){
		wxEndBusyCursor();
		return opt->getTargetSample();
	}else{
		delete opt;
	}

	wxEndBusyCursor();

	return 0;
};
