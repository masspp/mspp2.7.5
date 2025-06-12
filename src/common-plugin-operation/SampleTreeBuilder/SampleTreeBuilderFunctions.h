/**
 * @file SampleTreeBuilderFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author okada_h
 * @date 2012.09.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_SAMPLE_TREE_BUILDER_FUNCTIONS_H__
#define __KOME_SAMPLE_TREE_BUILDER_FUNCTIONS_H__


#include <vector>
#include <boost/tokenizer.hpp>

using kome::plugin::FileFactory;	// for FileFactory
using kome::objects::SampleSet;		// for SampleSet
using kome::objects::Sample;		// for Sample
using kome::objects::DataGroupNode;	// for DataGroupNode
using kome::objects::Spectrum;		// for Spectrum

#define CSV_TEXT_BUFFER	1024	/* CSV�t�@�C��1�s������̍ő�ǂݍ��݃o�C�g�� */
#define CSV_COMMENT_LETTER '#'	/* CSV�t�@�C���̃R�����g�����B�擪�����̕����Ȃ�A�R�����g�s */
#define CSV_KOUMOKU_SU		8	/* CSV�t�@�C���̍��ڐ��Brow,data type,key,spectrum,parent,stage,pc mass,retention time�ŁA8���� */


enum{
	COL_NO_ROW,
	COL_NO_DATA_TYPE,
	COL_NO_KEY,
	COL_NO_SPECTRUM,
	COL_NO_PARENT,
	COL_NO_STAGE,
	COL_NO_PC_MASS,
	COL_NO_RT
};

namespace kome{
	namespace  SampleTreeBuilder{

		class SampleTreeBuilderSample;

		// csv�t�@�C�� 1�s���̃f�[�^���i�[
		class CsvLine{		//�@CsvLine�@class
		public:
			CsvLine(){
				m_pSs =NULL;
				m_pSample = NULL;
				m_pSpec =NULL;
				m_pPspec =NULL;
				m_dPcMass = -1;
				m_dRetentionTime = -1;
				m_pTransSpec = NULL;
			};
			virtual ~CsvLine(){};

		public:
			// �A �s�ԍ�
			void setRow( int nRow ) { m_nRow = nRow; }  
			int getRow( void ) { return m_nRow; }
			// �C �f�[�^�^�C�v
			void setDataType( std::string strDataType ) { m_strDataType = strDataType; }  
			std::string getDataType( void ) { return m_strDataType; }
			// �E �L�[�i�t�@�C�����j
			void setKey( std::string strKey ) { m_strKey = strKey; }  
			std::string getKey( void ) { return m_strKey; }
			// �G SampleSet*
			void setSs( SampleSet* pSs ) { m_pSs = pSs; }  
			SampleSet* getSs( void ) { return m_pSs; }
			// �G�f Sample*	
			void setSample( Sample* pSample ) { m_pSample = pSample; }  
			Sample* getSample( void ) { return m_pSample; }
			// �I �X�y�N�g����
			void setSpectrum( std::string strSpectrum ) { m_strSpectrum = strSpectrum; }  
			std::string getSpectrum( void ) { return m_strSpectrum; }
			// �J Spectrum*
			void setSpec( Spectrum* pSpec ) { m_pSpec = pSpec; }  
			Spectrum* getSpec( void ) { return m_pSpec; }
			// �L �e�X�y�N�g���w��q	
			void setParent( std::string strParent ) { m_strParent = strParent; }  
			std::string getParent( void ) { return m_strParent; }
			// �N �e�X�y�N�g����Spectrum*
			void setPspec( Spectrum* pPspec ) { m_pPspec = pPspec; }  
			Spectrum* getPspec( void ) { return m_pPspec; }
			// �P MS Stage�ԍ�
			void setStageStr( std::string strStageNum ) { m_strStageNum = strStageNum; }  
			std::string getStageStr( void ) { return m_strStageNum; }
			// �P' 
			void setStage( int nStage ) { m_nStage = nStage; }
			int getStage( void ) { return m_nStage; }
			// �R �v���J�[�T�C�I������
			void setPcMass( std::string strPcMass ) { m_strPcMass = strPcMass; }  
			std::string getPcMassStr( void ) { return m_strPcMass; }
			// �R�f
			void setPcMassNum( double dPcMass ) { m_dPcMass = dPcMass; }
			double getPcMassNum( void ) { return m_dPcMass; }
			// �T
			void setRetentionTimeStr( std::string strRetentionTime ) { m_strRetentionTime = strRetentionTime; }  
			std::string getRetentionTimeStr( void ) { return m_strRetentionTime; }
			// �T�f
			void setRetentionTime( double dRetentionTime ) { m_dRetentionTime = dRetentionTime; }
			double getRetentionTime( void ) { return m_dRetentionTime; }
			// �V
			void setTransSpec( kome::operation::TransformedSpectrum* pSpec ) { m_pTransSpec = pSpec; }  
			 kome::operation::TransformedSpectrum* getTransSpec( void ) { return m_pTransSpec; }

