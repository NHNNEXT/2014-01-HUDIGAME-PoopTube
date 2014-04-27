#include "stdafx.h"
#include "Application.h"
#include "CollisionBox.h"
#include "CollisionManager.h"
#include "SkinnedMesh.h"

namespace pooptube {
	CollisionBox::CollisionBox() {
	}

	CollisionBox::~CollisionBox() {
	}

//	std::shared_ptr<CollisionBox> CollisionBox::Create( std::shared_ptr<Node> pNode ) {
	std::shared_ptr<CollisionBox> CollisionBox::Create( Node* pNode ) {
		std::shared_ptr<CollisionBox> pCollisionBox(new CollisionBox);

		if( pCollisionBox->Init( pNode ) ) {
			CollisionManager::GetInstance()->AddCollisionBox( pCollisionBox, pNode );
			return pCollisionBox;
		}
		else 
			return nullptr;
	}

//	bool CollisionBox::Init( std::shared_ptr<Node> pNode ) {
	bool CollisionBox::Init( Node* pNode ) {
		if (!Node::Init())
			return false;

		mAxisLen[AXIS_X] = 0.5f;
		mAxisLen[AXIS_Y] = 0.5f;
		mAxisLen[AXIS_Z] = 0.5f;
		mParentNode = pNode;

		return true;
	}

	void CollisionBox::SetAABBCollisionBoxFromVertices(MESH_CUSTOM_VERTEX* vertices, UINT Size) {

		MESH_CUSTOM_VERTEX Max;
		MESH_CUSTOM_VERTEX Min;

		if (Size == 0)
			return;

		Max = vertices[0];
		Min = vertices[0];

		for (UINT i = 1; i < Size; ++i) {
			Min.position.x = __min(Min.position.x, vertices[i].position.x);
			Min.position.y = __min(Min.position.y, vertices[i].position.y);
			Min.position.z = __min(Min.position.z, vertices[i].position.z);
			Max.position.x = __max(Max.position.x, vertices[i].position.x);
			Max.position.y = __max(Max.position.y, vertices[i].position.y);
			Max.position.z = __max(Max.position.z, vertices[i].position.z);
		}

		mAxisLen[AXIS_X] = (Max.position.x - Min.position.x) * 0.5f;
		mAxisLen[AXIS_Y] = (Max.position.y - Min.position.y) * 0.5f;
		mAxisLen[AXIS_Z] = (Max.position.z - Min.position.z) * 0.5f;

		Node::Translation(D3DXVECTOR3(0.f, mAxisLen[AXIS_Y], 0.f));

		return;
	}


	void CollisionBox::SetAABBCollisionBoxFromSkinnedMesh(std::shared_ptr<SkinnedMesh> pMesh) {
		SetAABBCollisionBoxFromVertices(pMesh->GetMeshData()->GetVertices(), pMesh->GetMeshData()->GetVertexCount());
	}

