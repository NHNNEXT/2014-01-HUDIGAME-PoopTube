/**
* @author 이선협
* @version 2014/04/28 김지환 - HeightMapData 삭제
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
#include "Ground.h"

namespace pooptube {

	class Mesh;
	class Ground;

	class ResourceManager : public Object {
	public:
		ResourceManager();
		~ResourceManager();

		static ResourceManager* GetInstance();

		std::shared_ptr<Mesh>				LoadMeshFromFBX(const std::string& FilePath);
		std::shared_ptr<Ground::MapData>	LoadHeightMap(const std::string& FilePath);
		LPDIRECT3DTEXTURE9					LoadTexture(const std::wstring& FilePath);
		//처참한 fbx의 흔적
		//FbxScene*						LoadFBX(const std::string &FilePath);

	protected:
		bool _Init();
		bool _FBXInit();

		std::shared_ptr<Mesh>	_LoadFBXFile(const std::string& FilePath);

		//@brief 
		//임시로 매쉬만 뽑아내도록 만든 함수
		//머테리얼 정보는 임시로 초록색으로 지정
		//@todo
		//fbx파일의 노드를 순회하면서 메쉬, 에니메이션 캐쉬, 텍스쳐 등의 정보를 뽑아
		//만든 자료형에 보관하는 기능을 구현해야 한다.
		std::shared_ptr<Mesh> _ReadVerticesFromFBX(FbxScene* pScene);

	private:

		static ResourceManager*			mInstance;
		LPDIRECT3DDEVICE9				mDevice;

		//@brief
		//sdk에서 제공하는 FbxManager
		FbxManager*						mManager = nullptr;

		//@brief
		//로드된 fbxmesh정보를 쥐고있는 meshtable
		//아직 특정 data 제거기능을 안넣음
		std::map<std::string, std::shared_ptr<Mesh>>			mFBXMeshTable;
		std::map<std::string, std::shared_ptr<Ground::MapData>>			mHeightMapTable;
		std::map<std::wstring, LPDIRECT3DTEXTURE9>				mTextureTable;

		//std::map<std::string, FbxScene*>						mFBXTable;
	};

}