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
		void SetTextureUVSize( float u, float v );
		void SetTextureMatrix( D3DXMATRIXA16& matMatrix ){ mMatrix = matMatrix; }

		void SetEffectTech(std::string& TechName) { mEffectTech = TechName; }

	private:
		bool _SetVertexBuffer();
		bool _SetIndexBuffer();

		LPDIRECT3DVERTEXBUFFER9		mMeshVertexBuffer = nullptr;
		LPDIRECT3DINDEXBUFFER9		mMeshIndexBuffer = nullptr;
		LPDIRECT3DTEXTURE9			mTexture = nullptr;
		ID3DXEffect*				mEffect = nullptr;
		D3DXMATRIXA16				mMatrix, mMatIdentity;

		std::string					mEffectTech = "t4";
		std::array<MESH_CUSTOM_VERTEX, 4> vertexList;
	};
}
