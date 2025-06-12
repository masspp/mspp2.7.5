/**
 * @file SampleTreeBuilderFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author okada_h
 * @date 2012.09.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "SampleTreeBuilderFunctions.h"
#include "SampleTreeBuilderManager.h"

#include <memory>	// for auto_ptr

using kome::plugin::FileFactory;	// for FileFactory
using kome::objects::SampleSet;		// for SampleSet
using kome::objects::Sample;		// for Sample
using kome::objects::DataGroupNode;	// for DataGroupNode
using kome::objects::Spectrum;		// for Spectrum

using kome::SampleTreeBuilder::SampleTreeBuilderSampleSet;
using kome::SampleTreeBuilder::SampleTreeBuilderSample;

#define MEMORY_LEAKER			/*NO_ACTION*/
#define MEMORY_LEAKER_TXT(X)	/*NO_ACTION*/


SampleTreeBuilderSampleSet::SampleTreeBuilderSampleSet(){

	m_p_csv_parser = new CsvParser;

	m_ptr_sample_new = NULL;

	SampleTreeBuilderManager &mgr = SampleTreeBuilderManager::getInstance();
	mgr.registPtr( this );
};


SampleTreeBuilderSampleSet::~SampleTreeBuilderSampleSet()
{

	delete m_p_csv_parser;	// @date 2012/10/03 <Add> OKADA
	m_p_csv_parser=NULL;

	SampleTreeBuilderManager &mgr = SampleTreeBuilderManager::getInstance();
	mgr.unregistPtr( this );

};

// on open file
bool SampleTreeBuilderSampleSet::onOpenFile( const char* path, kome::core::Progress* progress )  // add param @date 2014.07.08 <Mod> M.Izumi
{
	path = NULL;	// @date 2013/06/10 <Add> OKADA	// warning C4100: 'path' : �����͊֐��̖{�̕��� 1 �x���Q�Ƃ���܂���B
//	��������Sample�̔h���N���X�ASampleTreeBuilderSample ���쐬
	SampleTreeBuilderSample  *ptr_sample_new = new SampleTreeBuilderSample( this );
	m_ptr_sample_new = ptr_sample_new;

	this->addSample( ptr_sample_new );	// ������addSample����

	return ptr_sample_new->openSample();	// SampleTreeBuilderSample��onOpenSample( rootGroup )���Ă΂��BrootGroup�ɂ͏������ς݃C���X�^���X������
}

// on close file
bool SampleTreeBuilderSampleSet::onCloseFile() 
{
	return true;
}

// on open sample
bool SampleTreeBuilderSampleSet::onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress )  // add param @date 2014.07.08 <Mod> M.Izumi
{
	sample = NULL;	// @date 2013/06/10 <Add> OKADA	// warning C4100: 'sample' : �����͊֐��̖{�̕��� 1 �x���Q�Ƃ���܂���B
	return true;
}

// on close sample
bool SampleTreeBuilderSampleSet::onCloseSample( kome::objects::Sample* sample ) 
{
	sample = NULL;	// @date 2013/06/10 <Add> OKADA	// warning C4100: 'sample' : �����͊֐��̖{�̕��� 1 �x���Q�Ƃ���܂���B
	return true;
}

/* �������啶���ɒ����Ĕ�r */
int capitalized_strcmp(const char *s1, const char *s2)
{
    while (toupper(*s1) == toupper(*s2)) {  /* �������������ԃ��[�v���� */
        if (*s1 == '\0'){                   /* �����܂œ�������� */
            return (0);                     /* �O��Ԃ� */
		}
        s1++;                               /* ������̓r���Ȃ̂� */
        s2++;                               /* �C���N�������g���� */
    }    /* �������Ȃ��̂ŕ����̒l�̍���Ԃ� */
    return (toupper(*s1) - toupper(*s2));
};