	void CollisionBox::Render()
	{
		Node::Render();

		D3DXMATRIX projMat, viewMat;
		GetDevice()->GetTransform( D3DTS_PROJECTION, &projMat );
		GetDevice()->GetTransform(D3DTS_VIEW, &viewMat);
		viewMat *= projMat;

		ID3DXLine *Line;

		if (D3DXCreateLine(GetDevice(), &Line) != D3D_OK)
			return;
		Line->SetWidth( 1 );
		Line->SetAntialias( true );

		D3DXVECTOR3 mXDirVec, mFrontVector, tXDirVec;
		D3DXVec3Normalize( &mFrontVector, &GetFrontVector() );
		D3DXVec3Cross( &mXDirVec, &GetUpVector(), &mFrontVector );
		D3DXVECTOR3 mAxisDir[3] = { mXDirVec, GetUpVector(), GetFrontVector() };
		D3DXVECTOR3 vF[3];
		for( int i = 0; i < 3; ++i ){
			vF[i] = mAxisDir[i] * mAxisLen[i];
		}

		D3DXVECTOR3 centerPos = GetPosition();
		D3DXCOLOR	lineColor = D3DXCOLOR( 0.0f, 0.5f, 1.0f, 1.0f );
		D3DXVECTOR3 point[11];
		point[0] = centerPos - vF[0] + vF[1] + vF[2];
		point[1] = centerPos + vF[0] + vF[1] + vF[2];
		point[2] = centerPos + vF[0] + vF[1] - vF[2];
		point[3] = centerPos - vF[0] + vF[1] - vF[2];
		point[4] = centerPos - vF[0] + vF[1] + vF[2];
		point[5] = centerPos - vF[0] - vF[1] + vF[2];
		point[6] = centerPos + vF[0] - vF[1] + vF[2];
		point[7] = centerPos + vF[0] - vF[1] - vF[2];
		point[8] = centerPos - vF[0] - vF[1] - vF[2];
		point[9] = centerPos - vF[0] - vF[1] + vF[2];
		Line->Begin();
		Line->DrawTransform( point, 10, &viewMat, lineColor );

		D3DXVECTOR3 pointT[2];
		pointT[0] = centerPos - vF[0] - vF[1] - vF[2];
		pointT[1] = centerPos - vF[0] + vF[1] - vF[2];
		Line->DrawTransform( pointT, 2, &viewMat, lineColor );

		pointT[0] = centerPos + vF[0] - vF[1] - vF[2];
		pointT[1] = centerPos + vF[0] + vF[1] - vF[2];
		Line->DrawTransform( pointT, 2, &viewMat, lineColor );

		pointT[0] = centerPos + vF[0] - vF[1] + vF[2];
		pointT[1] = centerPos + vF[0] + vF[1] + vF[2];
		Line->DrawTransform( pointT, 2, &viewMat, lineColor );
		Line->End();

		Line->Release();
	}

	void CollisionBox::Update( float dTime ) {
		Node::Update( dTime );
//		CollisionManager::GetInstance()->CollisionCheck( this );
	}

	bool CollisionBox::CollisionCheck( CollisionBox* target ) {
		D3DXVECTOR3 D = GetPosition() - target->GetPosition();

		if( mParentNode == target->mParentNode )
			return false;

		//Check By Sphere
		//속도 문제 개선 필요 여기 내용들은 매번 업데이트마다 순회함
		if( D3DXVec3Length( &D ) > D3DXVec3Length( &D3DXVECTOR3( mAxisLen[0], mAxisLen[1], mAxisLen[2] ) )
			+ D3DXVec3Length( &D3DXVECTOR3( target->mAxisLen[0], target->mAxisLen[1], target->mAxisLen[2] ) )
			)
		{
			return false;
		}

		//Start Check By OBB
		float C[3][3];    //matrix C=A^T B,c_{ij}=Dot(A_i,B_j)
		float absC[3][3]; //|c_{ij}|
		float AD[3];      //Dot(A_i,D)
		float R0, R1, R;    //interval radii and distance between centers
		float R01;        //=R0+R1
		D3DXVECTOR3 mXDirVec, tXDirVec, fVec;
		D3DXVec3Normalize( &fVec, &GetFrontVector( ) );
		D3DXVec3Cross( &mXDirVec, &GetUpVector(), &fVec );
		D3DXVECTOR3 mAxisDir[3] = { mXDirVec, GetUpVector( ), fVec };
		D3DXVec3Normalize( &fVec, &target->GetFrontVector( ) );
		D3DXVec3Cross( &tXDirVec, &target->GetUpVector( ), &fVec );
		D3DXVECTOR3 tAxisDir[3] = { tXDirVec, target->GetUpVector( ), fVec };

		//A0~2
		for( int j = 0; j < 3; ++j ){
			for( int i = 0; i < 3; ++i ){
				C[j][i] = D3DXVec3Dot( &tAxisDir[j], &mAxisDir[i] );
				absC[j][i] = abs( C[j][i] );
			}
			AD[j] = D3DXVec3Dot( &tAxisDir[j], &D );
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