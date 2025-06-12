/**
 * @file ErrorCheck
 * @brief �x���̕\�����܂Ƃ߂�����
 *
 * @author M.Fukuda
 * @date 2013.03.12
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "calcFunc\StrFunc.h"
#include "ErrorCheck.h"
#include "rel_stdafx.h"

// same as k_help_prefix
const std::string kErrPrefix = 
	"consists of two or three alphabetical capital characters.";

const std::string kErrOnlyApl = 
	"Prefix field only accepts alphanumeric characters.\n";

const std::string kErrOver8words = 
	"The total length of the [Prefix] and [Base Num] fields should be at most 8 characters.\n";

const std::string kErrOverTotal8words = 
	"The total length of [Prefix] + [Last Spectrum Number (Base Num + Number of selected spectra - 1)]\
must not exceed 8 characters.\nThe last spectrum record's Accession : ";

const std::string kErrFolderDNWrite = 
	"Cannot save to the specified folder. Select a different folder.";

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace CHK {

// �I������Ă���X�y�N�g����������
const bool showErrorNoSpecSelect() {
	std::string emsg = "No spectra selected.";
	kome::window::WindowTool::showInfo(emsg.c_str(), "Input Error");
	return false;
}

const bool showErrorNoSpecSelectAndAveraged() 
{
	std::string emsg = "No spectra selected.\nNo Average spectra.";
	kome::window::WindowTool::showInfo(emsg.c_str(), "Input Error");
	return false;
}

// input Error Dialog
const bool showInfoDialog(const std::string& infoMsg) {
	kome::window::WindowTool::showInfo(infoMsg.c_str(), "Deny Input");
	return false;
}


// input Error Dialog (TRE212-4046�����̓G���[��info�ɂ���j
const bool showInputErrorDialog(const std::string& errMsg) {
	kome::window::WindowTool::showInfo(errMsg.c_str(), "Input Error");
	return false;
}

// check Prefix
const bool checkPrefix(const std::string& prefix, const bool showErrFg) {
	const unsigned int prefixsize = prefix.size();
	if (2 <= prefixsize && prefixsize <= 3) return true;
	if(showErrFg) {
		showInputErrorDialog(kErrPrefix);
	}
	return false;
}

// check Input Prefix
const bool checkInputedPrefix(const std::string& prefix) {
	if (prefix.empty()) {return true;}
	std::string refStr = prefix;
	if (refStr.size() > 3) {
		return showInputErrorDialog(kErrPrefix);
	}
	if(!ext::isAlphabet(prefix)) {
		return showInputErrorDialog(kErrOnlyApl);
	}
	return true;
}

// check BaseNum
const bool checkBaseNum_s(
	const std::string& preval, 
	const std::string& basenum, 
	const unsigned int futurecount
) {
	return checkBaseNum(preval, ext::toInt(basenum),futurecount);
}

// check BaseNum
const bool checkBaseNum(
	const std::string& preval, 
	const int basenum, 
	const unsigned int futurecount
) {
	if (preval.empty() || basenum < 0) return false;
	if (checkBaseNum(preval, basenum)) {
		if (checkBaseNum(preval, basenum + futurecount)) {
			return true;
		} else {
			std::string emsg = kErrOverTotal8words + FMT("%s%d will be too long.", preval.c_str(),(basenum + futurecount));
			return showInputErrorDialog(emsg);
		}
	} else {
		return showInputErrorDialog(kErrOver8words);
	}
}

// check BaseNum
const bool checkBaseNum(const std::string& preval, const int basenum) {
	std::string bas = ext::toStr(basenum);
	unsigned int accLen = preval.length() + bas.length();
	return (accLen <= 8);
}

#include <io.h>


const bool checkSaveFolderPath(const std::string& folderpath) 
{
	bool ret = isdirectory(folderpath.c_str());
	if (ret) {
		// Accession�{.txt�Ɠ����̕�����̉��t�@�C��������������B
		// MSPP + hoge + TEST �̃A�i�O�����B�ɗ͖��Ӗ��i�ق��Ŏg���Ȃ��j�ɂ����B
		const char* dmy = "MhTSoEPgSPeT";
		std::string dmyFileName = getpath(folderpath.c_str(), dmy);
		int fth;
		if(_sopen_s(&fth, dmyFileName.c_str(), _O_CREAT| _O_TEMPORARY,_SH_DENYNO,_S_IWRITE) == 0) {
			_close(fth);
		} else {
			// Target Dir Failed......
			showInputErrorDialog(kErrFolderDNWrite);
			return false;
		}
	}
	return ret;
}

// subtag�̐擪�������؂�o���B
const bool SplitTopParams(
	const std::string& src, 
	std::vector<std::string>& vals
) {
	if (src.empty()) return false;

	std::vector<std::string> lineTokens, innerTokens;
	bool ret = true;
	std::string fstStr,sndStr;

	using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


	kome::plugin::SettingsValue::separateListValue(src.c_str(), lineTokens);
	
	std::for_each(lineTokens.begin(), lineTokens.end(), [&](const std::string& tmp) {
		innerTokens.clear();
		kome::plugin::SettingsValue::separateListValue(tmp.c_str(), innerTokens);
		if (!innerTokens.empty()) {
			fstStr = kome::plugin::SettingsValue::convertToDisplayingString(innerTokens[0].c_str());
			if (!fstStr.empty()) {
				//�A���_�[�o�[�Ȃǂ̋K����ϊ�
				fstStr = ext::trancetoMassBankTag(fstStr);
				vals.push_back(fstStr);
			}
		} else {
			//get Empty. so false.
			ret = false;
		}
	});
	return ret;
}

// �v�f���̏d�����`�F�b�N����
const bool isOverlap(
	std::vector<std::string> vals, 
	const int etype,
	bool showMsg) 
{
	// �d�����p��
	if (vals.size() <= 1) return false;

	std::sort(vals.begin(), vals.end());
	std::vector<std::string>::iterator it1,it2, eit;
	it1 = vals.begin();
	eit = vals.end();
	for(it2 = it1 + 1; it2 != eit; ++it1, ++it2) {
		if(*it2 == *it1) {
			// �d����������
			if(showMsg) {
				std::string emsg("");
				if (etype == 0) {
					emsg = FMT("Can not use this subtag. : %s is Overlap.", it1->c_str());
				} else if(etype == 1) {
					emsg = FMT("Must be a unique External DB. : %s is Overlap.", it1->c_str());
				}
				showInputErrorDialog(emsg);
			}
			return true;
		}
	}
	return false;
}


// SubTag�̃`�F�b�N���s��
const bool checkSubTags(
	const std::string& src, 
	const std::vector<std::string>& ngVals,
	bool showMsg
	) {
	
	typedef std::vector<std::string> vSTR;
	typedef std::vector<std::string>::const_iterator vsIt;

	std::vector<std::string> subtagArr;
	if (SplitTopParams(src, subtagArr)) {

		if (isOverlap(subtagArr,1,showMsg)) {
			return false; // �d������
		}

		// SubTag�̗v�f������������ꍇ�B
		for (vsIt n_it = ngVals.begin(),n_eit = ngVals.end(); n_it != n_eit; ++n_it) {
			std::string ngWord = ext::trancetoMassBankTag(*n_it);
			for(vsIt s_it = subtagArr.begin(), s_eit = subtagArr.end(); s_it != s_eit; ++s_it) {
				if (ngWord.compare(*s_it) == 0) {
					// NG Word Match.
					if(showMsg) {
						std::string emsg = FMT("Can not use this subtag. : %s", n_it->c_str());
						showInputErrorDialog(emsg);
						return false;
					}
				}
			}
		}
	} else {
		// Subtag�̗v�f���������s���ȏꍇ�B
		// �����ꍇ�� Empty.
		if(!subtagArr.empty()) {
			//�����łȂ���΃p�[�X���ɃG���[���������B
			return false;
		}
	}
	return true;
}


} // end of namespace
