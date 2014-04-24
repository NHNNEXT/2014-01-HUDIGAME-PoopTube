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
	class HeightMapData;

	class ResourceManager : public Object {
	public:
		ResourceManager();
		~ResourceManager();

		static ResourceManager* GetInstance();

		std::shared_ptr<Mesh>			LoadMeshFromFBX(const std::string& FilePath);
		std::shared_ptr<HeightMapData>	LoadHeightMap(const std::string& FilePath);

		LPDIRECT3DTEXTURE9 LoadTexture(const std::wstring& FilePath);

	protected:
		bool _Init();
		bool _FBXInit();

		std::shared_ptr<Mesh> _LoadFBXFile(const std::string& FilePath);

		//@brief 
		//�ӽ÷� �Ž��� �̾Ƴ����� ���� �Լ�
		//���׸��� ������ �ӽ÷� �ʷϻ����� ����
		//@todo
		//fbx������ ��带 ��ȸ�ϸ鼭 �޽�, ���ϸ��̼� ĳ��, �ؽ��� ���� ������ �̾�
		//���� �ڷ����� �����ϴ� ����� �����ؾ� �Ѵ�.
		std::shared_ptr<Mesh> _ReadVerticesFromFBX(FbxScene* pScene);



	private:

		static ResourceManager*			mInstance;
		LPDIRECT3DDEVICE9				mDevice;

		//@brief
		//sdk���� �����ϴ� FbxManager
		FbxManager*						mManager = nullptr;

		//@brief
		//�ε�� fbxmesh������ ����ִ� meshtable
		std::map<std::string, std::shared_ptr<Mesh>>			mFBXMeshTable;
		std::map<std::string, std::shared_ptr<HeightMapData>>	mHeightMapTable;
		std::map<std::wstring, LPDIRECT3DTEXTURE9>				mTextureTable;


	};

}