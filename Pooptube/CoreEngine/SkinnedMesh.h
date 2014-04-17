#pragma once
#include "Node.h"
#include "CollisionBox.h" // 임시

namespace pooptube {

	//전방선언
	class Mesh;

	class SkinnedMesh : public Node {
	public:
		SkinnedMesh();
		virtual ~SkinnedMesh();

		static SkinnedMesh* Create(std::string FilePath);
		virtual bool Init(std::string FilePath);

		virtual void Render();
		virtual void Update(float dTime);

		LPDIRECT3DVERTEXBUFFER9 GetMeshVertexBuffer() const { return mMeshVertexBuffer; }

		CollisionBox* mCollisionBox; // 임시
	protected:



	private:
		//메쉬의 버텍스 값을 저장하는 버텍스 버퍼
		LPDIRECT3DVERTEXBUFFER9 mMeshVertexBuffer;
		LPDIRECT3DINDEXBUFFER9	mMeshIndexBuffer;

		Mesh*		mMesh;
		UINT		mFBXMeshHandle;
	};

}