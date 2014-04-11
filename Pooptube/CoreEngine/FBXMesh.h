#pragma once
#include "Mesh.h"

namespace pooptube {

	class FBXMesh : public Mesh
	{
	public:
		FBXMesh();
		virtual ~FBXMesh();

		//팩토리 구조, 생성을 위해서는 FBXManager에서 생성한 FBXScene을 인자로 넘겨야 한다.
		static FBXMesh* Create(FbxScene* pScene);

		virtual bool Init(FbxScene* pScene);

		virtual void Render();
		virtual void Update(float dTime);

		LPDIRECT3DVERTEXBUFFER9 GetMeshVertexBuffer() const { return mMeshVertexBuffer; }

	protected:

		bool _LoadMeshFromFBXScene(FbxScene* pScene);

	private:
		//메쉬의 버텍스 값을 저장하는 버텍스 버퍼
		LPDIRECT3DVERTEXBUFFER9 mMeshVertexBuffer;
		FbxScene*				mScene;

	};

}
