// CLICore.h

#pragma once

#include "Object.h"

using namespace System;

namespace Core {

	public ref class D3D9Renderer
	{
	public :
		bool Init(HWND hwnd, int width, int height, bool isWindowed);
		//bool Release();
		bool DrawBegin();
		bool DrawEnd();
		bool Clear();
		bool ToggleFullscreen();

		LPDIRECT3D9 GetD3D() const { return mD3D; }
		LPDIRECT3DDEVICE9 GetDevice() const { return mD3DDevice; }

	private:
		D3D9Renderer();
		~D3D9Renderer();

	private:
		LPDIRECT3D9 mD3D;
		LPDIRECT3DDEVICE9 mD3DDevice;

		D3D9Renderer *mpInstance = nullptr;
	};
}
