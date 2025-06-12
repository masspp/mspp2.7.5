/**********************************************************/
/*! @addtogroup MRM3DPlugin 
    @file       MRMChromatograms.h
    @brief      MRM Chromarogram Objects Class
    @date       2012.10.24
	@author     M.Fukuda
	@version    1.0.0
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
 **********************************************************
2012.10.24
MRM3D �Ǝ���Chromatogram�Ǘ��N���X
*/
/**********************************************************/
#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <boost/function.hpp>

namespace kome{
namespace mrm3d{

/*!
 * @breif struct RT-Intensity
 */
struct RTInt {
	double RT;
	double Intensity;
	RTInt(const double& _RT, const double& _int)
		: RT(_RT), Intensity(_int){}
};

// Filter�̌^��MRM�ł��邩�ۂ����`�F�b�N����N���X
// static Function�ɂ������Ȃ������̂ŃN���X�ɂ����B���ꂾ���B
class MRMType {
public:
	MRMType(){}
	~MRMType(){}
	const bool checkMRMType(const std::string& filterStr);
private:
	// copy�̗ދ֎~�E�h�~
	MRMType(const MRMType& m) {}
	MRMType& operator=(const MRMType& rhs){}
};

// double�͍����o��̂ŁA������z��������
inline const bool isEqual(const double& a, const double& b) {
	return fabs(a-b) < 1.0E-6;
}

class MRMChrom {
private:
	const std::string q1word;
	const std::string q3word;
	double q1, q3, q3f, q3e;
	std::vector<RTInt*> dataPoints;
	boost::function<void(const unsigned int)> reserveFunc;
	boost::function<void(const double&, const double&)> addFunc;
public:
	// constructor
	MRMChrom(const std::string& _q1, const std::string& _q3);
	// destructor
	~MRMChrom();
private:
	// Copy �֎~
	MRMChrom(const MRMChrom& m){}
	MRMChrom& operator=(const MRMChrom& rhs){}
public:
	// getter
	double getQ1() const { return q1; }
	double getQ3() const { return q3; }
	std::tuple<double, double, double> getQ3s() const {
		return std::make_tuple(q3f,q3,q3e);
	}
	// setter
	void setPoints(const double& rt, const double& intensity);
	void addLastPoints(const double& intensity);

	// Judges
	const int isInRange(const double& tgt);

	// Addition out of src
	template <typename RFunc> void setReserveFunc(RFunc f) {reserveFunc = f;}
	template <typename AFunc> void setAddFunc(AFunc f) {addFunc = f;}

	// sort my Points
	void sortDataPoints();

	// Assign XYDatas
	void doAssignXYData();

private:
	// init
	void SetInit();
};

//--------------------------------------------------------------------------
//
// MRMChrom ��Filter�����P�ʂŊǗ�����B
// MRMChrom�@���̂�Q3�̑����ŕ����ς݂ł���B
//
//--------------------------------------------------------------------------
class MRMChromKeeper {
private:
	typedef std::shared_ptr<MRMChrom> tMRMChromPtr;
	typedef std::vector<tMRMChromPtr> tMRMVCT;

	std::string myFilter_;    /*! ���g�̃t�B���^�� */
	tMRMVCT chroms_;          /*! �t�B���^������ɂȂ�MRMChrom�i�q��) */
	unsigned int chromsIdx_;  /*! �q���̌������̃C���f�b�N�X */
	bool overRange_;          /*! ����ȏ���MZ�Ɏq�����Ȃ��ꍇ��Ture */

public:
	explicit MRMChromKeeper(const std::string& myFilter);
	~MRMChromKeeper();
private:
	// Copy �֎~
	MRMChromKeeper(const MRMChromKeeper& m){}
	MRMChromKeeper& operator=(const MRMChromKeeper& rhs){}

public:
	//
	const bool doInit();

	void addPointsBase(const double& rt);
	void findAndsetPoints(const double& mz, const double& intensity);
	void margeToAll(tMRMVCT& allChroms);

private:
	const bool onSetInit();
	void sortMyChromsByQ3();
	const bool canAddPoints() const;
};
	//// SortOperator
	//struct q3_sorter : 
	//	public std::binary_function<tMRMChromPtr, tMRMChromPtr, bool> {
	//public:
	//	bool operator ()(const tMRMChromPtr& a, const tMRMChromPtr& b) const {
	//		return ( a->getQ3() < b->getQ3() );
	//	};
	//};

} // end of namespace mrm3d
} // end of namespace kome
