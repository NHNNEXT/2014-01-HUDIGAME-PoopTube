#pragma once
#include "Node.h"

namespace pooptube{

	class BillBoard :
		public Node {

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