bool SampleTreeBuilderSample::onOpenSample( DataGroupNode* rootGroup, kome::core::Progress* progress ) 
{
	bool bRet = false;

	if( rootGroup == NULL ){
		return false;	// �G���[
	}
	
	CsvParser* ptrCsvParser;
	ptrCsvParser = m_sampleSet->getPtrCsvParser();
	size_t sizeOfCsvDatas = ptrCsvParser->getNumberOfCsvData();	// csv�t�@�C���̍s��
	for( unsigned int i=0; i<sizeOfCsvDatas; i++ ){

		SampleSet* pSsSource = NULL;

		std::string strDataType = ptrCsvParser->getCsvData( i ).getDataType();
		if( capitalized_strcmp( strDataType.c_str(), "file" ) == 0 ){
			// file�ǂݍ��ݏ���
			FileFactory& factory = FileFactory::getInstance();
			std::string strFilename = ptrCsvParser->getCsvData( i ).getKey();
			pSsSource = factory.openFile( strFilename.c_str(), NULL );	// add param
		}else{
			// DIT-FP����̓ǂݍ��ݏ���
		}
		if (pSsSource == NULL) {
			std::string strFilename = ptrCsvParser->getCsvData( i ).getKey();
			printf( "Error: Can't open file. '%s'\n", strFilename.c_str() );
			return false;	// �G���[
		}
		ptrCsvParser->getCsvData( i ).setSs( pSsSource );	// �G

		// Sample Set�Ɋ܂܂�Ă���T���v���̌��𓾂�
		Sample* sampleSource = NULL;

		unsigned int nSample = 0;	// �ŏ��̂P�̂ݓǂݎ��
		sampleSource = pSsSource->getSample(nSample);
		if( sampleSource == NULL ){ 
			continue;	// nSample�Ԗڂ̃T���v�����擾�ł��Ȃ�����
		}
		
		bool bOpenedSource = sampleSource->isOpened();
		if( bOpenedSource == false ){
			sampleSource->openSample();	// �ǂݏo�����t�@�C���̃T���v�����I�[�v������Ă��Ȃ���΃I�[�v������
		}
		
		// �擾�������X�y�N�g���̖��O�𓾂�iopenFile()�֐����ĂԑO�ɐݒ肵�����j
		std::string strSpecName = ptrCsvParser->getCsvData( i ).getSpectrum();
		if( strSpecName == "" ){ 
			printf( "Error: Spectrum not found. \n" );
			continue; 
		}

		// �ǂݏo�����t�@�C���̃T���v����root node�𓾂�
		DataGroupNode* nodeSource = sampleSource->getRootDataGroupNode();
		if (nodeSource == NULL){
			continue;	// �ǂݏo�����t�@�C����root node�iDataGroupNode�j���擾�ł��Ȃ�����
			// �ʏ킠�蓾�Ȃ�
		}

		// �X�y�N�g�����Ō������ăX�y�N�g���𓾂�
		Spectrum* spec_Source = NULL;
		if( strSpecName != "*" ){
			spec_Source = getSpecByName( nodeSource, strSpecName.c_str() );
		}else{
			int n = 0;
			spec_Source =  nodeSource->getSpectrum(n);   // SpecName �� * �̏ꍇ�ɂ́A�擪�̃X�y�N�g���������ΏۂƂ���B
		}
		if( spec_Source == NULL ){ 
			// ���̖��O�̃X�y�N�g���́A�ǂݍ��݌��t�@�C���ɂ���܂���ł���
			printf( "Error: Spectrum not found. '%s'\n",  strSpecName.c_str() );
			return false;	// �G���[
		}

		ptrCsvParser->getCsvData( i ).setSpec( spec_Source );	// �J

		// �L
		// �e���w�肳��Ă��邩�`�F�b�N
		std::string strParent = ptrCsvParser->getCsvData( i ).getParent();

		if( strParent == "*" ){
			// �e�͎w�肳��Ă��Ȃ�
			ptrCsvParser->getCsvData( i ).setPspec( NULL );	// �N

			int nStageNum = atoi( ptrCsvParser->getCsvData( i ).getStageStr().c_str() );
			ptrCsvParser->getCsvData( i ).setStage( nStageNum );

		}else{
			// �e�͎w�肳��Ă���
			int nParent = atoi( strParent.c_str() );

			if( (int)sizeOfCsvDatas < nParent ){
				// �S�̂̍s�������傫���l���w�肳��Ă���̂�NG
				printf( "Error: Parent spectrum is not found. row no=%d\n", nParent );
				return false;	// �G���[
			}else{
				;
			}
			if( nParent <= 0 ){
				// �e�ԍ��́A1�`
				return false;	// �G���[
			}
			// �e�̎w��́A���݂̍s�ԍ����Ⴍ�Ȃ���΂Ȃ�Ȃ�
			if( nParent < ptrCsvParser->getCsvData( i ).getRow() ){
				// ����
			}else{
				// �e�ԍ�����������
				printf( "Error: Parent spectrum is not right. Number=%d\n", nParent );
				return false;	// �G���[
			}
			// �e�͎w�肳��Ă���
			bool flgIcchi = false;
			int nParentIndex = -1;
			for( unsigned int j=0; j<i; j++ ){
				if( nParent == ptrCsvParser->getCsvData( j ).getRow() ){
					// ��v����
					Spectrum* pSpec = ptrCsvParser->getCsvData( j ).getSpec();
					ptrCsvParser->getCsvData( i ).setPspec( pSpec );	// �N
					flgIcchi = true;
					nParentIndex = j;
				}
			}
			if( flgIcchi == false ){
				// �w�肳�ꂽ�e�̍s�ԍ���������Ȃ�
				return false;	// �G���[	
			}

			int nStageNum = atoi( ptrCsvParser->getCsvData( i ).getStageStr().c_str() );
			ptrCsvParser->getCsvData( i ).setStage( nStageNum );
		}

		// �X�y�N�g�������b�p�[�N���X�ɕϊ�
		kome::operation::TransformedSpectrum* trans_spec = new kome::operation::TransformedSpectrum( *spec_Source );
		if( trans_spec == NULL ){ continue;	}

		// ������ trans_spec �̓��e�����H���� 
		// �e�X�y�N�g����ݒ�
		{
			int nParent = atoi( strParent.c_str() );	// strParent���u*�v�̏ꍇ�AnParent��0�ɂȂ邩��A�ǂ�Ƃ���v���Ȃ�
			bool flgIcchi = false;
			for( unsigned int j=0; j<i; j++ ){
				if( nParent == ptrCsvParser->getCsvData( j ).getRow() ){
					// ��v����
					flgIcchi = true;
					trans_spec->setParentSpectrum( ptrCsvParser->getCsvData( j ).getTransSpec() );
				}
			}
			if( flgIcchi == false ){	// �e��������΁A�O�̂���NULL������
				trans_spec->setParentSpectrum( NULL );
			}
		}

		// MS Stage�ԍ���ݒ�

		// �e�X�y�N�g�����w�肳��Ă���A�i����1�j
		// ���A�e�X�y�N�g����MS Stage�ԍ�������ꍇ�i����2�j
		bool flgIn = false;
		if( std::string::npos == strParent.find( "*", 0 ) )			// ����1
		{	// ����1
			int nParentMSStageNo = -1;

			// �e��MS Stage�ԍ����擾
			int nParent = atoi( strParent.c_str() );
			for( unsigned int j=0; j<i; j++ ){
				if( nParent == ptrCsvParser->getCsvData( j ).getRow() ){
					// ��v����
					nParentMSStageNo = ptrCsvParser->getCsvData( j ).getStage();
				}
			}

			if( 0 < nParentMSStageNo ){	// ����2
				flgIn = true;
				std::string strMSStageNum = ptrCsvParser->getCsvData( i ).getStageStr();
				if( strMSStageNum != "*" ){
					// �e+1���w�肳��Ă����OK
					if(ptrCsvParser->getCsvData( i ).getStage() == nParentMSStageNo+1){
						// �e+1���w�肳��Ă����̂�OK
					}else{
						// �x�����b�Z�[�W��\��
						printf( "Warning: MS Stage is %d in csv file. Right value is %d. \n", ptrCsvParser->getCsvData( i ).getStage(), nParentMSStageNo+1 );
					}
					ptrCsvParser->getCsvData( i ).setStage( nParentMSStageNo+1 );	// �e+1�̒l�ɂȂ�
				}else{
					ptrCsvParser->getCsvData( i ).setStage( nParentMSStageNo+1 );	// �e+1�̒l�ɂȂ�
				}
				
			}
		}
		if( flgIn == false ){
			// ��L�A����1/2�̂ǂ���ɂ����Ă͂܂�Ȃ��ꍇ
			std::string strMSStageNum = ptrCsvParser->getCsvData( i ).getStageStr();
			if( std::string::npos == strMSStageNum.find( "*", 0 ) ){
				// MSStage �l�`�F�b�N
				int nMSStage = atoi(strMSStageNum.c_str());
				if(( nMSStage < 1 )||( 10 < nMSStage )){
					printf( "Error: MS Stage is %d\n", nMSStage );
					return false;	// �G���[
				}

				// ���̂܂�
				ptrCsvParser->getCsvData( i ).setStage( atoi(strMSStageNum.c_str()) );
			}else{
				// MS Stage��"*" ���ݒ肳��Ă���

				// ����MSStage���擾
				int nOriginalMSStageNo = ptrCsvParser->getCsvData( i ).getSpec()->getMsStage();
				ptrCsvParser->getCsvData( i ).setStage( nOriginalMSStageNo );
				if( 0 < nOriginalMSStageNo ){	// MSStage���ݒ肳��Ă���
					ptrCsvParser->getCsvData( i ).setStage( nOriginalMSStageNo );
				}else{
					ptrCsvParser->getCsvData( i ).setStage( nOriginalMSStageNo );
					// �ݒ肳��Ă��Ȃ��B�x���B
					printf( "Warning: Original MS Stage is not seted.\n" );
				}
			}
		}
		// �A�C�R����MS1, MS2, MS3, MS4, MS5, MS6, MS7, MSn������܂��B8�ȏ�� MSn �̃A�C�R���ɂ��ĉ������B
		int nStage = ptrCsvParser->getCsvData( i ).getStage();
		trans_spec->setMsStage( nStage );
		if(( 1 <= nStage )&&( nStage <= 7 )){	// MS���A�C�R�����w��
			char szIcon[16];
			sprintf( szIcon, "MS%d", nStage );
			trans_spec->setIcon( szIcon );
		}else if( 8 <= nStage ){
			trans_spec->setIcon( "MSn" );
		};

		// �v���J�[�T�C�I�����ʂ̐ݒ� //////////

		// ���Ŏw��B�ύX���Ȃ��ꍇ�́u*�v�BMS Stage�ԍ���1�̏ꍇ�́u�����v�i���̐��j�Ƃ���
		std::string strPcMass = ptrCsvParser->getCsvData( i ).getPcMassStr();
		if( std::string::npos == strPcMass.find( "*", 0 ) ){
			// "*"������
			// MS Stage�ԍ���1�̏ꍇ�́u�����v
			if( ptrCsvParser->getCsvData( i ).getStage() == 1 ){
				trans_spec->setPrecursor( -1.0 );
				ptrCsvParser->getCsvData( i ).setPcMassNum( -1.0 );
			}else{
				double precursor = atof( strPcMass.c_str() );
				trans_spec->setPrecursor( precursor );
				ptrCsvParser->getCsvData( i ).setPcMassNum( precursor );
			}
		}else{
			// �ύX���Ȃ�
			double precursor = trans_spec->getPrecursor( ptrCsvParser->getCsvData( i ).getStage() );
			ptrCsvParser->getCsvData( i ).setPcMassNum( precursor );
		}

		// ���e���V�����^�C���̐ݒ� //////////

		// ���Ŏw��B�ύX���Ȃ��ꍇ�́u*�v�B�u*�v�w�莞�Ɍ��f�[�^�Ƀ��e���V�����^�C�����ݒ肳��Ă��Ȃ��ꍇ�́A�x��
		std::string strRT =  ptrCsvParser->getCsvData( i ).getRetentionTimeStr();

		if( std::string::npos == strRT.find( "*", 0 ) ){
			// "*"������
			double dRT = atof( strRT.c_str() );
			ptrCsvParser->getCsvData( i ).setRetentionTime( dRT );
			trans_spec->setRt( dRT );
			trans_spec->setHasChromatogram( true );	// @date 2013/03/27 <Add> OKADA
		}else{
			// "*"������
			// �ύX���Ȃ�
			// �u*�v�w�莞�Ɍ��f�[�^�Ƀ��e���V�����^�C�����ݒ肳��Ă��Ȃ��ꍇ�́A�x��
			double dRT = trans_spec->getRt();
			if( dRT < 0 ){
				// ���e���V�����^�C�����ݒ肳��Ă��Ȃ�
				// �x��
				printf( "Warning: Retention time is nothing." );
			}
			ptrCsvParser->getCsvData( i ).setRetentionTime( dRT );
		}

		// SPEC 88275
		// scan number���A�e�[�u���icsv)�̍s�ԍ�-1�ɐݒ肷��
		trans_spec->setScanNumber( i );	// loop counter�i�ϐ� i�j��0����n�܂�̂ŁAi���Z�b�g���� // @date 2012/10/22 <Add> OKADA

		ptrCsvParser->getCsvData( i ).setTransSpec( trans_spec );	// trans_spec��ۑ�

		// �X�y�N�g����ǉ�
		rootGroup->addSpectrum( trans_spec );	// root node �̉��ɒǉ��i�K�w�݂͐��Ȃ��j
		bRet = true;

	}

	return bRet;
}


