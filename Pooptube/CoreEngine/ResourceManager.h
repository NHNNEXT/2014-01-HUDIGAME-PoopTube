/**
* @author 양현찬
* @brief 
* 리소스를 로드하고 로드된 리소스는 헤쉬에 보관하는 객체
* fbxsdk관련 객체들 관리
* 싱글톤구조, 오브젝트 메니져에서 관리
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

		// agebreak : Input 값에는 const를 달것. 수정되는지 헷갈림
		void CalculateNormal(D3DXVECTOR3* pOut, D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2);

	protected:
		bool _Init();
		bool _FBXInit();

		Mesh* _LoadFBXFile(const std::string& FilePath);
		Mesh* _LoadHeightMap(const std::string& FilePath);

		//@brief 
		//임시로 매쉬만 뽑아내도록 만든 함수
		//머테리얼 정보는 임시로 초록색으로 지정
		//@todo
		//fbx파일의 노드를 순회하면서 메쉬, 에니메이션 캐쉬, 텍스쳐 등의 정보를 뽑아
		//만든 자료형에 보관하는 기능을 구현해야 한다.
		Mesh* _ReadVerticesFromFBX(FbxScene* pScene);



	private:

		static ResourceManager*			mInstance;
		LPDIRECT3DDEVICE9				mDevice;

		//@brief
		//sdk에서 제공하는 FbxManager
		FbxManager*						mManager = nullptr;

		//@brief
		//로드된 fbxmesh정보를 쥐고있는 meshtable
		std::map<std::string, Mesh*>	mFBXMeshTable;
		std::map<std::string, Mesh*>	mHeightMapTable;


	};

}