/**
 * @file OpenMS.h
 * @brief interfaces of OpenMS class
 *
 * @author S.Tanaka
 * @date 2015.07.01
 * 
 * Copyright(C) 2015 Mass++ All rights reserved.
 */


#ifndef __KOME_THIRDPARTY_OPENMS_H__
#define __KOME_THIRDPARTY_OPENMS_H__


namespace kome {
	namespace thirdparty {
		namespace openms {
			/**
			 * @class OpenMS
			 * @brief local peak detect class
			 */
			class OpenMS {
			public:
				/**
				 * @fn OpenMS( kome::objects::SettingParameterValues* params );
				 * @brief constructor
				 * @param[in] params parameters
				 */
				OpenMS( kome::objects::SettingParameterValues* params );

				/**
				 * @fn virtual ~OpenMS()
				 * @brief destructor
				 */
				virtual ~OpenMS();

			protected:
				/** parameters */
				kome::objects::SettingParameterValues* m_params;

				/** temporary directory */
				std::string m_tmpDir;

				/**
				 * @struct ParamInfo
				 * @brief parameter information
				 */
				struct ParamInfo {
					std::string section;
					std::string name;
					std::string value;
					std::string type;
				};

				/** parameters */
				std::vector< ParamInfo > m_iniParameters;

			public:
				/**
				 * @fn void wavelet( kome::core::XYData& xyData, kome::objects::Peaks& peaks )
				 * @brief detects peaks by wavelet
				 * @param xyData data points
				 * @param peaks peak object to store peak data
				 */
				void wavelet( kome::core::XYData& xyData, kome::objects::Peaks& peaks );

				/**
				 * @fn void hires( kome::core::XYData& xyData, kome::objects::Peaks& peaks )
				 * @brief detects peaks by high resolution
				 * @param xyData data points
				 * @param peaks peak object to store peak data
				 */
				void hires( kome::core::XYData& xyData, kome::objects::Peaks& peaks );

			protected:
				/**
				 * @fn const char* getTmpDir();
				 * @brief gets the temporary directory
				 * @return temporary directory
				 */
				const char* getTmpDir();

				/**
				 * @fn void addParameter( const char* sec, const char* name, const char* value, const char* type )
				 * @brief adds parameter
				 * @param[in] sec section
				 * @param[in] name parameter name
				 * @param[in] value parameter value
				 * @param[in] type parameter type
				 */
				void addParameter( const char* sec, const char* name, const char* value, const char* type );

				/**
				 * @fn void writeIniFile( const char* path, const char* name )
				 * @brief writes ini file
				 * @param[in] path ini file path
				 * @param[in] name program name
				 */
				void writeIniFile( const char* path, const char* name );

				/**
				 * @fn std::string getCommandPath( const char* commandName );
				 * @brief gets the command path
				 * @param[in] commandName command name
				 * @return command path
				 */
				std::string getCommandPath( const char* commandName );

				/**
				 * @fn void writeDta2d( const char* path, kome::core::XYData& xyData );
				 * @brief writes dta2d file
				 * @param[in] path file path
				 * @param[in] xyData xy data
				 */
				void writeDta2d( const char* path, kome::core::XYData& xyData );

				/**
				 * @fn
				 * @brief reads dta2d file
				 * @param[in] path file path
				 * @param[out] xyData the object to store data
				 */
				void readDta2d( const char* path, kome::core::XYData& xyData );

				/**
				 * @fn bool createProcess( const char* command );
				 * @brief creates process
				 * @param[in] command command line
				 * @return If true, the process was finished successfully.
				 */
				bool createProcess( const char* command );
			};
		}
	}
}


#endif	// __KOME_THIRDPARTY_OPENMS_H__
