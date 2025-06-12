/**
 * @file OpenMS.cpp
 * @brief implements of OpenMS class
 *
 * @author S.Tanaka
 * @date 2015.07.01
 * 
 * Copyright(C) 2015 Mass++ All rights reserved.
 */


#include "stdafx.h"
#include "OpenMS.h"

#include <windows.h>


using namespace kome::thirdparty::openms;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
OpenMS::OpenMS( kome::objects::SettingParameterValues* params ) {
	m_params = params;
}

// destructor
OpenMS::~OpenMS() {
}

// wavelet peak detection
void OpenMS::wavelet (
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks
) {
	// tmp dir
	std::string tmpDir = getTmpDir();

	// parameters
	double sn = m_params->getDoubleValue( "sn", 1.0 );
	double width = m_params->getDoubleValue( "width", 0.15 );
	bool estimate = m_params->getBoolValue( "estimate", false );

	addParameter( "", "signal_to_noise", FMT( "%f", sn ).c_str(), "double" );
	addParameter( "", "peak_width", FMT( "%f", width ).c_str(), "double" );
	addParameter( "", "estimate_peak_width", ( estimate ? "true" : "false" ), "string" );

	// files
	std::string dataDta2d = getpath( tmpDir.c_str(), "data.dta2d" );
	std::string dataMzml = getpath( tmpDir.c_str(), "data.mzML" );
	std::string peaksMzml = getpath( tmpDir.c_str(), "peaks.mzML" );
	std::string peaksDta2d = getpath( tmpDir.c_str(), "peaks.dta2d" );
	std::string ini = getpath( tmpDir.c_str(), "params.ini" );

	// run mzml
	writeDta2d( dataDta2d.c_str(), xyData );

	std::string command = getCommandPath( "FileConverter.exe" );
	command = FMT(
		"%s -in \"%s\" -out \"%s\"",
		command.c_str(),
		dataDta2d.c_str(),
		dataMzml.c_str()
	);
	if( !createProcess( command.c_str() ) ) {
		return;
	}

	writeIniFile( ini.c_str(), "PeakPickerWavelet" );
	command = getCommandPath( "PeakPickerWavelet.exe" );
	command = FMT( 
		"%s -in \"%s\" -out \"%s\" -ini \"%s\"",
		command.c_str(),
		dataMzml.c_str(),
		peaksMzml.c_str(),
		ini.c_str()
	);
	if( !createProcess( command.c_str() ) ) {
		return;
	}

	command = getCommandPath( "FileConverter.exe" );
	command = FMT(
		"%s -in \"%s\" -out \"%s\"",
		command.c_str(),
		peaksMzml.c_str(),
		peaksDta2d.c_str()
	);
	if( !createProcess( command.c_str() ) ) {
		return;
	}

	readDta2d( peaksDta2d.c_str(), peaks );
}

// high resolution peak detection
void OpenMS::hires (
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks
) {
	// tmp dir
	std::string tmpDir = getTmpDir();

	// parameters
	double sn = m_params->getDoubleValue( "sn", 1.0 );
	double len = m_params->getDoubleValue( "len", 200.0 );
	int bin = m_params->getIntValue( "cnt", 30 );
	int minElms = m_params->getIntValue( "min", 10 );

	addParameter( "", "signal_to_noise", FMT( "%f", sn ).c_str(), "double" );
	addParameter( "", "ms_levels", "[1,2]", "int" );
	addParameter( "SignalToNoise", "win_len", FMT( "%f", len ).c_str(), "double" );
	addParameter( "SignalToNoise", "bin_count", FMT( "%d", bin ).c_str(), "int" );
	addParameter( "SignalToNoise", "min_required_elements", FMT( "%d", minElms ).c_str(), "int" );

	// files
	std::string dataDta2d = getpath( tmpDir.c_str(), "data.dta2d" );
	std::string dataMzml = getpath( tmpDir.c_str(), "data.mzML" );
	std::string peaksMzml = getpath( tmpDir.c_str(), "peaks.mzML" );
	std::string peaksDta2d = getpath( tmpDir.c_str(), "peaks.dta2d" );
	std::string ini = getpath( tmpDir.c_str(), "params.ini" );

	// run mzml
	writeDta2d( dataDta2d.c_str(), xyData );

	std::string command = getCommandPath( "FileConverter.exe" );
	command = FMT(
		"%s -in \"%s\" -out \"%s\"",
		command.c_str(),
		dataDta2d.c_str(),
		dataMzml.c_str()
	);
	if( !createProcess( command.c_str() ) ) {
		return;
	}

	writeIniFile( ini.c_str(), "PeakPickerHiRes" );
	command = getCommandPath( "PeakPickerHiRes.exe" );
	command = FMT( 
		"%s -in \"%s\" -out \"%s\" -ini \"%s\"",
		command.c_str(),
		dataMzml.c_str(),
		peaksMzml.c_str(),
		ini.c_str()
	);
	if( !createProcess( command.c_str() ) ) {
		return;
	}

	command = getCommandPath( "FileConverter.exe" );
	command = FMT(
		"%s -in \"%s\" -out \"%s\"",
		command.c_str(),
		peaksMzml.c_str(),
		peaksDta2d.c_str()
	);
	if( !createProcess( command.c_str() ) ) {
		return;
	}

	readDta2d( peaksDta2d.c_str(), peaks );
}


