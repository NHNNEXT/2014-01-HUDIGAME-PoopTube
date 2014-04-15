
#include "stdafx.h"
#include "D3D9Renderer.h"

namespace pooptube {

	D3D9Renderer::D3D9Renderer() : mD3D(nullptr), mD3DDevice(nullptr) {}
	D3D9Renderer::~D3D9Renderer() {
		mD3D->Release();
		mD3DDevice->Release();
	}

	bool D3D9Renderer::Init( HWND hwnd, int width, int height, bool isWindowed ) {
		D3DPRESENT_PARAMETERS d3dPresentParameters;

		mD3D = nullptr;
		mD3DDevice = nullptr;

		mD3D = Direct3DCreate9( D3D_SDK_VERSION );

		ZeroMemory( &d3dPresentParameters, sizeof(d3dPresentParameters) );

		D3DMULTISAMPLE_TYPE mst = D3DMULTISAMPLE_NONE;
		/* 샘플링
		switch(4)
		{
		case 1: mst = D3DMULTISAMPLE_NONMASKABLE;break;
		case 2: mst = D3DMULTISAMPLE_2_SAMPLES;break;
		case 3: mst = D3DMULTISAMPLE_3_SAMPLES;break;
		case 4: mst = D3DMULTISAMPLE_4_SAMPLES;break;
		case 5: mst = D3DMULTISAMPLE_5_SAMPLES;break;
		case 6: mst = D3DMULTISAMPLE_6_SAMPLES;break;
		case 7: mst = D3DMULTISAMPLE_7_SAMPLES;break;
		case 8: mst = D3DMULTISAMPLE_8_SAMPLES;break;
		case 9: mst = D3DMULTISAMPLE_9_SAMPLES;break;
		case 10: mst = D3DMULTISAMPLE_10_SAMPLES;break;
		case 11: mst = D3DMULTISAMPLE_11_SAMPLES;break;
		case 12: mst = D3DMULTISAMPLE_12_SAMPLES;break;
		case 13: mst = D3DMULTISAMPLE_13_SAMPLES;break;
		case 14: mst = D3DMULTISAMPLE_14_SAMPLES;break;
		case 15: mst = D3DMULTISAMPLE_15_SAMPLES;break;
		case 16: mst = D3DMULTISAMPLE_16_SAMPLES;break;
		default: mst = D3DMULTISAMPLE_NONE;break;
		}
		*/
		d3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dPresentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dPresentParameters.BackBufferWidth = width;
		d3dPresentParameters.BackBufferHeight = height;
		d3dPresentParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;// D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dPresentParameters.hDeviceWindow = hwnd;
		d3dPresentParameters.Windowed = isWindowed;
		
		//깊이 버퍼 설정 추가
		d3dPresentParameters.EnableAutoDepthStencil = TRUE;
		d3dPresentParameters.AutoDepthStencilFormat = D3DFMT_D16;

		if( SUCCEEDED(mD3D->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT,
		  D3DDEVTYPE_HAL, d3dPresentParameters.BackBufferFormat, isWindowed, mst, NULL )) ) { 
			d3dPresentParameters.MultiSampleType = mst;
		}
		else { 
			return false;
		}

		if( FAILED(mD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		  D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dPresentParameters, &mD3DDevice )) ) {
			return false;
		}

		// 컬링을 설정합니다.
		mD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		// z버퍼를 사용합니다.
		mD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

		return true;
	}
	
	bool D3D9Renderer::DrawBegin() {
		if ( mD3DDevice == nullptr )
			return false;

		if( FAILED(mD3DDevice->BeginScene()) )
			return false;

		mD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		return true;
	}
	bool D3D9Renderer::DrawEnd() {
		if ( mD3DDevice == nullptr )
			return false;

		if( FAILED(mD3DDevice->EndScene()) )
			return false;

		if( FAILED(mD3DDevice->Present( NULL, NULL, NULL, NULL )) )
			return false;

		return true;
	}
	bool D3D9Renderer::Clear() {
		if ( mD3DDevice == nullptr )
			return false;

		//z버퍼 추가
		if( FAILED(mD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,0,0,0), 1.0f, 0 )) )
			return false;

		return true;
	}
	bool D3D9Renderer::ToggleFullscreen() {
		return true;
	}
}