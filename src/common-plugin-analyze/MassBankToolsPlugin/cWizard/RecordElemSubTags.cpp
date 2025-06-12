/**
 * @file RecordElemSubTags.cpp
 * @brief AC,MS�����̃T�u�^�O��񓙂��`�E�쐬����
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include <string>
#include <vector>

#include "RecordElemSubTags.h"
#include "objectsName.h"
#include <boost/assign.hpp>

void RecordElemSubTags::initMe()
{
	initSubSpec();
	initSubChrom();
	initSubIon();
	initIonTypes();
}

void RecordElemSubTags::initSubSpec()
{
	specTags_ = 
		boost::assign::list_of
		(k_AC_Spec_ColEnergy)(k_AC_Spec_ColGas)(k_AC_Spec_Date)
		(k_AC_Spec_DesGasFlow)(k_AC_Spec_DesTemp)(k_AC_Spec_IonEnergy)
		(k_AC_Spec_Laser)(k_AC_Spec_Matrix)(k_AC_Spec_MsAcc)
		(k_AC_Spec_RegGas)(k_AC_Spec_Scanning)(kCanInputSelf);

	// SubTag��List�ɓo�^�ł��Ȃ�
	specNGTags_ = boost::assign::list_of
		(kInstrument)(kInstrumentType)(kMSType)(kIonMode);
}

void RecordElemSubTags::initSubChrom()
{
	chromTags_ = 
		boost::assign::list_of
		(k_AC_CH_CapVol)(k_AC_CH_ColName)(k_AC_CH_ColTemp)
		(k_AC_CH_FlowGrad)(k_AC_CH_FlowRate)(k_AC_CH_RetentionTime)
		(k_AC_CH_Solvent)(kCanInputSelf);
	// NG�͂Ȃ�(ver2.7.0)
}

void RecordElemSubTags::initSubIon()
{	
	ionTags_ =  boost::assign::list_of
		(k_MS_BasePeak)(k_MS_DerForm)(k_MS_DerMass)
		(k_MS_DerType)(k_MS_PrecMZ)(kCanInputSelf);

	// SubTag��List�ɓo�^�ł��Ȃ�
	ionNGTags_ =  boost::assign::list_of(kPrecursorType)(kIonType);
}

void RecordElemSubTags::initIonTypes()
{
	IonArr_ = boost::assign::list_of(kNone)
		("[M]+")("[M]+*")("[M+H]+")("[2M+H]+")
		("[M+Na]+")("[M-H+Na]+")("[2M+Na]+")
		("[M+2Na-H]+")("[(M+NH3)+H]+")("[M+H-H2O]+")
		("[M+H-C6H10O4]+")("[M+H-C6H10O5]+")
		("[M]-")("[M-H]-")("[M-2H]-")("[M-2H+H2O]-")
		("[M-H+OH]-")("[2M-H]-")("[M+HCOO-]-")
		("[(M+CH3COOH)-H]-")("[2M-H-CO2]-")("[2M-H-C6H10O5]-");
}
