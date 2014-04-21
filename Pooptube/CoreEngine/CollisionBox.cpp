#include "stdafx.h"
#include "Application.h"
#include "CollisionBox.h"
#include "CollisionManager.h"

namespace pooptube {
	CollisionBox::CollisionBox()
		: mCollisionType( COLLISION_TYPE::COLLISION_NONE )
		, mBound( 0.0f )
		, mMass( 1.0f )
	{
		// 다른 방법 없나...
		mAxisDir[0] = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		mAxisDir[1] = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		mAxisDir[2] = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		mAxisLen[0] = 0.5f;
		mAxisLen[1] = 0.5f;
		mAxisLen[2] = 0.5f;
	}

	CollisionBox::CollisionBox( COLLISION_TYPE collisionType, float bound, float mass )
		: mCollisionType( collisionType )
		, mBound( bound )
		, mMass( mass )
	{
		mAxisDir[0] = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		mAxisDir[1] = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		mAxisDir[2] = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		mAxisLen[0] = 0.5f;
		mAxisLen[1] = 0.5f;
		mAxisLen[2] = 0.5f;
	}

	CollisionBox::~CollisionBox()
	{
	}

	CollisionBox* CollisionBox::Create()
	{
		return Create( COLLISION_TYPE::COLLISION_NONE, 0.0f, 1.0f );
	}
	CollisionBox* CollisionBox::Create( COLLISION_TYPE collisionType, float bound, float mass )
	{
		CollisionBox* pCollisionBox = new CollisionBox;

		if( pCollisionBox->Init() ) {
			//여기서 두곳 다 넣으니 뻑이나네요
			//ObjectManager::GetInstance()->AddObject( pCollisionBox );
			CollisionManager::GetInstance()->AddCollisionBox( pCollisionBox );
		}
		else {
			delete pCollisionBox;
			pCollisionBox = nullptr;
		}
		return pCollisionBox;
	}

	void CollisionBox::Render()
	{
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		D3DXMATRIX projMat, viewMat;
		pDevice->GetTransform( D3DTS_PROJECTION, &projMat );
		pDevice->GetTransform( D3DTS_VIEW, &viewMat );
		viewMat *= projMat;

		ID3DXLine *Line;

		if( D3DXCreateLine( pDevice, &Line ) != D3D_OK )
			return;
		Line->SetWidth( 1 );
		Line->SetAntialias( true );

		D3DXVECTOR3 vF[3];
		for( int i = 0; i < 3; ++i ){
			vF[i] = mAxisDir[i] * mAxisLen[i];
		}

		D3DXVECTOR3 point[11];
		point[0] = mCenterPos - vF[0] + vF[1] + vF[2];
		point[1] = mCenterPos + vF[0] + vF[1] + vF[2];
		point[2] = mCenterPos + vF[0] + vF[1] - vF[2];
		point[3] = mCenterPos - vF[0] + vF[1] - vF[2];
		point[4] = mCenterPos - vF[0] + vF[1] + vF[2];
		point[5] = mCenterPos - vF[0] - vF[1] + vF[2];
		point[6] = mCenterPos + vF[0] - vF[1] + vF[2];
		point[7] = mCenterPos + vF[0] - vF[1] - vF[2];
		point[8] = mCenterPos - vF[0] - vF[1] - vF[2];
		point[9] = mCenterPos - vF[0] - vF[1] + vF[2];
		Line->Begin();
		Line->DrawTransform( point, 10, &viewMat, D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ) );

