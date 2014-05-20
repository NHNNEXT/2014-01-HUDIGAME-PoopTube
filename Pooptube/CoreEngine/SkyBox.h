#pragma once
#include "Node.h"

namespace pooptube {

	const int D3DFVF_CUSTOMVERTEX_SKYBOX = D3DFVF_XYZ | D3DFVF_TEX1;

	struct SKYBOX_CUSTOM_VERTEX {
		D3DXVECTOR3	position;
		float		tu, tv;
	};

	class SkyBox : public Node{
	public:
		SkyBox();
		virtual ~SkyBox();

		static SkyBox *Create(const std::string& top,
			const std::string& bottom,
			const std::string& front,
			const std::string& back,
			const std::string& left,
			const std::string& right);

		void Render();
		void Update(float dTime);


	protected:
		bool _Init(const std::string& top,
			const std::string& bottom,
			const std::string& front,
			const std::string& back,
			const std::string& left,
			const std::string& right);

	private:
		LPDIRECT3DTEXTURE9 mTopTexture = nullptr;
		LPDIRECT3DTEXTURE9 mBotTexture = nullptr;
		LPDIRECT3DTEXTURE9 mFrontTexture = nullptr;
		LPDIRECT3DTEXTURE9 mBackTexture = nullptr;
		LPDIRECT3DTEXTURE9 mLeftTexture = nullptr;
		LPDIRECT3DTEXTURE9 mRightTexture = nullptr;

		LPDIRECT3DVERTEXBUFFER9 mVertexBuffer = nullptr;
		LPDIRECT3DINDEXBUFFER9	mIndexBuffer = nullptr;

		float				mWidth = 1000.f;
		float				mHeight = 1000.f;
		float				mDepth = 1000.f;

	};

}