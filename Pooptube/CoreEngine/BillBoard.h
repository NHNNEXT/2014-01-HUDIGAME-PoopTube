#pragma once
#include "Node.h"

namespace pooptube{

	class BillBoard :
		public Node
	{
		const int D3DFVF_CUSTOMVERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

		struct MESH_CUSTOM_VERTEX {
			D3DXVECTOR3	position;
			D3DXVECTOR3	normal;
			D3DCOLOR	color;
			float		tu, tv;
		};

		struct MESH_CUSTOM_INDEX {
			UINT w0, w1, w2;
		};

	public:
		BillBoard();
		virtual ~BillBoard();

		static BillBoard *Create();
		bool _Init();

		virtual void Render();

		void SetTexture( std::wstring filename );

	private:
		LPDIRECT3DVERTEXBUFFER9		mMeshVertexBuffer = nullptr;
		LPDIRECT3DINDEXBUFFER9		mMeshIndexBuffer = nullptr;
		LPDIRECT3DTEXTURE9			mTexture = nullptr;

		std::array<MESH_CUSTOM_VERTEX, 4> vertexList;
	};
}