		private:
			int			m_nRow;				// �s�ԍ�							// �A	// csv
			std::string	m_strDataType;		// �f�[�^�^�C�v					// �C	// csv
			std::string	m_strKey;			// �L�[�i�t�@�C�����j				// �E	// csv
			SampleSet*	m_pSs;				// SampleSet*						// �G
			Sample*		m_pSample;			// Sample*							// �G'
			std::string	m_strSpectrum;		// �X�y�N�g����					// �I	// csv
			Spectrum*	m_pSpec;			// Spectrum*						// �J
			std::string	m_strParent;		// �e�X�y�N�g���w��q				// �L	// csv
			Spectrum*	m_pPspec;			// �e�X�y�N�g����Spectrum*		// �N
			std::string	m_strStageNum;		// MS Stage�ԍ�					// �P	// csv
			int			m_nStage;			// ������������					// �P'
			std::string	m_strPcMass;		// �v���J�[�T�C�I������			// �R	// csv
			double		m_dPcMass;			// ������������					// �R�f
			std::string	m_strRetentionTime;	// ���e���V�����^�C��				// �T	// csv
			double		m_dRetentionTime;	// ������������					// �T�f
			kome::operation::TransformedSpectrum*	m_pTransSpec;			// kome::operation::TransformedSpectrum*						// �V
																			// ���R�����g�Ɂucsv�v�Ƃ��郁���o�́Acsv�t�@�C������ǂݍ��܂�܂�
		};

		class CsvParser{		// CsvParser class
		
		public:
			/**
			 * @fn CsvParser();
			 * @brief Constructor
			 */
			CsvParser();
			/**
			 * @fn virtual ~CsvParser();
			 * @brief destructor
			 */
			virtual ~CsvParser();

		private:
			/** csv datas */
			std::vector<kome::SampleTreeBuilder::CsvLine> m_CsvDatas;

		public:
			/**
			 * @fn size_t getNumberOfCsvData( void )
			 * @brief get number of csv data
			 * @return number of csv data
			 */
			size_t getNumberOfCsvData( void ){
				return m_CsvDatas.size();
			};

			/**
			 * @fn kome::SampleTreeBuilder::CsvLine& getCsvData( int index )
			 * @brief get csv data
			 * @return csv data
			 */
			kome::SampleTreeBuilder::CsvLine& getCsvData( int index ){
				return m_CsvDatas.at( index );
			};

			/**
			 * @fn kome::SampleTreeBuilder::CsvLine& setCsvDataSampleSet( int index, kome::SampleTreeBuilder::CsvLine& csv_data )
			 * @brief set csv data sample set
			 * @return csv data sample set
			 */
			kome::SampleTreeBuilder::CsvLine& setCsvDataSampleSet( int index, kome::SampleTreeBuilder::CsvLine& csv_data ){
				m_CsvDatas[index] = csv_data;

				return m_CsvDatas.at( index );
			};
			/**
			 * @fn int addRow( char * szLine )
			 * @brief add row
			 * @return -1:error 0:success
			 */
			int addRow( char * szLine )
			{
				CsvLine csv_line;

				if( szLine[0] == CSV_COMMENT_LETTER ){
					return -1;
				}

				typedef boost::escaped_list_separator<char> esc_sep;
				typedef boost::tokenizer<esc_sep > tokenizer;

				std::string s = szLine;

				esc_sep sep('@', ',', '\"');

				tokenizer tokens( s,  sep );

				int col = 0;
				std::string strSeparatedText;
				for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter, ++col ) {
					strSeparatedText = *tok_iter;

					switch( col ){
					case COL_NO_ROW:
						{	// �G���[����
							int nRowNo = atoi( strSeparatedText.c_str() );
							if( nRowNo <= 0 ){
								printf( "Error: Row No Error'%s'\n", strSeparatedText.c_str() );
								return -1;
							}
						}
						csv_line.setRow( atoi( strSeparatedText.c_str() ) );
						break;
					case COL_NO_DATA_TYPE:
						csv_line.setDataType( strSeparatedText );
						break;
					case COL_NO_KEY:
						csv_line.setKey( strSeparatedText );
						break;
					case COL_NO_SPECTRUM:
						csv_line.setSpectrum( strSeparatedText );
						break;
					case COL_NO_PARENT:
						csv_line.setParent( strSeparatedText );
						break;
					case COL_NO_STAGE:
						csv_line.setStageStr( strSeparatedText );
						break;
					case COL_NO_PC_MASS:
						csv_line.setPcMass( strSeparatedText );
						break;
					case COL_NO_RT:
						csv_line.setRetentionTimeStr( strSeparatedText );
						break;
					default:
						// error
						break;
					}
				}
				if( col != CSV_KOUMOKU_SU ){
					printf( "Error: The number of items is not right. Item count=%d\n", col  );
					return -1;
				}

				std::cout << std::endl;
				m_CsvDatas.push_back( csv_line );

				return 0;
			}
		};


		class SampleTreeBuilderSampleSet : public kome::objects::SampleSet {			// SampleTreeBuilderSampleSet class
			public:
				/**
				 * @fn SampleTreeBuilderSampleSet();
				 * @brief Constructor
				 */
				SampleTreeBuilderSampleSet();
				/**
				 * @fn virtual ~SampleTreeBuilderSampleSet();
				 * @brief destructor
				 */
				virtual ~SampleTreeBuilderSampleSet();

