#pragma once
#include "Node.h"

namespace pooptube {

	//Ŀ���� ���ؽ��� ����
	const int D3DFVF_CUSTOMVERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;

	//�Ž����� ���Ǵ� Ŀ���� ���ؽ��Դϴ�.
	//�̰� ����ֵ� �ǳ�? ���� ��Ŭ��
	struct MESH_CUSTOM_VERTEX {
		D3DXVECTOR3	position;
		D3DXVECTOR3	normal;
		D3DCOLOR	color;
	};

	struct MESH_CUSTOM_INDEX {
		//���� 16��Ʈ ũ�� ���� ������ 32��Ʈ�� ���� �ʿ�
		WORD w0, w1, w2;
	};

	class FBXMesh : public Node
	{
	public:
		FBXMesh();
		virtual ~FBXMesh();

		//���丮 ����, ������ ���ؼ��� FBXManager���� ������ FBXScene�� ���ڷ� �Ѱܾ� �Ѵ�.
		static FBXMesh* Create(int VertexCount, int PolygonCount);

		virtual bool Init(int VertexCount, int PolygonCount);

		virtual void Render();
		virtual void Update(float dTime);

		int						GetPolygonCount() const { return mPolygonCount; }
		int						GetVertexCount() const { return mVertexCount; }
		MESH_CUSTOM_VERTEX*		GetVertices() const { return mVertices; }
		MESH_CUSTOM_INDEX*		GetIndices() const { return mIndices; }

		UINT					GetCountHandle() { return ++mCountHandle; }

	protected:


	private:
		//�޽��� ������ ����
		int								mPolygonCount;
		int								mVertexCount;
	

		//����� skinnedMesh�� �ڽ��� �����ϰ� �ִ���
		UINT							mCountHandle;

		MESH_CUSTOM_VERTEX*				mVertices;
		MESH_CUSTOM_INDEX*				mIndices;
		

	};

}
