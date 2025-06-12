/**
* @file SampleReader.cpp
* @brief Sample Reader
* @author OKADA, H.
* @date 2012/07/18
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
* <�⑫����>
*/


#include "SampleReader.h"


using kome::objects::SampleReader;


// constructor
SampleReader::SampleReader(){
}


// destructor
SampleReader::~SampleReader(){
}



// �L�[��I������B�����w��̏ꍇ�̓J���}( , ) ��؂�Ƃ���B
std::string SampleReader::selectKeys( void ){

	std::string strTemp;

    try {
        strTemp = onSelectKeys();
    } catch (const std::runtime_error& ex) {
        strTemp = ex.what();

		//cerr << ex.what() << endl;
    }
	catch (...) {
		strTemp = "";
	}


	return strTemp;

};


// �L�[�������Ƃ��ăT���v�����J���B
kome::objects::Sample* SampleReader::openData( std::string strKey ){
	return onOpenData( strKey );
};


