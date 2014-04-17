/**
* @author ������
* @brief ���ؽ�, ���ϸ��̼�, �ؽ��� ���� ������ �о�� �����ϴ� ��ü
* @date 2014/04/17
* @file SkinnedMesh.h
*/
#pragma once
#include "Node.h"

namespace pooptube {

	//���漱��
	class Mesh;

	//@brief 
	//���丮 ����. Mesh�� �������� objmanager���� ����
	//����ڰ� ���� �����ϴ� �Լ�
	class SkinnedMesh : public Node {
	public:
		SkinnedMesh();
		virtual ~SkinnedMesh();

		//@param VertexCount 
		//@param PolygonCount
		//@return ������ Mesh�� ��ȯ
		static SkinnedMesh* Create(std::string FilePath);
		virtual bool Init(std::string FilePath);

		virtual void Render();
		virtual void Update(float dTime);

		LPDIRECT3DVERTEXBUFFER9 GetMeshVertexBuffer() const { return mMeshVertexBuffer; }

	protected:



	private:
		//�޽��� ���ؽ� ���� �����ϴ� ���ؽ� ����
		LPDIRECT3DVERTEXBUFFER9 mMeshVertexBuffer;
		LPDIRECT3DINDEXBUFFER9	mMeshIndexBuffer;

		Mesh*		mMesh;
	};

}