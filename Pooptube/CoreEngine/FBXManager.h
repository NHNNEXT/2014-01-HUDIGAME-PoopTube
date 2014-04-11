#pragma once
#include "stdafx.h"
#include "Object.h"

namespace pooptube {

	class Mesh;

	//fbx의 관련 설명자료는 곧 만들어서 위키에 올리도록 하겠습니다.
	class FBXManager : public Object
	{
	public:

		FBXManager(void);
		~FBXManager(void);

		//일단 싱클톤으로 제작 fbxmanager관리문제는 차차 생각해봐야 함
		static FBXManager* GetInstance();

		//FBX파일을 로드하는 함수 로드한 후 FBXScene을 반환한다.
		FbxScene* LoadFBXFile(const char* pFileName);

	protected:

		//FBXManager를 create하고 iosetting을 한다.
		//생성시 자동호출
		bool _Init();

		//fbx파일은 기본적으로 node의 트리구조를 이루고 있습니다.
		//리커시브하게 돌면서 fbx파일의 모든 node를 읽습니다.
		//성능상 재귀함수를 루프로 푸는 방법을 연구해야함
		void _LoadNodeRecursive(FbxNode* pNode, Mesh* CoreMesh);
		void _LoadNode(FbxNode* pNode, Mesh* CoreMesh);
		void _LoadMesh(FbxNode* pNode, Mesh* CoreMesh);


	private:
		static  FBXManager* mInstance;

		//이 클래스 아님 sdk에서 제공하는 FbxManager
		FbxManager*	mManager;
		//FbxScene*	mScene;


	};
}