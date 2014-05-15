#pragma once
#include "Node.h"

namespace pooptube{
	class XMesh : public Node {
	public:
		XMesh();
		virtual ~XMesh();

		static XMesh *Create(const std::string& FilePath);

		//그냥 귀찮아서 리소스 메니져 안거치고 다 때려박음
		bool Init(const std::string& FilePath);

		void Update(float dTime);
		void Render();

		std::vector<D3DXVECTOR3> GetVertices() { return mVertices; }
		std::vector<D3DXVECTOR3> GetIndices() { return mIndices; }

		LPD3DXMESH GetMesh() { return mXMesh; }

	private:
		LPD3DXMESH			mXMesh = nullptr;
// 		D3DMATERIAL9*		mMaterial = nullptr;
// 		LPDIRECT3DTEXTURE9* mTexture = nullptr;

		std::vector<D3DXVECTOR3> mVertices;
		std::vector<D3DXVECTOR3> mIndices;

		DWORD				mNumMaterial = 0;
	};
}



