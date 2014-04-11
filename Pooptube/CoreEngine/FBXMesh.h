#pragma once
#include "Mesh.h"

namespace pooptube {

	class FBXMesh : public Mesh
	{
	public:
		FBXMesh();
		virtual ~FBXMesh();

		//���丮 ����, ������ ���ؼ��� FBXManager���� ������ FBXScene�� ���ڷ� �Ѱܾ� �Ѵ�.
		static FBXMesh* Create(FbxScene* pScene);

		virtual bool Init(FbxScene* pScene);

		virtual void Render();
		virtual void Update(float dTime);

		LPDIRECT3DVERTEXBUFFER9 GetMeshVertexBuffer() const { return mMeshVertexBuffer; }

	protected:

		bool _LoadMeshFromFBXScene(FbxScene* pScene);

	private:
		//�޽��� ���ؽ� ���� �����ϴ� ���ؽ� ����
		LPDIRECT3DVERTEXBUFFER9 mMeshVertexBuffer;
		FbxScene*				mScene;

	};

}
