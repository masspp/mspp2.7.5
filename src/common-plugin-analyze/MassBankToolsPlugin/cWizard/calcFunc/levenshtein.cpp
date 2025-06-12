/**
 * @file levenshtein.cpp
 * @brief Levenshtein distance
 *
 * @author M.Fukuda
 * @date 2013.02.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "levenshtein.h"
#include <vector>

namespace ext {

// �Ίp�������݂���̂ňړ�
const int onSnake(
	const int k,
	int y,
	const int nX, 
	const int nY, 
	const std::string& sX, 
	const std::string& sY
) {
	int x = y - k;
	while( x < nX && y < nY && sX[x] == sY[y]) {
		x++;
		y++;
	}
	return y;
}

// O(ND) algorithm By (Wu)
// �Q�l�FAn O(NP) Sequence Comparison Algorith
// sX < sY�@�Ŕ�����s��
// ���ق̓x����_���ŕԂ��ׁA�O�Ȃ犮�S��v�ł���B
const int levenshtein(
	const std::string& sX, 
	const std::string& sY
) {
	const int nX = static_cast<int>(sX.size());
	const int nY = static_cast<int>(sY.size());

	// �O��Ƌt�Ȃ̂œ���ւ��ď����B
	if (nX > nY) return levenshtein(sY, sX);

	const int nD = nY - nX;
	// �Е����O�Ȃ̂Ł@�قȂ鋗�������̂܂�
	if (nX == 0 || nY == 0) return nD;

	const int nO = nX + 1;
	const int nOD = nD + nO;

	std::vector<int> fp((nX + nY + 3), -1);
	int p = -1;
	register int k;

	while(fp[nOD] != nY) {
		p = p++;
		for (k = -p; k < nD; k++) {
			fp[k + nO] = onSnake(k, (std::max)(fp[k + nO - 1] + 1, fp[k + nO + 1]), nX, nY, sX, sY);
		}
		for (k = nD + p; k > nD; k--) {
			fp[k + nO] = onSnake(k, (std::max)(fp[k + nO - 1] + 1, fp[k + nO + 1]), nX, nY, sX, sY);
		}
		fp[nOD] = onSnake(nD, (std::max)(fp[nOD - 1] + 1, fp[nOD + 1]), nX, nY, sX, sY);
	}
	return nD + 2 * p;
}

} // end of namespace
