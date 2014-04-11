#pragma once
#include "Node.h"

namespace pooptube {

	//Ŀ���� ���ؽ��� ����
	const int D3DFVF_CUSTOMVERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;

	//�Ž����� ���Ǵ� Ŀ���� ���ؽ��Դϴ�.
	//�̰� ����ֵ� �ǳ�? ���� ��Ŭ��
	struct MESHCUSTOMVERTEX
	{
		D3DXVECTOR3	position;
		D3DXVECTOR3	normal;
		D3DCOLOR	color;
	};

	class FBXMesh : public Node
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
		int						GetPolygonCount() const { return mPolygonCount; }

	protected:

		bool _LoadMeshFromFBXScene(FbxScene* pScene);

	private:
		//�޽��� ���ؽ� ���� �����ϴ� ���ؽ� ����
		LPDIRECT3DVERTEXBUFFER9 mMeshVertexBuffer;
		FbxScene*				mScene;
		//�޽��� ������ ����
		//�Ź� ���Ҷ����� ����������Ѵ�. ���ϰ� �ڵ����� ���ϴ� ����� �����غ���.
		int						mPolygonCount;
	};

}
