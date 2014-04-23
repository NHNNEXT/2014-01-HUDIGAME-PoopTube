/**
* @author ������
* @brief
* ���ؽ�, ���ϸ��̼�, �ؽ��� ���� ������ �о�� �����ϴ� ��ü
* ���丮 ����. Mesh�� �������� objmanager���� ����
* ����ڰ� ���� �����ϴ� �Լ�
* @date 2014/04/17
* @file SkinnedMesh.h
*/
#pragma once
#include "Node.h"

namespace pooptube {

	class Mesh;

	//@brief �޽��� ���ҽ� Ÿ�������� ����
	enum RESOURCE_TYPE {
		RESOURCE_NULL = 0,
		RESOURCE_FBX = 1,
		RESOURCE_HEIGHTMAP = 2,
	};

	class SkinnedMesh : public Node {
	public:
		SkinnedMesh();
		virtual ~SkinnedMesh();

		//@param VertexCount 
		//@param PolygonCount
		//@param ResourceType
		//@return ������ Mesh�� ��ȯ
		//@exception �ʱ�ȭ�� �����ϰų� ���ҽ� Ÿ���� null�� ��� nullptr��ȯ
		static std::shared_ptr<SkinnedMesh> Create(const std::string& MeshFilePath, RESOURCE_TYPE ResourceType);
		virtual bool Init(const std::string& MeshFilePath, RESOURCE_TYPE ResourceType);

		virtual void Render();
		virtual void Update(float dTime);

	protected:



	private:

		LPDIRECT3DVERTEXBUFFER9 mMeshVertexBuffer = nullptr;
		LPDIRECT3DINDEXBUFFER9	mMeshIndexBuffer = nullptr;

		std::shared_ptr<Mesh>	mMesh = nullptr;

		RESOURCE_TYPE			mResourceType = RESOURCE_NULL;
	};

}