bool SampleTreeBuilderSample::onCloseSample() {
	return true;
};

// DataGroupNode�����疼�O�ŃX�y�N�g������������
Spectrum* getSpecByName( DataGroupNode* node, const char *szRequestedName ){
	const unsigned int numSpec = node->getNumberOfSpectra();
	for (unsigned int n = 0; n < numSpec; n++){
		Spectrum* spec = node->getSpectrum(n);
		const char *szName = spec->getName();


		// �啶���E������������ʂ��Ȃ���r�i"Scan 0"��"scan 0"�𓯂��悤�Ɉ����j
		size_t sizeName          = strlen(szName);
		size_t sizeRequestedName = strlen(szRequestedName);

		if( sizeName != sizeRequestedName ){
			continue;	// �����񒷂��قȂ�̂ŕs��v�B��v���Ȃ������̂Ŏ��̕������T��
		}

		bool flgIcchi = true;
		for( size_t sizeCount=0; sizeCount<sizeName; sizeCount++ ){
			if( toupper( szName[sizeCount] ) == toupper( szRequestedName[sizeCount] ) ){
				// ��v
			}else{
				// �s��v
				flgIcchi = false;
				break;
			}
		}
		if( flgIcchi == false ){
			continue;	// ��v���Ȃ������̂Ŏ��̕������T��
		}else{
			// ��v
			return spec;	
		}
	}

	const unsigned int numChild = node->getNumberOfChildren();
	for (unsigned int n = 0; n < numChild; n++){
		DataGroupNode* child = node->getChild(n);
		if (child != NULL){
			return getSpecByName(child, szRequestedName);
		}
		else{
			assert(false);
		}
	}

	return NULL;
}

