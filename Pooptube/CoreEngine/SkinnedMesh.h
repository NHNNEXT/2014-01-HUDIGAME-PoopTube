/**
* @author 양현찬
* @brief 버텍스, 에니메이션, 텍스쳐 등의 정보를 읽어와 렌더하는 객체
* @date 2014/04/17
* @file SkinnedMesh.h
*/
#pragma once
#include "Node.h"

namespace pooptube {

	//전방선언
	class Mesh;

	//@brief 
	//팩토리 구조. Mesh를 생성한후 objmanager에서 관리
	//사용자가 직접 접근하는 함수
	class SkinnedMesh : public Node {
	public:
		SkinnedMesh();
		virtual ~SkinnedMesh();

		//@param VertexCount 
		//@param PolygonCount
		//@return 생성한 Mesh를 반환
		static SkinnedMesh* Create(std::string FilePath);
		virtual bool Init(std::string FilePath);

		virtual void Render();
		virtual void Update(float dTime);

		LPDIRECT3DVERTEXBUFFER9 GetMeshVertexBuffer() const { return mMeshVertexBuffer; }

	protected:



	private:
		//메쉬의 버텍스 값을 저장하는 버텍스 버퍼
		LPDIRECT3DVERTEXBUFFER9 mMeshVertexBuffer;
		LPDIRECT3DINDEXBUFFER9	mMeshIndexBuffer;

		Mesh*		mMesh;
	};

}