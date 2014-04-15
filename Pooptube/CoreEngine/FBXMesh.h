#pragma once
#include "Node.h"

namespace pooptube {

	//커스텀 버텍스를 설정
	const int D3DFVF_CUSTOMVERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;

	//매쉬에서 사용되는 커스텀 버텍스입니다.
	//이걸 여기둬두 되나? 누가 태클쩜
	struct MESH_CUSTOM_VERTEX {
		D3DXVECTOR3	position;
		D3DXVECTOR3	normal;
		D3DCOLOR	color;
	};

	struct MESH_CUSTOM_INDEX {
		//현제 16비트 크기 설정 조정시 32비트로 조정 필요
		WORD w0, w1, w2;
	};

	class FBXMesh : public Node
	{
	public:
		FBXMesh();
		virtual ~FBXMesh();

		//팩토리 구조, 생성을 위해서는 FBXManager에서 생성한 FBXScene을 인자로 넘겨야 한다.
		static FBXMesh* Create(int VertexCount, int PolygonCount);

		virtual bool Init(int VertexCount, int PolygonCount);

		virtual void Render();
		virtual void Update(float dTime);

		int						GetPolygonCount() const { return mPolygonCount; }
		int						GetVertexCount() const { return mVertexCount; }
		MESH_CUSTOM_VERTEX*		GetVertices() const { return mVertices; }
		MESH_CUSTOM_INDEX*		GetIndices() const { return mIndices; }

		UINT					GetCountHandle() { return ++mCountHandle; }

	protected:


	private:
		//메쉬의 폴리곤 갯수
		int								mPolygonCount;
		int								mVertexCount;
	

		//몇게의 skinnedMesh가 자신을 참조하고 있는지
		UINT							mCountHandle;

		MESH_CUSTOM_VERTEX*				mVertices;
		MESH_CUSTOM_INDEX*				mIndices;
		

	};

}