// temporary directory
const char* OpenMS::getTmpDir() {
	if( m_tmpDir.empty() ) {
		// open ms folder
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		std::string dir = msppMgr.getTmpDir();
		dir = getpath( dir.c_str(), "openms" );
		if( !fileexists( dir.c_str() ) ) {
			makedirectory( dir.c_str() );
		}
		
		// tmp dir
		m_tmpDir = getpath( dir.c_str(), FMT( "%lld", getcurrenttime() ).c_str() );
		makedirectory( m_tmpDir.c_str() );
	}

	return m_tmpDir.c_str();
}

// add parameter
void OpenMS::addParameter( const char* sec, const char* name, const char* value, const char* type ) {
	m_iniParameters.resize( m_iniParameters.size() + 1 );
	m_iniParameters.back().section = NVL( sec, "" );
	m_iniParameters.back().name = NVL( name, "" );
	m_iniParameters.back().value = NVL( value, "" );
	m_iniParameters.back().type = NVL( type, "" );
}

// write ini file
void OpenMS::writeIniFile( const char* path, const char* name ) {
	// open
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		return;
	}

	// write
	fprintf( fp, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" );
	fprintf( fp, "<PARAMETERS version=\"1.6.2\" xsi:noNamespaceSchemaLocation=\"http://open-ms.sourceforge.net/schemas/Param_1_6_2.xsd\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n" );
	fprintf( fp, "  <NODE name=\"%s\">\n", name );
	fprintf( fp, "    <ITEM name=\"version\" value=\"2.0.0\" type=\"string\" />\n" );
	fprintf( fp, "    <NODE name=\"1\">\n" );
	fprintf( fp, "      <ITEM name=\"in\" value=\"\" type=\"input-file\" />\n" );
	fprintf( fp, "      <ITEM name=\"out\" value=\"\" type=\"output-file\" />\n" );

	fprintf( fp, "      <NODE name=\"algorithm\">\n" );

	std::string currentSec = "";
	for( unsigned int i = 0; i < m_iniParameters.size(); i++ ) {
		std::string s = m_iniParameters[ i ].section;
		std::string n = m_iniParameters[ i ].name;
		std::string v = m_iniParameters[ i ].value;
		std::string t = m_iniParameters[ i ].type;

		std::vector< std::string > values;
		stringtoken( v.c_str(), ", []", values );

		if( currentSec.compare( s ) != 0 ) {
			if( currentSec.empty() ) {
				fprintf( fp, "        <NODE name=\"%s\">\n", s.c_str() );
			}
			else {
				fprintf( fp, "        </NODE>\n" );
			}
			currentSec = s;
		}

		int indent = ( currentSec.empty() ? 8 : 10 );

		for( int j = 0; j < indent; j++ ) {
			fprintf( fp, " " );
		}
		
		if( values.size() > 1 ) {
			fprintf( fp, "<ITEMLIST name=\"%s\" type=\"%s\">\n", n.c_str(), t.c_str() );
			for( unsigned int j = 0; j < values.size(); j++ ) {
				for( int k = 0; k < indent + 2; k++ ) {
					fprintf( fp, " " );
				}
				fprintf( fp, "<LISTITEM value=\"%s\" />\n", values[ i ].c_str() );
			}
			for( int k = 0; k < indent; k++ ) {
				fprintf( fp, " " );
			}
			fprintf( fp, "</ITEMLIST>\n" );
		}
		else {
			fprintf( 
			    fp,
			    "<ITEM name=\"%s\" value=\"%s\" type=\"%s\" />\n",
			    n.c_str(),
				v.c_str(),
				t.c_str()
		    );
		}
	}
	if( !currentSec.empty() ) {
		fprintf( fp, "        </NODE>\n" );
	}
	fprintf( fp, "      </NODE>\n" );
	fprintf( fp, "    </NODE>\n" );
	fprintf( fp, "  </NODE>\n" );
	fprintf( fp, "</PARAMETERS>\n" );
	
	// close
	fflush( fp );
	fclose( fp );
}

