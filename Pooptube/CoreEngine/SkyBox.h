﻿#pragma once
#include "Node.h"

namespace pooptube {

	const int D3DFVF_CUSTOMVERTEX_SKYBOX = D3DFVF_XYZ | D3DFVF_TEX1;

	struct SKYBOX_CUSTOM_VERTEX {
		D3DXVECTOR3	position;
		float		tu, tv;
	};

	//todo : dp콜 줄이는 방향으로 변환
	class SkyBox : public Node{
	public:
		SkyBox();
		virtual ~SkyBox();

		static SkyBox *Create(const std::wstring& top,
			const std::wstring& bottom,
			const std::wstring& front,
			const std::wstring& back,
			const std::wstring& left,
			const std::wstring& right);

		void Render();
		void Update(float dTime);

		void SetTarget(Node* val) { mTarget = val; }

		void Vibrater(float dTime);

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

		ID3DXEffect*	   mEffect = nullptr;

		LPDIRECT3DVERTEXBUFFER9 mVertexBuffer = nullptr;
		LPDIRECT3DINDEXBUFFER9	mIndexBuffer = nullptr;

		float				mWidth = 200.f;
		float				mHeight = 200.f;
		float				mDepth = 200.f;

		bool				mSwitchVibe = true;
		float				mVibeSpeed = 0.03f;
		float				mVibeCurrentTime = 0.f;

		Node*				mTarget = nullptr;

	};

}