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

	class Ground;
	class MeshData;
	class XMeshData;

	class ResourceManager : public Object {
	public:
		ResourceManager();
		~ResourceManager();

		static ResourceManager* GetInstance();

		MeshData*			LoadSkinnedMesh(const std::wstring& FilePath);
		XMeshData*			LoadXMesh(const std::wstring& FilePath);
		Ground::MapData*	LoadHeightMap(const std::wstring& FilePath);
		LPDIRECT3DTEXTURE9	LoadTexture(const std::wstring& FilePath);
		ID3DXEffect*		LoadHLSL(const std::wstring& FilePath);

	protected:
		bool _Init();

	private:

		static ResourceManager*			mInstance;
		LPDIRECT3DDEVICE9				mDevice;

		std::map<std::wstring, Ground::MapData*>		mHeightMapTable;
		std::map<std::wstring, LPDIRECT3DTEXTURE9>		mTextureTable;
		std::map<std::wstring, ID3DXEffect*>			mHLSLShaderTable;
		std::map<std::wstring, MeshData*>				mMeshDataTable;
		std::map<std::wstring, XMeshData*>				mXMeshDataTable;

		//안쓰는 넘들
		FbxManager*									mManager = nullptr;
	};

}