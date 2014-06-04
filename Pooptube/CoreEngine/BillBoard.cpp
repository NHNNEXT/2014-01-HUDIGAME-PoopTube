#include "stdafx.h"
#include "BillBoard.h"
#include "ResourceManager.h"

namespace pooptube{
	const int BILLBOARD_VERTEX_NUM = 4;
	const int BILLBOARD_POLYGON_NUM = 2;

	BillBoard::BillBoard()
	{
	}

	BillBoard::~BillBoard()
	{
	}

	BillBoard * BillBoard::Create()
	{
		BillBoard *pBoard = new BillBoard;
		if( pBoard->_Init() )
			return pBoard;
		else
			return nullptr;
	}

	bool BillBoard::_Init()
	{
		Node::Init();

		D3DXMatrixIdentity( &mMatIdentity );
		D3DXMatrixIdentity( &mMatrix );

		vertexList[0] = {
			D3DXVECTOR3( -1.f, 1.f, 0.f ),
			D3DXVECTOR3( 0.f, 0.f, -1.f ),
			D3DXCOLOR( 1.f, 1.f, 1.f, 1.f ),
			0.f, 0.f
		};
		vertexList[1] = {
			D3DXVECTOR3( -1.f, -1.f, 0.f ),
			D3DXVECTOR3( 0.f, 0.f, -1.f ),
			D3DXCOLOR( 1.f, 1.f, 1.f, 1.f ),
			0.f, 1.f
		};
		vertexList[2] = {
			D3DXVECTOR3( 1.f, -1.f, 0.f ),
			D3DXVECTOR3( 0.f, 0.f, -1.f ),
			D3DXCOLOR( 1.f, 1.f, 1.f, 1.f ),
			1.f, 1.f
		};
		vertexList[3] = {
			D3DXVECTOR3( 1.f, 1.f, 0.f ),
			D3DXVECTOR3( 0.f, 0.f, -1.f ),
			D3DXCOLOR( 1.f, 1.f, 1.f, 1.f ),
			1.f, 0.f
		};

		//버텍스 버퍼 생성
		if( GetDevice()->CreateVertexBuffer( BILLBOARD_VERTEX_NUM * sizeof( MESH_CUSTOM_VERTEX ), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &mMeshVertexBuffer, NULL ) < 0 )
		{
			return false;
		}
		if(!_SetVertexBuffer())
			return false;

		//인덱스 버퍼 생성
		if( GetDevice()->CreateIndexBuffer( BILLBOARD_POLYGON_NUM * sizeof( MESH_CUSTOM_INDEX ), 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &mMeshIndexBuffer, NULL ) < 0 ) {
			return false;
		}
		if(!_SetIndexBuffer())
			return false;
		
		return true;
	}

	void BillBoard::Render()
	{
		if (mIsVisible == true) {
			Node::Render();

			mDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

			D3DXMATRIXA16	MatView;
			mDevice->GetTransform(D3DTS_VIEW, &MatView);

			SetFrontVector(MatView._13, MatView._23, MatView._33);

			//디바이스에 버텍스버퍼를 전달
			mDevice->SetStreamSource(0, mMeshVertexBuffer, 0, sizeof(MESH_CUSTOM_VERTEX));

			//인덱스 설정
			mDevice->SetIndices(mMeshIndexBuffer);

			mDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
			mDevice->SetTransform( D3DTS_TEXTURE0, &mMatrix );

			GetDevice()->SetTexture( 0, mTexture );
			
			mDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, BILLBOARD_VERTEX_NUM, 0, BILLBOARD_VERTEX_NUM );
			mDevice->SetTransform( D3DTS_TEXTURE0, &mMatIdentity );
		}
	}

	void BillBoard::SetTexture( std::wstring filename )
	{
		mTexture = ResourceManager::GetInstance()->LoadTexture( filename );
	}

	void BillBoard::SetTextureUVSize( float u, float v )
	{
		vertexList[1].tv = v;
		vertexList[2].tu = u;
		vertexList[2].tv = v;
		vertexList[3].tu = u;
		_SetVertexBuffer();
	}

	bool BillBoard::_SetVertexBuffer()
	{
		VOID* pVertices;
		if( mMeshVertexBuffer->Lock( 0, BILLBOARD_VERTEX_NUM * sizeof( MESH_CUSTOM_VERTEX ), (void**)&pVertices, 0 ) < 0 )
			return false;
		memcpy( pVertices, vertexList.data(), BILLBOARD_VERTEX_NUM * sizeof( MESH_CUSTOM_VERTEX ) );
		mMeshVertexBuffer->Unlock();

		return true;
	}

	bool BillBoard::_SetIndexBuffer()
	{
		std::array<MESH_CUSTOM_INDEX, BILLBOARD_POLYGON_NUM> indexList;
		indexList[0] = { 0, 3, 1 };
		indexList[1] = { 3, 2, 1 };

		VOID* pIndices;
		if( mMeshIndexBuffer->Lock( 0, BILLBOARD_POLYGON_NUM * sizeof( MESH_CUSTOM_INDEX ), (void**)&pIndices, 0 ) < 0 )
			return false;
		memcpy( pIndices, indexList.data(), BILLBOARD_POLYGON_NUM * sizeof( MESH_CUSTOM_INDEX ) );
		mMeshIndexBuffer->Unlock();

		return true;
	}

}