// --------------------
// CsvParser
// --------------------
// constructor
CsvParser::CsvParser(){
};

// destructor
CsvParser::~CsvParser(){
};


// �R�}���h���C��������s�����Ƃ��̃G���g���֐�
kome::objects::Variant execSampleTreeBuilderCommandline( kome::objects::Parameters* params) 
{
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	ret = execSampleTreeBuilderBatch( params );

	return ret;
}

// ���j���[������s�����Ƃ��̃G���g���֐�
kome::objects::Variant execSampleTreeBuilderBatch( kome::objects::Parameters* params ) 
{
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	SampleTreeBuilderSampleSet *pSampleSetNew = new SampleTreeBuilderSampleSet;

	bool flgNonError = true;
	bool flgFileOpenOk = false;
	if( pSampleSetNew ){

		// ������csv�t�@�C����ǂݍ���

		// CSV�t�@�C�������擾
		// get settings
		kome::objects::SettingParameterValues* settings
			= kome::plugin::PluginCallTool::getSettingValues( params );
		if( settings == NULL ) {
			LOG_ERROR( FMT( "Failed to get the parameters file path." ) );
			return ret;
		}

		// csv file name
		const char* pszCsvFileName = NULL;
		pszCsvFileName = settings->getParameterValue( "csv_filename" );

		FILE * fp;
		char szRow[1024] = { '\0' };	// NULL�����Ŗ��߂�

		fp = fopen( pszCsvFileName , "r" );
		if( fp == NULL ) {
			printf( "Error: Can not open this file '%s'\n", pszCsvFileName );
			return ret;	// �t�@�C���I�[�v���G���[
		}

		while( fgets( szRow , sizeof( szRow ) , fp ) != NULL ) {
			szRow[sizeof( szRow )-1] = '\0';	// �s�v�����O�̂���

			size_t length = strlen(szRow);
			if( sizeof(szRow)-length <= 1 ){
				// buffer��t�܂œǂݍ���ł���
				printf( "row=%s", szRow );/* szRow�̖�����\n\0 */
				printf( "Error: Buffer over flow. In csv file, maximu length is %d bytes by row. \n", sizeof( szRow )-1 );
				fclose(fp);
				return ret;
			}
			if( szRow[0] == CSV_COMMENT_LETTER ){
				// �R�����g�s�ł��I
				printf( "Comment=%s" , szRow );
			}else{

				printf( "row=%s", szRow );/* szRow�̖�����\n\0 */

				int nRet = pSampleSetNew->getPtrCsvParser()->addRow( szRow );	// ������CSV�p�[�T�ɗ^����
				if( nRet != 0 ){
					flgNonError = false;	// �G���[
					break;
				}
				// �s�ԍ��`�F�b�N
				{
					// �s�ԍ����ُ킩�ۂ����`�F�b�N
					int nLines = pSampleSetNew->getPtrCsvParser()->getNumberOfCsvData();
					if( 1 <= nLines ){
						int nRow = pSampleSetNew->getPtrCsvParser()->getCsvData( nLines-1 ).getRow();
						if( nRow == nLines ){
							// �s���ƍŐV�̍s�ԍ�����v�BOK
						}else{
							;	// �s�ԍ����ُ�
							printf( "Error: Row number error.\n" );
							return ret;
						}
					}
				}
			}
		}

		if( flgNonError == true ){
			flgFileOpenOk = pSampleSetNew->openFile( "", NULL );	// SampleTreeBuilderSampleSet�N���X����onOpenFile()���Ă΂��
		}else{
			// �G���[�����������̂ŁA�ϊ��������Ȃ�
		}

		fclose(fp);
	}

	// ���̏�����ǉ�����ƁA������s�v�ɂȂ�(by �c��)
	if(( flgNonError == true )&&( flgFileOpenOk )){	// @date 2013/03/26 <Add> OKADA	// �t�@�C���I�[�v�����s���AonOpenSample()���ŃN���b�V������� �����ǉ�
		SampleTreeBuilderSample  *pSampleNew = (*pSampleSetNew).getSample();
		kome::plugin::PluginCallTool::onOpenSample( *pSampleNew );
	
		if( flgFileOpenOk ){
			// �t�@�C���ɏo�͂���ݒ�
			kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();	// manager
			aoMgr.setActiveSample( pSampleNew );
		}
	}

	if(( flgNonError == true )&&( flgFileOpenOk == true )){
		ret.prim.boolVal = true;
	}else{
		ret.prim.boolVal = false;
	}

	return ret;
}