			protected:
				/**
				 * @fn virtual bool onOpenFile( const char* path )
				 * @brief This method is called by openFile method. (override method)
				 * @param path file path
				 * @param progress
				 * @return If true, it succeeded to open the file
				 */
				virtual bool onOpenFile( const char* path, kome::core::Progress* progress = NULL );


				/**
				 * @fn virtual bool onCloseFile()
				 * @brief This method is called by closeFile method. (override method)
				 * @param path file path
				 * @return If true, it succeeded to close the file
				 */
				virtual bool onCloseFile();

			public:
				/**
				 * @fn virtual bool onOpenSample( kome::objects::Sample* sample )
				 * @brief This method is called by openTreatment method. (override method)
				 * @param treatment treatment object to be opened (If NULL, opens all treatments.)
				 * @param progress
				 * @return If true, it succeeded to open the treatment.
				 */
				virtual bool onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress = NULL );
			
				/**
				 * @fn virtual bool onCloseSample( kome::objects::Sample* sample )
				 * @brief This method is called by closeTreatment method. (override method)
				 * @param treatment treatment object to be closed (If NULL, closes all treatments.)
				 * @return If true, it succeeded to close the treatment.
				 */
				virtual bool onCloseSample( kome::objects::Sample* sample );

		public:

			/**
			  * @fn void setSpectra( std::vector < Spectrum* >vpSpectra )
			  * @brief set spectra
			  * @param vpSpectra spectra
			  */
			void setSpectra( std::vector < Spectrum* >vpSpectra ){
				m_vpSpectra = vpSpectra;
			};

		private:
			/** spectra */
			std::vector < Spectrum* >m_vpSpectra;

		private:
			/** ptr sample new */
			SampleTreeBuilderSample  *m_ptr_sample_new;

		public:

			/**
			 * @fn SampleTreeBuilderSample* getSample( void )
			 * @brief get sample
			 * @return ptr sample new
			 */
			SampleTreeBuilderSample* getSample( void ){
				return m_ptr_sample_new;
			}
		
		private:
			/** �ǂݏo�����ɂ��Ă̏����i�[����:SourceFileName */
			std::string m_strSourceFileName;

			/** �ǂݏo�����ɂ��Ă̏����i�[����:SpecName */
			std::vector<std::string> m_a_strSpecName;

		public:
			/**
			 * @fn std::string getSpecNameByIndex( unsigned int index )
			 * @brief get spec name by index
			 * @param index index
			 * @return spec name
			 */
			std::string getSpecNameByIndex( unsigned int index ){
				if( m_a_strSpecName.size() < index ){
					return "";
				}

				return m_a_strSpecName.at( index );
			}

			/**
			 * @fn unsigned int getNumberOfSpecNameArray( void )
			 * @brief get number of spec name array
			 * @return number of spec name array
			 */
			unsigned int getNumberOfSpecNameArray( void ){
				return m_a_strSpecName.size();
			}

			// csv�t�@�C������ۑ��A�qcsv�p�[�T�[�N���X���܂ނ悤�ɂ���

		private:
			/** csv parser */
			CsvParser* m_p_csv_parser;
		public:

			/**
			 * @fn CsvParser* getPtrCsvParser(void)
			 * @brief get ptr csv parser
			 * @return ptr csv parser
			 */
			CsvParser* getPtrCsvParser(void){	
				return m_p_csv_parser;
			}

		};


		class SampleTreeBuilderSample : public kome::objects::Sample {					// SampleTreeBuilderSample class
			
		public:
			/**
			 * @fn
			 * @brief
			 */
			SampleTreeBuilderSample( SampleTreeBuilderSampleSet* sampleSet ): Sample( sampleSet ){
				m_sampleSet = sampleSet;
			};

			/**
			 * @fn
			 * @brief
			 */
			virtual ~SampleTreeBuilderSample(){};

		protected:
			/**
			 * @fn virtual bool onOpenSample( DataGroupNode* rootGroup ) = 0
			 * @brief This method is called by openSample method. (abstract method)
			 * @param[out] rootGroup root spectrum group
			 * @return If true, it succeeded to open this sample.
			 */
			virtual bool onOpenSample( DataGroupNode* rootGroup, kome::core::Progress* progress = NULL );

			/**
			 * @fn virtual bool onCloseSample() = 0
			 * @brief This method is called by closeSample method. (abstract method)
			 * @return If true, it succeeded to close this sample.
			 */
			virtual bool onCloseSample();

		public:

		private:
			std::vector < Spectrum* >m_vpSpectra;
			SampleTreeBuilderSampleSet* m_sampleSet;
		};
	}
}

kome::objects::Variant execSampleTreeBuilderCommandline( kome::objects::Parameters* params);
kome::objects::Variant execSampleTreeBuilderBatch( kome::objects::Parameters* );
Spectrum* getSpecByName( DataGroupNode* node, const char *szRequestedName );


#endif    // __KOME_SAMPLE_TREE_BUILDER_FUNCTIONS_H__
