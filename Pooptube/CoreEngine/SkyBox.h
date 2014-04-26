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

		static std::shared_ptr<SkyBox> Create(const std::wstring& top,
			const std::wstring& bottom,
			const std::wstring& front,
			const std::wstring& back,
			const std::wstring& left,
			const std::wstring& right);

		void Render();
		void Update(float dTime);


	protected:
		bool _Init(const std::wstring& top,
			const std::wstring& bottom,
			const std::wstring& front,
			const std::wstring& back,
			const std::wstring& left,
			const std::wstring& right);

	private:
		LPDIRECT3DTEXTURE9 mTopTexture = nullptr;
		LPDIRECT3DTEXTURE9 mBotTexture = nullptr;
		LPDIRECT3DTEXTURE9 mFrontTexture = nullptr;
		LPDIRECT3DTEXTURE9 mBackTexture = nullptr;
		LPDIRECT3DTEXTURE9 mLeftTexture = nullptr;
		LPDIRECT3DTEXTURE9 mRightTexture = nullptr;

		LPDIRECT3DVERTEXBUFFER9 mVertexBuffer = nullptr;
		LPDIRECT3DINDEXBUFFER9	mIndexBuffer = nullptr;

		float				mWidth = 100.f;
		float				mHeight = 100.f;
		float				mDepth = 100.f;

	};

}