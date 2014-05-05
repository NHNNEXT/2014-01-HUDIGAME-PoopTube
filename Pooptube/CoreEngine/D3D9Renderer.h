﻿
#pragma once

#include "Object.h"
#include <array>

namespace pooptube {

	class D3D9Renderer : public Object {
	public:
		bool Init( HWND hwnd, int width, int height, bool isWindowed );
		//bool Release();
		bool DrawBegin();
		bool DrawEnd();
		bool Clear();
		bool ToggleFullscreen();

		LPDIRECT3D9 GetD3D() const { return mD3D; }
		LPDIRECT3DDEVICE9 GetDevice() const { return mD3DDevice; }
		std::array<D3DXPLANE, 6> GetFrustumPlane() const { return mFrustumPlanes; }

		//테스트용
		UINT mRenderCount = 0;
	private:
		D3D9Renderer();
		~D3D9Renderer();

	private:
		LPDIRECT3D9 mD3D;
		LPDIRECT3DDEVICE9 mD3DDevice;

		std::array<D3DXPLANE, 6> mFrustumPlanes;
		BOOL MakeFrustumPlanes( D3DXMATRIXA16* pmatViewProj );

		friend class SceneManager;
	};
}