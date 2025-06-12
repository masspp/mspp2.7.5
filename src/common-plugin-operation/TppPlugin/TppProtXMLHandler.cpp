/**
* @file TppProtXMLHandler.cpp
* @brief implements of TppProtXMLHandler class
*
* @author N.Tashiro
* @date 2013.03.15
* 
* Copyright(C) 2013 Shimadzu. All rights reserved.
*/

#include "stdafx.h"
#include "TppProtXMLHandler.h"

using namespace kome::Tpp;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::xml;

TppProtXMLHandler::TppProtXMLHandler(T_prot *p){
	m_prot = p;
	InitMember();	
}

TppProtXMLHandler::~TppProtXMLHandler(){

}

void TppProtXMLHandler::onStartDocument(){
	InitMember();
}

void TppProtXMLHandler::onStartElement(const char* name, kome::core::Properties& attrs){

	m_nCurrentTagLayer++;		

	int protein_group_index = m_prot->sizeProteinGroup() - 1;
	int protein_index = protein_group_index < 0 ? -1 : m_prot->ProteinGroup(protein_group_index).sizeProtein() - 1;
	int peptide_index = protein_index  < 0 ? -1 : m_prot->ProteinGroup(protein_group_index).Protein(protein_index).sizePeptide() - 1;
	int indistinguishable_protein_index = protein_index  < 0 ? -1 : m_prot->ProteinGroup(protein_group_index).Protein(protein_index).sizeIndistinguishableProteins() - 1;


	// protein_group�^�O
	if (strcmp (TAG_protein_group, name) == 0)
	{
		T_protein_group protein_group;

		protein_group.setTagLayer (m_nCurrentTagLayer);
		protein_group.setTagName(name);
		protein_group.setAttr(attrs);
		m_prot->insertProteinGroup (protein_group);
	}
	// protein�^�O
	if (strcmp (TAG_protein, name) == 0 && protein_group_index >= 0)	// �v���e�C���O���[�v���P�ȏ゠��Ƃ�
	{
		T_protein protein;
		protein.setTagLayer (m_nCurrentTagLayer);
		protein.setTagName(name);
		protein.setAttr(attrs);

		// protein_group�^�O�̂ЂƂ��̊K�w�ł��邩�H
		if (protein.getTagLayer() == m_prot->ProteinGroup(protein_group_index).getTagLayer() + 1 )
			m_prot->ProteinGroup(protein_group_index).insertProtein(protein);
	}
	// peptide�^�O
	if (strcmp (TAG_peptide, name) == 0 && protein_index >= 0)		// �v���e�C���O���[�v���P�ȏ゠��Ƃ�
	{
		T_peptide peptide;
		peptide.setTagLayer (m_nCurrentTagLayer);
		peptide.setTagName(name);
		peptide.setAttr(attrs);

		// protein_group�^�O�̂ЂƂ��̊K�w�ł��邩�H
		if (peptide.getTagLayer() == m_prot->ProteinGroup(protein_group_index).Protein(protein_index).getTagLayer() + 1)
			m_prot->ProteinGroup(protein_group_index).Protein(protein_index).insertPeptide (peptide);
	}

	//�@indistinguishable�^�O
	if (strcmp (TAG_indistinguishable_protein, name) == 0 && protein_index >= 0)	// protein���P�ȏ゠��Ƃ�
	{
		T_indistinguishable_protein indistinguishable;

		indistinguishable.setTagLayer (m_nCurrentTagLayer);
		indistinguishable.setTagName(name);
		indistinguishable.setAttr(attrs);

		// protein�^�O�̈���̊K�w�ł��邩�H
		if (indistinguishable.getTagLayer() == m_prot->ProteinGroup(protein_group_index).Protein(protein_index).getTagLayer() + 1)
			m_prot->ProteinGroup(protein_group_index).Protein(protein_index).insertIndistinguishableProteins (indistinguishable);
	}
	//�@parameter�^�O
	if (strcmp (TAG_parameter, name) == 0 && protein_index >= 0)		
	{
		T_parameter parameter;

		parameter.setTagLayer (m_nCurrentTagLayer);
		parameter.setTagName(name);
		parameter.setAttr(attrs);

		// protein�^�O�̈���̊K�w�ł��邩�H
		if (parameter.getTagLayer() == m_prot->ProteinGroup(protein_group_index).Protein(protein_index).getTagLayer() + 1)
			m_prot->ProteinGroup(protein_group_index).Protein(protein_index).insertParameter(parameter);
	}
	// annotation�^�O
	if (strcmp (TAG_annotation, name) == 0 	&&	protein_index >= 0)	// protein���P�ȏ゠��Ƃ�
	{
		T_annotation annotation;
		annotation.setTagLayer (m_nCurrentTagLayer);
		annotation.setTagName(name);
		annotation.setAttr(attrs);

		// protein�^�O�̈���̊K�w�ł��邩�H
		if (annotation.getTagLayer() == m_prot->ProteinGroup(protein_group_index).Protein(protein_index).getTagLayer() + 1)
			m_prot->ProteinGroup(protein_group_index).Protein(protein_index).insertAnnotation (annotation);

		// indistinguishable_protein�^�O�̈���̊K�w�ł��邩�H				
		if (indistinguishable_protein_index >= 0
			&&	annotation.getTagLayer() == 
			m_prot->ProteinGroup(protein_group_index).Protein(protein_index).IndistinguishableProteins(
			indistinguishable_protein_index ).getTagLayer() + 1)
		{
			m_prot->ProteinGroup(protein_group_index).Protein(protein_index).IndistinguishableProteins(
				indistinguishable_protein_index ).insertAnnotation(annotation);
		}
	}
	// peptide_parent_protein�^�O
	if (strcmp (TAG_peptide_parent_protein, name) == 0 && peptide_index >= 0)	// peptide���P�ȏ゠��Ƃ�
	{
		T_peptide_parent_protein peptide_parent_protein;
		peptide_parent_protein.setTagLayer (m_nCurrentTagLayer);
		peptide_parent_protein.setTagName(name);
		peptide_parent_protein.setAttr(attrs);

		// peptide�^�O�̈���̊K�w�ł��邩�H
		if (peptide_parent_protein.getTagLayer() == m_prot->ProteinGroup(protein_group_index).Protein(protein_index).getTagLayer() + 1)	
			m_prot->ProteinGroup(protein_group_index).Protein(protein_index).Peptide(peptide_index).insertPeptideParentProtein (peptide_parent_protein);
	}


}

void TppProtXMLHandler::onEndDocument(){

}

void TppProtXMLHandler::onEndElement(const char* name, const char* text){
	m_nCurrentTagLayer--;
}

