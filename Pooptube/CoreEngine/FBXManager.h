#pragma once
#include "stdafx.h"
#include "Object.h"

namespace pooptube {

	class Mesh;

	//fbx�� ���� �����ڷ�� �� ���� ��Ű�� �ø����� �ϰڽ��ϴ�.
	class FBXManager : public Object
	{
	public:

		FBXManager(void);
		~FBXManager(void);

		//�ϴ� ��Ŭ������ ���� fbxmanager���������� ���� �����غ��� ��
		static FBXManager* GetInstance();

		//FBX������ �ε��ϴ� �Լ� �ε��� �� FBXScene�� ��ȯ�Ѵ�.
		FbxScene* LoadFBXFile(const char* pFileName);

	protected:

		//FBXManager�� create�ϰ� iosetting�� �Ѵ�.
		//������ �ڵ�ȣ��
		bool _Init();

		//fbx������ �⺻������ node�� Ʈ�������� �̷�� �ֽ��ϴ�.
		//��Ŀ�ú��ϰ� ���鼭 fbx������ ��� node�� �н��ϴ�.
		//���ɻ� ����Լ��� ������ Ǫ�� ����� �����ؾ���
		void _LoadNodeRecursive(FbxNode* pNode, Mesh* CoreMesh);
		void _LoadNode(FbxNode* pNode, Mesh* CoreMesh);
		void _LoadMesh(FbxNode* pNode, Mesh* CoreMesh);


	private:
		static  FBXManager* mInstance;

		//�� Ŭ���� �ƴ� sdk���� �����ϴ� FbxManager
		FbxManager*	mManager;
		//FbxScene*	mScene;


	};
}