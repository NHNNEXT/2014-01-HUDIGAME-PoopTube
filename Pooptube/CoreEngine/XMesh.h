#pragma once
#include "Node.h"

namespace pooptube{
	
	class XMeshData : public Node {
	public:
		XMeshData();
		virtual ~XMeshData();

		static XMeshData *Create(const std::wstring& FilePath);
		bool Init(const std::wstring& FilePath);

		void Update(float dTime);
		void Render();

		LPD3DXMESH GetMesh() { return mXMesh; }

	private:
		LPD3DXMESH			mXMesh = nullptr;
		D3DMATERIAL9*		mMaterial = nullptr;
		LPDIRECT3DTEXTURE9* mTexture = nullptr;

		DWORD				mNumMaterial = 0;
	};

	class XMesh : public Node {
	public:
		XMesh();
		virtual ~XMesh();

		static XMesh *Create(const std::wstring& FilePath);

		//그냥 귀찮아서 리소스 메니져 안거치고 다 때려박음
		bool Init(const std::wstring& FilePath);

		void Update(float dTime);
		void Render();

	protected:

		void _MakeBoundingSphere(D3DXVECTOR3& outSphereCenter, float& outSphereRadius);
		bool _CheckFrustum();

	private:
		XMeshData* mMeshData = nullptr;

		D3DXVECTOR3						mBoundingSphereCenter; // 절두체 컬링용 Bounding Sphere
		float							mBoundingSphereRadius; // 절두체 컬링용 Bounding Sphere

	};
}