// gets the command path
std::string OpenMS::getCommandPath( const char* commandName ) {
	// return value
	std::string path = NVL( commandName, "" );

	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return path;
	}

	std::string dir = trimstring( ini->getString( "OpenMS", "OPENMS_HOME", "" ) );
	if( dir.compare( "none" ) == 0 ) {
		dir = "";
	}
	if( !dir.empty() ) {
		path = getpath( dir.c_str(), path.c_str() );
	}
	path = FMT( "\"%s\"", path.c_str() );

	return path;
}

// write data
void OpenMS::writeDta2d( const char* path, kome::core::XYData& xyData ) {
	// open
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		return;
	}

	// write
	fprintf( fp, "#SEC\tMZ\tINT\n" );

	for( unsigned int i = 0; i < xyData.getLength(); i++ ) {
		fprintf( fp, "%f\t%f\t%f\n", 1.0, xyData.getX( i ), xyData.getY( i ) );
	}
	 
	// close
	fflush( fp );
	fclose( fp );
}


// read data
void OpenMS::readDta2d( const char* path, kome::core::XYData& xyData ) {
	// open
	FILE* fp = fileopen( path, "r" );
	if( fp == NULL ) {
		return;
	}

	// read
	char line[ 4096 ];
	while( fgets( line, 4096, fp ) != NULL ) {
		double rt = 0.0;
		double mz = 0.0;
		double intensity = 0.0;

		if( sscanf( line, "%lf %lf %lf", &rt, &mz, &intensity ) > 0 ) {
			if( rt > 0.0 && mz > 0.0 && intensity > 0.0 ) {
				xyData.addPoint( mz, intensity );
			}
		}
	}

	// close
	fclose( fp );
}

// creates process
bool OpenMS::createProcess( const char* command ) {
	STARTUPINFO si = { sizeof( STARTUPINFO ) };
	PROCESS_INFORMATION pi = {};
	size_t	szLine =strlen( command ) + 1;
	char	*pLine = new char[ szLine ];
	bool bSuccess = false;

	sprintf( pLine, "%s", command );

	if (CreateProcess( NULL, pLine, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, getTmpDir(), &si, &pi))
	{
		// Wait until child process exits.
		DWORD r = WaitForSingleObject( pi.hProcess, INFINITE );
		if( r == WAIT_FAILED ) {
			LOG_ERROR( FMT( "Process Failed" ) );
		}
		else if( r == WAIT_ABANDONED ) {
			LOG_ERROR( FMT( "Process Abandoned" ) );
		}
		else if( r == WAIT_TIMEOUT ) {
			LOG_ERROR( FMT( "Process Timeout" ) );
		}
		else if( r == WAIT_OBJECT_0 ) {
			bSuccess = true;
		}
		else {
			LOG_ERROR( FMT( "Process Error (code=%d)", r ) );
		}
	}
	if  ( (char *)NULL != pLine )
	{
		delete[]	pLine;
	}

	return bSuccess;
}