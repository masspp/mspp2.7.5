/**
 * @file templ
 * @brief Masspp Manager���璼�� ini�l�̂���������ۂɎg���B
 *
 * @author M.Fukuda
 * @date 2013.03.12
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <string>

// -- Forward declaration --
namespace kome {
	namespace core {
		class IniFile;
	}
}

class ParamIniAcc
{
public:
	ParamIniAcc(const std::string& _section, const std::string& _paramKey);
private:
	kome::core::IniFile* mparam;
	const std::string& section;
	const std::string& paramKey;
public:
	template<typename T> T getParam(void) const;
	template<typename U> void setParam(const U& u);
};

// ���ꉻ����Ă��镔���̎������� CPP���ɂ���