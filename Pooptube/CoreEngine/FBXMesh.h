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
// 		//�޽��� ���ؽ� ���� �����ϴ� ���ؽ� ����
		LPDIRECT3DVERTEXBUFFER9 m_MeshVertexBuffer;
// 
// 	};
// 
// }
