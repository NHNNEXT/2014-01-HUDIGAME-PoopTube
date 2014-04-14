#pragma once
#include "stdafx.h"
#include "Object.h"


namespace pooptube {

	//전방선언
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
		//임시로 매쉬만 뽑아내도록 만든 함수
		//앞으로 fbx파일의 노드를 순회하면서 메쉬, 에니메이션 캐쉬, 텍스쳐 등의 정보를 뽑아
		//만든 자료형에 보관하는 기능을 구현해야 한다.
		FBXMesh* _ReadVerticesFromFBX(FbxScene* pScene);

	private:

		static  ResourceManager* mInstance;

		//sdk에서 제공하는 FbxManager
		FbxManager*	mManager;

		//로드된 fbxmesh정보를 쥐고있는 meshtable
		std::map<std::string, FBXMesh*> mFBXMeshTable;



	};

}