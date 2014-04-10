#pragma once
#include "Mesh.h"

// namespace pooptube {
// 
// 	class FBXMesh : public Mesh
// 	{
// 	public:
// 		FBXMesh();
// 		virtual ~FBXMesh();
// 
// 		CREATE_FUNC(FBXMesh);
// 
// 		virtual bool Init();
// 
// 		virtual void Render();
		virtual void Update(float dTime);
// 
// 		LPDIRECT3DVERTEXBUFFER9 GetMeshVertexBuffer() const { return m_MeshVertexBuffer; }
// 
// 	protected:
// 
// 	private:
// 		//메쉬의 버텍스 값을 저장하는 버텍스 버퍼
		LPDIRECT3DVERTEXBUFFER9 m_MeshVertexBuffer;
// 
// 	};
// 
// }
