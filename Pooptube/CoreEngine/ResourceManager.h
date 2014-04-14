#pragma once
#include "stdafx.h"
#include "Object.h"


namespace pooptube {

	//���漱��
	class FBXMesh;


	class ResourceManager : public Object {
	public:
		ResourceManager();
		~ResourceManager();

		static ResourceManager* GetInstance();

		FBXMesh* LoadMeshFromFBX(std::string FilePath);

	protected:
		bool _Init();
		bool _FBXInit();

		FBXMesh* _LoadFBXFile(std::string FilePath);
		//�ӽ÷� �Ž��� �̾Ƴ����� ���� �Լ�
		//������ fbx������ ��带 ��ȸ�ϸ鼭 �޽�, ���ϸ��̼� ĳ��, �ؽ��� ���� ������ �̾�
		//���� �ڷ����� �����ϴ� ����� �����ؾ� �Ѵ�.
		FBXMesh* _ReadVerticesFromFBX(FbxScene* pScene);

	private:

		static  ResourceManager* mInstance;

		//sdk���� �����ϴ� FbxManager
		FbxManager*	mManager;

		//�ε�� fbxmesh������ ����ִ� meshtable
		std::map<std::string, FBXMesh*> mFBXMeshTable;



	};

}