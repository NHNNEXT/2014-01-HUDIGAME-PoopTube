/**
* @author ������
* @brief 
* ���ҽ��� �ε��ϰ� �ε�� ���ҽ��� �콬�� �����ϴ� ��ü
* fbxsdk���� ��ü�� ����
* �̱��汸��, ������Ʈ �޴������� ����
* @date 2014/04/17
* @file ResourceManager.h
*/
#pragma once
#include "stdafx.h"
#include "Object.h"


namespace pooptube {

	class Mesh;

	class ResourceManager : public Object {
	public:
		ResourceManager();
		~ResourceManager();

		static ResourceManager* GetInstance();

		Mesh* LoadMeshFromFBX(const std::string& FilePath);
		Mesh* LoadMeshFromHeightMap(const std::string& FilePath);

		// agebreak : Input ������ const�� �ް�. �����Ǵ��� �򰥸�
		void CalculateNormal(D3DXVECTOR3* pOut, D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2);

	protected:
		bool _Init();
		bool _FBXInit();

		Mesh* _LoadFBXFile(const std::string& FilePath);
		Mesh* _LoadHeightMap(const std::string& FilePath);

		//@brief 
		//�ӽ÷� �Ž��� �̾Ƴ����� ���� �Լ�
		//���׸��� ������ �ӽ÷� �ʷϻ����� ����
		//@todo
		//fbx������ ��带 ��ȸ�ϸ鼭 �޽�, ���ϸ��̼� ĳ��, �ؽ��� ���� ������ �̾�
		//���� �ڷ����� �����ϴ� ����� �����ؾ� �Ѵ�.
		Mesh* _ReadVerticesFromFBX(FbxScene* pScene);



	private:

		static ResourceManager*			mInstance;
		LPDIRECT3DDEVICE9				mDevice;

		//@brief
		//sdk���� �����ϴ� FbxManager
		FbxManager*						mManager = nullptr;

		//@brief
		//�ε�� fbxmesh������ ����ִ� meshtable
		std::map<std::string, Mesh*>	mFBXMeshTable;
		std::map<std::string, Mesh*>	mHeightMapTable;


	};

}