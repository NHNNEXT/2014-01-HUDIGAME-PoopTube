#include "stdafx.h"
#include "PooMath.h"

namespace pooptube{
	void CalculateNormal( D3DXVECTOR3* pOut, D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2 ) {
		D3DXVECTOR3 n;
		D3DXVECTOR3 A = *v2 - *v0;
		D3DXVECTOR3 B = *v1 - *v0;

		D3DXVec3Cross( &n, &A, &B );
		D3DXVec3Normalize( &n, &n );

		*pOut = n;
	}

	float CalculateAngle( D3DXVECTOR3& src, D3DXVECTOR3& dst )
	{
		D3DXVECTOR3 srcN, dstN;
		D3DXVec3Normalize( &srcN, &src );
		D3DXVec3Normalize( &dstN, &dst );

		float dot = D3DXVec3Dot( &srcN, &dstN );
		float angle = acos( dot );

		// 방향 판단 y축 기준
		D3DXVec3Cross( &dstN, &srcN, &dstN );
		if( dstN.y > 0 )
			angle *= -1; // 시계 방향

		return angle;
	}
}