#pragma once

namespace pooptube {

	void CalculateNormal( D3DXVECTOR3* pOut, D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2 ); //버그있을 수 있음
	float CalculateAngle( D3DXVECTOR3& src, D3DXVECTOR3& dst );
	float CalculateAngleSignedByY( D3DXVECTOR3& src, D3DXVECTOR3& dst );
}