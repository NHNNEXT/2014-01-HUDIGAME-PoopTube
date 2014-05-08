#pragma once
#include "Node.h"

namespace pooptube{
	class XMesh : public Node {
	public:
		XMesh();
		virtual ~XMesh();

		static XMesh *Create(const std::string& FilePath);

		//�׳� �����Ƽ� ���ҽ� �޴��� �Ȱ�ġ�� �� ��������
		bool Init(const std::string& FilePath);

		void Update(float dTime);
		void Render();

	private:
		LPD3DXMESH			mXMesh = nullptr;
		D3DMATERIAL9*		mMaterial = nullptr;
		LPDIRECT3DTEXTURE9* mTexture = nullptr;

		DWORD				mNumMaterial = 0;
	};
}



