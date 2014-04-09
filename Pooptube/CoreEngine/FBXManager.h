#pragma once
#include "stdafx.h"
#include "Object.h"

namespace pooptube {

	class Mesh;

	class FBXManager : public Object
	{
	public:

		FBXManager(void);
		~FBXManager(void);

		//일단 싱클톤으로 제작 fbxmanager관리문제는 차차 생각해봐야 함
		static FBXManager* GetInstance();

		bool InitSdkObjects();
		void LoadFBXFile(const char* pFileName, Mesh* CoreMesh);

		//임시로 기능뽑기 위해 만든 함수
		void LoadFBXFile2(const char* pFileName, Mesh* CoreMesh);

	protected:

		//fbx파일은 기본적으로 node의 트리구조를 이루고 있습니다.
		//리커시브하게 돌면서 fbx파일의 모든 node를 읽습니다.
		//성능상 재귀함수를 루프로 푸는 방법을 연구해야함
		void _LoadNodeRecursive(FbxNode* pNode, Mesh* CoreMesh);
		void _LoadNode(FbxNode* pNode, Mesh* CoreMesh);
		void _LoadMesh(FbxNode* pNode, Mesh* CoreMesh);


	private:
		static  FBXManager* m_Instance;

		//이 클래스 아님 sdk에서 제공하는 FbxManager
		FbxManager*	m_pManager;
		FbxScene*	m_pScene;


	};
}