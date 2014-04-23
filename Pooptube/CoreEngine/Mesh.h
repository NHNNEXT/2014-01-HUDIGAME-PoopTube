/**
* @author ������
* @brief �޽��� �����͸� �����ִ� ��ü
* ���丮 ����. Mesh�� �������� objmanager���� ����
* ����ڰ� ���� ������ ���� ����
* @date 2014/04/17
* @file Mesh.h
*/
#pragma once
#include "Node.h"

namespace pooptube {

	//@brief Ŀ���� ���ؽ��� ����
	const int D3DFVF_CUSTOMVERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;

	//@brief Ŀ���� ���ؽ�
	struct MESH_CUSTOM_VERTEX {
		D3DXVECTOR3	position;
		D3DXVECTOR3	normal;
		D3DCOLOR	color;
	};

	//@brief 
	//Ŀ���� �ε���
	//���� 16��Ʈ ũ�� ���� ������ 32��Ʈ�� ���� �ʿ�
	struct MESH_CUSTOM_INDEX {
		WORD w0, w1, w2;
	};

	class Mesh : public Node
	{
	public:
		Mesh();
		virtual ~Mesh();

		//@param VertexCount 
		//@param PolygonCount
		//@return ������ Mesh�� ��ȯ
		//@exception �ʱ�ȭ�� �����ϸ� nullptr��ȯ
		static std::shared_ptr<Mesh> Create(int VertexCount, int PolygonCount);

		virtual bool Init(int VertexCount, int PolygonCount);

		virtual void Render();
		virtual void Update(float dTime);

		int						GetPolygonCount() const { return mPolygonCount; }
		int						GetVertexCount() const { return mVertexCount; }
		MESH_CUSTOM_VERTEX*		GetVertices() const { return mVertices; }
		MESH_CUSTOM_INDEX*		GetIndices() const { return mIndices; }

		float					GetHeight(float x, float z) const;

	protected:


	private:

		int								mPolygonCount = 0;
		int								mVertexCount = 0;

		MESH_CUSTOM_VERTEX*				mVertices = nullptr;
		MESH_CUSTOM_INDEX*				mIndices = nullptr;
		
		LPDIRECT3DDEVICE9				mDevice = nullptr;

		float							mHeightMapSize = 0.5f;
	};

}