		D3DXVECTOR3 pointT[2];
		pointT[0] = mCenterPos - vF[0] - vF[1] - vF[2];
		pointT[1] = mCenterPos - vF[0] + vF[1] - vF[2];
		Line->DrawTransform( pointT, 2, &viewMat, D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ) );

		pointT[0] = mCenterPos + vF[0] - vF[1] - vF[2];
		pointT[1] = mCenterPos + vF[0] + vF[1] - vF[2];
		Line->DrawTransform( pointT, 2, &viewMat, D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ) );

		pointT[0] = mCenterPos + vF[0] - vF[1] + vF[2];
		pointT[1] = mCenterPos + vF[0] + vF[1] + vF[2];
		Line->DrawTransform( pointT, 2, &viewMat, D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ) );
		Line->End();

		Line->Release();
	}

	void CollisionBox::Update( float dTime )
	{
		Node::Update( dTime );

		D3DXMATRIXA16 mMatrix = GetMatrix();
		mCenterPos = D3DXVECTOR3( mMatrix._41, mMatrix._42, mMatrix._43 );
		mMatrix._41 = 0;
		mMatrix._42 = 0;
		mMatrix._43 = 0;

		D3DXMATRIXA16 temp;
		D3DXMatrixIdentity( &temp );
		temp._41 = 1;
		D3DXMatrixMultiply( &temp, &temp, &mMatrix );
		mAxisDir[0] = D3DXVECTOR3( temp._41, temp._42, temp._43 );
		D3DXMatrixIdentity( &temp );
		temp._42 = 1;
		D3DXMatrixMultiply( &temp, &temp, &mMatrix );
		SetAxisDir( mAxisDir[0], D3DXVECTOR3( temp._41, temp._42, temp._43 ) );
	}

	bool CollisionBox::CollisionCheck( const CollisionBox* target )
	{
		D3DXVECTOR3 D = mCenterPos - target->mCenterPos;

		float C[3][3];    //matrix C=A^T B,c_{ij}=Dot(A_i,B_j)
		float absC[3][3]; //|c_{ij}|
		float AD[3];      //Dot(A_i,D)
		float R0, R1, R;    //interval radii and distance between centers
		float R01;        //=R0+R1

		//A0~2
		for( int j = 0; j < 3; ++j ){
			for( int i = 0; i < 3; ++i ){
				C[j][i] = D3DXVec3Dot( &target->mAxisDir[j], &mAxisDir[i] );
				absC[j][i] = abs( C[j][i] );
			}
			AD[j] = D3DXVec3Dot( &target->mAxisDir[j], &D );
			R = abs( AD[j] );
			R1 = mAxisLen[0] * absC[j][0] + mAxisLen[1] * absC[j][1] + mAxisLen[2] * absC[j][2];
			R01 = target->mAxisLen[j] + R1;
			if( R > R01 ) return false;
		}

		//B0~2
		for( int j = 0; j < 3; ++j ){
			R = abs( D3DXVec3Dot( &mAxisDir[j], &D ) );
			R0 = target->mAxisLen[0] * absC[0][j] + target->mAxisLen[1] * absC[1][j] + target->mAxisLen[2] * absC[2][j];
			R01 = R0 + mAxisLen[j];
			if( R > R01 ) return false;
		}

		//A0xB0
		R = abs( AD[2] * C[1][0] - AD[1] * C[2][0] );
		R0 = target->mAxisLen[1] * absC[2][0] + target->mAxisLen[2] * absC[1][0];
		R1 = mAxisLen[1] * absC[0][2] + mAxisLen[2] * absC[0][1];
		R01 = R0 + R1;
		if( R > R01 ) return false;

		//A0xB1
		R = abs( AD[2] * C[1][1] - AD[1] * C[2][1] );
		R0 = target->mAxisLen[1] * absC[2][1] + target->mAxisLen[2] * absC[1][1];
		R1 = mAxisLen[0] * absC[0][2] + mAxisLen[2] * absC[0][0];
		R01 = R0 + R1;
		if( R > R01 ) return false;

		//A0xB2
		R = abs( AD[2] * C[1][2] - AD[1] * C[2][2] );
		R0 = target->mAxisLen[1] * absC[2][2] + target->mAxisLen[2] * absC[1][2];
		R1 = mAxisLen[0] * absC[0][1] + mAxisLen[1] * absC[0][0];
		R01 = R0 + R1;
		if( R > R01 )return false;

		//A1xB0
		R = abs( AD[0] * C[2][0] - AD[2] * C[0][0] );
		R0 = target->mAxisLen[0] * absC[2][0] + target->mAxisLen[2] * absC[0][0];
		R1 = mAxisLen[1] * absC[1][2] + mAxisLen[2] * absC[1][1];
		R01 = R0 + R1;
		if( R > R01 ) return false;

		//A1xB1
		R = abs( AD[0] * C[2][1] - AD[2] * C[0][1] );
		R0 = target->mAxisLen[0] * absC[2][1] + target->mAxisLen[2] * absC[0][1];
		R1 = mAxisLen[0] * absC[1][2] + mAxisLen[2] * absC[1][0];
		R01 = R0 + R1;
		if( R > R01 ) return false;

		//A1xB2
		R = abs( AD[0] * C[2][2] - AD[2] * C[0][2] );
		R0 = target->mAxisLen[0] * absC[2][2] + target->mAxisLen[2] * absC[0][2];
		R1 = mAxisLen[0] * absC[1][1] + mAxisLen[1] * absC[1][0];
		R01 = R0 + R1;
		if( R > R01 ) return false;

		//A2xB0
		R = abs( AD[1] * C[0][0] - AD[0] * C[1][0] );
		R0 = target->mAxisLen[0] * absC[1][0] + target->mAxisLen[1] * absC[0][0];
		R1 = mAxisLen[1] * absC[2][2] + mAxisLen[2] * absC[2][1];
		R01 = R0 + R1;
		if( R > R01 ) return false;

		//A2xB1
		R = abs( AD[1] * C[0][1] - AD[0] * C[1][1] );
		R0 = target->mAxisLen[0] * absC[1][1] + target->mAxisLen[1] * absC[0][1];
		R1 = mAxisLen[0] * absC[2][2] + mAxisLen[2] * absC[2][0];
		R01 = R0 + R1;
		if( R > R01 ) return false;

		//A2xB2
		R = abs( AD[1] * C[0][2] - AD[0] * C[1][2] );
		R0 = target->mAxisLen[0] * absC[1][2] + target->mAxisLen[1] * absC[0][2];
		R1 = mAxisLen[0] * absC[2][1] + mAxisLen[1] * absC[2][0];
		R01 = R0 + R1;
		if( R > R01 ) return false;

		return true;
	}
}