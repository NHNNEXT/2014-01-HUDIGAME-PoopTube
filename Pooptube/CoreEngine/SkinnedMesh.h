/**
* @author 양현찬
* @brief
* 버텍스, 에니메이션, 텍스쳐 등의 정보를 읽어와 렌더하는 객체
* 팩토리 구조. Mesh를 생성한후 objmanager에서 관리
* 사용자가 직접 접근하는 함수
* @date 2014/04/17
* @file SkinnedMesh.h
*/
#pragma once
#include "Node.h"

namespace pooptube {

	class Mesh;

	class SkinnedMesh : public Node {
	public:
		SkinnedMesh();
		virtual ~SkinnedMesh();

		//@param VertexCount 
		//@param PolygonCount
		//@param ResourceType
		//@return 생성한 Mesh를 반환
		//@exception 초기화에 실패하거나 리소스 타입이 null일 경우 nullptr반환
		static SkinnedMesh *Create(const std::string& MeshFilePath);
		virtual bool Init(const std::string& MeshFilePath);

		virtual void Render();
		virtual void Update(float dTime);

		Mesh *GetMeshData() const { return mMesh; }

	protected:
		bool _InitFBX(const std::string& MeshFilePath);

		std::vector<D3DMATERIAL9>		mMaterial;
		std::vector<LPDIRECT3DTEXTURE9>	mTexture;

	private:

		LPDIRECT3DVERTEXBUFFER9			mMeshVertexBuffer = nullptr;
		LPDIRECT3DINDEXBUFFER9			mMeshIndexBuffer = nullptr;
		Mesh			*mMesh = nullptr;
	};

}