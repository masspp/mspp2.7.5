/**
 * @file BasicScalingPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author M.Izumi
 * @date 2012.11.05
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "BasicScalingPluginFunctions.h"
#include "BasicScalingPluginManager.h"

#include <math.h>

using namespace kome::scaling;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// UV 
kome::objects::Variant calcUvValue( kome::objects::Parameters* params ){   
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	BasicScalingManager& mgr = BasicScalingManager::getInstance();

	// matrix
	int matrix = kome::plugin::PluginCallTool::getInt( params, "matrix", -1 );
	if( matrix < 0 ) {
		LOG_ERROR_CODE( FMT( "Pareto cannot be calculated because no matrix exists." ), ERR_OTHER );
		return ret;
	}

	// groups
	std::vector< int > groups;
	identMgr.getGroups( groups, matrix );
	if( groups.size() < 0 ) {
		return ret;
	}

	// samples
	std::vector<int>samples;
	for( unsigned int i=0; i < groups.size(); i++ ){
		identMgr.getMatrixSamplesFromGroup( samples, groups[i] );
	}
	
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, matrix );
	if( peaks.size() == 0 ) {
		LOG_ERROR( FMT( "There are no peaks in the matrix." ) );
		return ret;
	}

	// progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	progress->setRange( 0, peaks.size() );
	progress->setPosition( 0 );

	double peak_sd = 0;
	double peak_sd_fix = 0;
	for( unsigned int i=0; i < peaks.size()&& !progress->isStopped(); i++ ){
		int peakId= peaks[ i ];
		std::vector< double > peak_values;

		// �e�s�[�N�ɂ�����A�T���v�����Ƃ̒l���擾
		for( unsigned int j=0; j < samples.size(); j++ ){
			int sampleId = samples[ j ];
			double val = identMgr.getPeakValue( peakId, sampleId );
			peak_values.push_back(val);
		}
		  
		// �W���΍����v�Z
		double av_peak_values = 0.0;
		if( peak_values.size() >= 0.0 ){
			av_peak_values = kome::numeric::Statistics::average( &( peak_values[ 0 ] ), peak_values.size() );
		}

		double sum = 0.0;
		for( unsigned int j=0; j < samples.size(); j++ ){
			int sampleId = samples[ j ];
			double val_for_sum = av_peak_values - identMgr.getPeakValue( peakId, sampleId );
			sum = sum + val_for_sum * val_for_sum;
		}
		peak_sd_fix = sqrt(sum/( samples.size() - 1));      // ���ڂ��Ă���m/z�̃s�[�N�l�̕W���΍�
		

		// �e�s�[�N��pareto�X�P�[�����O
		for( unsigned int j=0; j < samples.size(); j++ ){
			int sampleId = samples[ j ];

			double peak_val = identMgr.getPeakValue( peakId, sampleId );   // �X�P�[�����O����s�[�N�̒l
			double calc = 0.0;
			if (0 < peak_val && 0 < peak_sd_fix){
				calc = mgr.UV( peak_sd_fix, peak_val );
			} 
				
			// �s�[�N���擾
			kome::objects::PeakElement pk;
			identMgr.getPeakInfo(peakId, sampleId, &pk);

			// profile�擾(�s�[�N�g�`�`��ɕK�v)
			int profile = -1;
			profile = identMgr.getProfile(peakId, sampleId);
			
			if ( calc > 0.0 ) {
				identMgr.setPeakValue( peakId, sampleId, calc, &pk, profile );
			} else {
				identMgr.setPeakValue( peakId, sampleId, 0.0, &pk, profile );
			}
		}
		progress->setPosition( i + 1 );
	}
	
	if( !progress->isStopped() ) {
		progress->fill();
	}

	ret.prim.boolVal = true;
	return ret;
}

// pareto 
kome::objects::Variant calcParetoValue( kome::objects::Parameters* params ){   // <Modified> @20140507 by Y.Fujita �{�֐���啝�ɉ���
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	BasicScalingManager& mgr = BasicScalingManager::getInstance();

	// matrix
	int matrix = kome::plugin::PluginCallTool::getInt( params, "matrix", -1 );
	if( matrix < 0 ) {
		LOG_ERROR_CODE( FMT( "Pareto cannot be calculated because no matrix exists." ), ERR_OTHER );
		return ret;
	}

	// groups
	std::vector< int > groups;
	identMgr.getGroups( groups, matrix );
	if( groups.size() < 0 ) {
		return ret;
	}


	// samples
	std::vector<int>samples;
	for( unsigned int i=0; i < groups.size(); i++ ){
		identMgr.getMatrixSamplesFromGroup( samples, groups[i] );
	}
	
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, matrix );
	if( peaks.size() == 0 ) {
		LOG_ERROR( FMT( "There are no peaks in the matrix." ) );
		return ret;
	}

	// progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	progress->setRange( 0, peaks.size() );
	progress->setPosition( 0 );

	double peak_sd = 0;
	double peak_sd_fix = 0;
	for( unsigned int i=0; i < peaks.size()&& !progress->isStopped(); i++ ){   
		int peakId= peaks[ i ];												

		std::vector< double > peak_values;

		// �e�s�[�N�ɂ�����A�T���v�����Ƃ̒l���擾
		for( unsigned int j=0; j < samples.size(); j++ ){
			int sampleId = samples[ j ];
			double val = identMgr.getPeakValue( peakId, sampleId );
			peak_values.push_back(val);
		}

		// �W���΍����v�Z
		double av_peak_values = 0.0;
		if( peak_values.size() >= 0.0 ){
			av_peak_values = kome::numeric::Statistics::average( &( peak_values[ 0 ] ), peak_values.size() );
		}

		double sum = 0.0;
		for( unsigned int j=0; j < samples.size(); j++ ){
			int sampleId = samples[ j ];
			double val_for_sum = av_peak_values - identMgr.getPeakValue( peakId, sampleId );
			sum = sum + val_for_sum * val_for_sum;
		}
		peak_sd_fix = sqrt(sum/( samples.size() - 1));       // ���ڂ��Ă���m/z�̃s�[�N�l�̕W���΍�
		

		// �e�s�[�N��pareto�X�P�[�����O
		for( unsigned int j=0; j < samples.size(); j++ ){
			int sampleId = samples[ j ];

			double peak_val = identMgr.getPeakValue( peakId, sampleId );    // �X�P�[�����O����s�[�N�̒l
			double calc = 0.0;
			if (0 < peak_val && 0 < peak_sd_fix){
				calc = mgr.pareto( peak_sd_fix, peak_val );
			}

			// �s�[�N���擾
			kome::objects::PeakElement pk;
			identMgr.getPeakInfo(peakId, sampleId, &pk);

			// profile�擾(�s�[�N�g�`�`��ɕK�v)
			int profile = -1;
			profile = identMgr.getProfile(peakId, sampleId);
		
			
			if(  calc > 0.0 ){
				identMgr.setPeakValue( peakId, sampleId, calc, &pk, profile );
			} else {
				identMgr.setPeakValue( peakId, sampleId, 0.0, &pk, profile );
			}
			
		}
		progress->setPosition( i + 1 );
	}
	
	if( !progress->isStopped() ) {
		progress->fill();
	}

	ret.prim.boolVal = true;
	return ret;
}

// log10 
kome::objects::Variant calcLogValue( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	BasicScalingManager& mgr = BasicScalingManager::getInstance();

	// matrix
	int matrix = kome::plugin::PluginCallTool::getInt( params, "matrix", -1 );
	if( matrix < 0 ) {
		LOG_ERROR_CODE( FMT( "log cannot be calculated because no matrix exists." ), ERR_OTHER );
		return ret;
	}
	// groups
	std::vector< int > groups;
	identMgr.getGroups( groups, matrix );
	if( groups.size() < 0 ) {
		return ret;
	}

	// samples
	std::vector<int>samples;
	for( unsigned int i=0; i < groups.size(); i++ ){
		identMgr.getMatrixSamplesFromGroup( samples, groups[i] );
	}
	
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, matrix );
	if( peaks.size() == 0 ) {
		LOG_ERROR( FMT( "There are no peaks in the matrix." ) );
		return ret;
	}

	// progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	progress->setRange( 0, peaks.size() );
	progress->setPosition( 0 );

	for( unsigned int i=0; i < peaks.size()&& !progress->isStopped(); i++ ){
		int peakId = peaks[ i ];

		for( unsigned int j=0; j < samples.size(); j++ ){
			int sampleId = samples[ j ];

			double p = identMgr.getPeakValue( peakId, sampleId );

			// �s�[�N���擾
			kome::objects::PeakElement pk;
			identMgr.getPeakInfo(peakId, sampleId, &pk);

			// profile�擾(�s�[�N�g�`�`��ɕK�v)
			int profile = -1;
			profile = identMgr.getProfile(peakId, sampleId);

			if(  p > 0.0 ){
				double calc = mgr.log( p );
				if ( calc > 0.0 ) {
					identMgr.setPeakValue( peakId, sampleId, calc, &pk, profile );
				} else {                                                          // Mass++�̃s�[�N�}�g���N�X�̗v�f��0�ȏ�̒l��z�肵�Ă���B
					identMgr.setPeakValue( peakId, sampleId, 0.0, &pk, profile ); // �s�[�N�l��log�v�Z����ƁA�ꍇ�ɂ���Ă�0�����̒l�������邪�A
				}                                                                 // ���̎��̓s�[�N�l�Ƃ���0������B
			}else{      // ���̒l��(p > 0.0)�ȊO�̒l�Ȃ�A0������
				identMgr.setPeakValue( peakId, sampleId, 0.0, &pk, profile );  
			}
		}

		progress->setPosition( i + 1 );
	}

	if( !progress->isStopped() ) {
		progress->fill();
	}


	ret.prim.boolVal = true;
	return ret;
}

// ln
kome::objects::Variant calcLnValue( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;
	
	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	BasicScalingManager& mgr = BasicScalingManager::getInstance();

	// matrix
	int matrix = kome::plugin::PluginCallTool::getInt( params, "matrix", -1 );
	if( matrix < 0 ) {
		LOG_ERROR_CODE( FMT( "ln cannot be calculated because no matrix exists." ), ERR_OTHER );
		return ret;
	}
	// groups
	std::vector< int > groups;
	identMgr.getGroups( groups, matrix );
	if( groups.size() < 0 ) {
		return ret;
	}

	// samples
	std::vector<int>samples;
	for( unsigned int i=0; i < groups.size(); i++ ){
		identMgr.getMatrixSamplesFromGroup( samples, groups[i] );
	}
	
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, matrix );
	if( peaks.size() == 0 ) {
		LOG_ERROR( FMT( "There are no peaks in the matrix." ) );
		return ret;
	}

	// progress
	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	progress->setRange( 0, peaks.size() );
	progress->setPosition( 0 );

	for( unsigned int i=0; i < peaks.size()&& !progress->isStopped(); i++ ){
		int peakId = peaks[ i ];

		for( unsigned int j=0; j < samples.size(); j++ ){
			int sampleId = samples[ j ];

			double p = identMgr.getPeakValue( peakId, sampleId );

			// �s�[�N���擾
			kome::objects::PeakElement pk;
			identMgr.getPeakInfo(peakId, sampleId, &pk);

			// profile�擾(�s�[�N�g�`�`��ɕK�v)
			int profile = -1;
			profile = identMgr.getProfile(peakId, sampleId);
			
			if( p > 0.0 ){
				double calc = log(p);       
				if (calc > 0.0 ) {
					identMgr.setPeakValue( peakId, sampleId, calc, &pk, profile );
				} else {                                                            // Mass++�̃s�[�N�}�g���N�X�̗v�f��0�ȏ�̒l��z�肵�Ă���
					identMgr.setPeakValue( peakId, sampleId, 0.0, &pk, profile );   // �s�[�N�l��ln�v�Z����ƁA�ꍇ�ɂ���Ă�0�����̒l�������邪�A
				}                                                                   // ���̎��̓s�[�N�l�Ƃ���0������B
			} else {     // ���̒l��(p > 0.0)�ȊO�̒l�Ȃ�A0������
				identMgr.setPeakValue( peakId, sampleId, 0.0, &pk, profile );    
			}

		}

		progress->setPosition( i + 1 );
	}

	if( !progress->isStopped() ) {
		progress->fill();
			
	}

	ret.prim.boolVal = true;
	return ret;
}
