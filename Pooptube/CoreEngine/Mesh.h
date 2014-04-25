/**
* @author 양현찬
* @brief 메쉬의 데이터를 물고있는 객체
* 팩토리 구조. Mesh를 생성한후 objmanager에서 관리
* 사용자가 직접 접근할 일은 없음
* @date 2014/04/17
* @file Mesh.h
*/
#pragma once
#include "Node.h"

namespace pooptube {

	//@brief 커스텀 버텍스를 설정
	const int D3DFVF_CUSTOMVERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;

	//@brief 커스텀 버텍스
	struct MESH_CUSTOM_VERTEX {
		D3DXVECTOR3	position;
		D3DXVECTOR3	normal;
		D3DCOLOR	color;
	};

	//@brief 
	//커스텀 인덱스
	//현제 16비트 크기 설정 조정시 32비트로 조정 필요
	struct MESH_CUSTOM_INDEX {
		UINT w0, w1, w2;
	};

	class Mesh : public Node
	{
	public:
		Mesh();
		virtual ~Mesh();

		//@param VertexCount 
		//@param PolygonCount
		//@return 생성한 Mesh를 반환
		//@exception 초기화에 실패하면 nullptr반환
		static std::shared_ptr<Mesh> Create(int VertexCount, int PolygonCount);

		virtual bool Init(int VertexCount, int PolygonCount);

		virtual void Render();
		virtual void Update(float dTime);

		int						GetPolygonCount() const { return mPolygonCount; }
		int						GetVertexCount() const { return mVertexCount; }
		MESH_CUSTOM_VERTEX*		GetVertices() const { return mVertices; }
		MESH_CUSTOM_INDEX*		GetIndices() const { return mIndices; }

	protected:


	private:

		UINT							mPolygonCount = 0;
		UINT							mVertexCount = 0;

		MESH_CUSTOM_VERTEX*				mVertices = nullptr;
		MESH_CUSTOM_INDEX*				mIndices = nullptr;
		
		LPDIRECT3DDEVICE9				mDevice = nullptr;

		D3DMATERIAL9					mMaterial;

		float							mHeightMapSize = 0.5f;
	};

}
