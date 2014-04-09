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

		//�ϴ� ��Ŭ������ ���� fbxmanager���������� ���� �����غ��� ��
		static FBXManager* GetInstance();

		bool InitSdkObjects();
		void LoadFBXFile(const char* pFileName, Mesh* CoreMesh);

		//�ӽ÷� ��ɻ̱� ���� ���� �Լ�
		void LoadFBXFile2(const char* pFileName, Mesh* CoreMesh);

	protected:

		//fbx������ �⺻������ node�� Ʈ�������� �̷�� �ֽ��ϴ�.
		//��Ŀ�ú��ϰ� ���鼭 fbx������ ��� node�� �н��ϴ�.
		//���ɻ� ����Լ��� ������ Ǫ�� ����� �����ؾ���
		void _LoadNodeRecursive(FbxNode* pNode, Mesh* CoreMesh);
		void _LoadNode(FbxNode* pNode, Mesh* CoreMesh);
		void _LoadMesh(FbxNode* pNode, Mesh* CoreMesh);


	private:
		static  FBXManager* m_Instance;

		//�� Ŭ���� �ƴ� sdk���� �����ϴ� FbxManager
		FbxManager*	m_pManager;
		FbxScene*	m_pScene;


	};
}