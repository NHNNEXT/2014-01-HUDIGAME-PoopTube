#include "stdafx.h"

namespace pooptube {

	//버그있을 수 있음
	void CalculateNormal(D3DXVECTOR3* pOut, D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2) {
		D3DXVECTOR3 n;
		D3DXVECTOR3 A = *v2 - *v0;
		D3DXVECTOR3 B = *v1 - *v0;

		D3DXVec3Cross(&n, &A, &B);
		D3DXVec3Normalize(&n, &n);

		*pOut = n;
	}

}