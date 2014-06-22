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
	CollisionBox *CollisionBox::Create( Node* pNode ) {
		CollisionBox *pCollisionBox(new CollisionBox);

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

		InitVB();
		InitIB();

		mObjectName = "CollisionBox" + std::to_string(Node::ObjectNum-1);
		mClassName = "CollisionBox";

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


// 	void CollisionBox::SetAABBCollisionBoxFromSkinnedMesh(SkinnedMesh *pMesh) {
// 		SetAABBCollisionBoxFromVertices(pMesh->GetMeshData()->GetVertices(), pMesh->GetMeshData()->GetVertexCount());
// 	}

	void CollisionBox::Render()
	{
		Node::Render();

		/*
		DWORD BeforeRenderState;
		DWORD BeforeLightState;

		mDevice->GetRenderState(D3DRS_LIGHTING, &BeforeLightState);
		mDevice->GetRenderState(D3DRS_FILLMODE, &BeforeRenderState);
		mDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		mDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		mDevice->SetFVF(D3DFVF_CUSTOMVERTEX_COLLISIONBOX);
		//디바이스에 버텍스버퍼를 전달
		mDevice->SetStreamSource(0, mVertexBuffer, 0, sizeof(COLLISION_CUSTOM_VERTEX));

		//인덱스 설정
		mDevice->SetIndices(mIndexBuffer);
		mDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		mDevice->SetRenderState(D3DRS_LIGHTING, BeforeLightState);
		mDevice->SetRenderState(D3DRS_FILLMODE, BeforeRenderState);
		*/
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
		if( D3DXVec3LengthSq( &D ) > D3DXVec3LengthSq( &D3DXVECTOR3( mAxisLen[0], mAxisLen[1], mAxisLen[2] ) )
			+ D3DXVec3LengthSq( &D3DXVECTOR3( target->mAxisLen[0], target->mAxisLen[1], target->mAxisLen[2] ) )
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

	int CollisionBox::InitVB() {
		/// 상자(cube)를 렌더링하기위해 8개의 정점을 선언
		COLLISION_CUSTOM_VERTEX vertices[] =
		{
			{ -mAxisLen[0], mAxisLen[1], mAxisLen[2], 0xFFFF0000 },	/// v0
			{ mAxisLen[0], mAxisLen[1], mAxisLen[2], 0xFFFF0000 },	/// v1
			{ mAxisLen[0], mAxisLen[1], -mAxisLen[2], 0xFFFF0000 },	/// v2
			{ -mAxisLen[0], mAxisLen[1], -mAxisLen[2], 0xFFFF0000 },	 /// v3

			{ -mAxisLen[0], -mAxisLen[1], mAxisLen[2], 0xFFFF0000 },	 /// v4
			{ mAxisLen[0], -mAxisLen[1], mAxisLen[2], 0xFFFF0000 },	/// v5
			{ mAxisLen[0], -mAxisLen[1], -mAxisLen[2], 0xFFFF0000 },	 /// v6
			{ -mAxisLen[0], -mAxisLen[1], -mAxisLen[2], 0xFFFF0000 }, /// v7
		};

		/// 정점버퍼 생성
		/// 8개의 사용자정점을 보관할 메모리를 할당한다.
		/// FVF를 지정하여 보관할 데이터의 형식을 지정한다.
		if (FAILED(mDevice->CreateVertexBuffer(8 * sizeof(COLLISION_CUSTOM_VERTEX),
			0, D3DFVF_CUSTOMVERTEX_COLLISIONBOX,
			D3DPOOL_DEFAULT, &mVertexBuffer, NULL)))
		{
			return E_FAIL;
		}

		/// 정점버퍼를 값으로 채운다. 
		/// 정점버퍼의 Lock()함수를 호출하여 포인터를 얻어온다.
		VOID* pVertices;
		if (FAILED(mVertexBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
			return E_FAIL;

		memcpy(pVertices, vertices, sizeof(vertices));
		mVertexBuffer->Unlock();

		return S_OK;
	}

	int CollisionBox::InitIB() {
		/// 상자(cube)를 렌더링하기위해 12개의 면을 선언
		COLLISION_CUSTOM_INDEX	indices[] =
		{
			{ 0, 1, 2 }, { 0, 2, 3 },	/// 윗면
			{ 4, 6, 5 }, { 4, 7, 6 },	/// 아랫면
			{ 0, 3, 7 }, { 0, 7, 4 },	/// 왼면
			{ 1, 5, 6 }, { 1, 6, 2 },	/// 오른면
			{ 3, 2, 6 }, { 3, 6, 7 },	/// 앞면
			{ 0, 4, 5 }, { 0, 5, 1 }	/// 뒷면
		};

		/// 인덱스버퍼 생성
		/// D3DFMT_INDEX16은 인덱스의 단위가 16비트 라는 것이다.
		/// 우리는 MYINDEX 구조체에서 WORD형으로 선언했으므로 D3DFMT_INDEX16을 사용한다.
		if (FAILED(mDevice->CreateIndexBuffer(12 * sizeof(COLLISION_CUSTOM_INDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mIndexBuffer, NULL)))
		{
			return E_FAIL;
		}

		/// 인덱스버퍼를 값으로 채운다. 
		/// 인덱스버퍼의 Lock()함수를 호출하여 포인터를 얻어온다.
		VOID* pIndices;
		if (FAILED(mIndexBuffer->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
			return E_FAIL;
		memcpy(pIndices, indices, sizeof(indices));
		mIndexBuffer->Unlock();

		return S_OK;
	}

}