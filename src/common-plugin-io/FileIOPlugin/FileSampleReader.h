/**
* @file FileSampleReader.h
* @brief File Sample Reader
* @author OKADA, H.
* @date 2012/07/20
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
*/


#ifndef _FILE_SAMPLE_READER_
#define _FILE_SAMPLE_READER_

#include "stdafx.h"

// �t�@�C�����𕡐��w�肷��ۂ̋�؂蕶��
#define STR_FILE_STRINGS_SEPARATOR	"|"	

namespace kome {
	namespace io {

		/** 
		 * @class FileSampleReader
		 * @ brief File Sample Reader class
		 */
		class FileSampleReader : public kome::objects::SampleReader{
			
		public:
			/**
			 * @fn FileSampleReader();
			 * @brief constructor
			 */
			FileSampleReader();
			
			/**
			 * @fn ~FileSampleReader();
			 * @brief destructor
			 */
			virtual ~FileSampleReader();
			
		private:

		protected:

			/**
			 * @fn std::string onSelectKeys( void )
			 * @brief RawDataFile �_�C�A���O���J���t�@�C���p�X��1�ȏ�I������B�e�X�̃L�[���t�@�C���p�X�Ƃ���B
			 */
			std::string onSelectKeys( void );

			/**
			 * @fn kome::objects::Sample* onOpenData( std::string strKey )
			 * @brief �L�[���t�@�C���p�X�Ƃ��ăT���v�����擾����B
			 * @param strKey
			 */
			kome::objects::Sample* onOpenData( std::string strKey );

		};
	}
}


#endif
