#pragma once
#include "Node.h"
#include "CollisionBox.h" // �ӽ�

namespace pooptube {

	//���漱��
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

		CollisionBox* mCollisionBox; // �ӽ�
	protected:



	private:
		//�޽��� ���ؽ� ���� �����ϴ� ���ؽ� ����
		LPDIRECT3DVERTEXBUFFER9 mMeshVertexBuffer;
		LPDIRECT3DINDEXBUFFER9	mMeshIndexBuffer;

		Mesh*		mMesh;
		UINT		mFBXMeshHandle;
	};

}