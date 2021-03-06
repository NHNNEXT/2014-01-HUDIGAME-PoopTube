﻿
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
		//d3dPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		d3dPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
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

		//빛을끈다.
		mD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

		// 컬링을 설정합니다.
		mD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		// z버퍼를 사용합니다.
		mD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

		// 알파 테스팅 사용
		mD3DDevice->SetRenderState( D3DRS_ALPHAREF, 0x00 );
		mD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
		mD3DDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );

		// 알파 블랜딩 사용
		//mD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		mD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		mD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		// 텍스쳐 필터링 적용
		mD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		mD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

		//뷰변환에서 모든 법선을 정규화시키는 설정
		//이거 안하면 일부 노멀이 안먹히는 현상이 벌어짐
		//skinned mesh같은거
		mD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		//디더링 색혼합설정
		//mD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
		//mD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x33333333);

		return true;
	}
	
	bool D3D9Renderer::DrawBegin() {
		if ( mD3DDevice == nullptr )
			return false;

		if( FAILED(mD3DDevice->BeginScene()) )
			return false;

		//절두체 컬링
		D3DXMATRIXA16 matViewProj, matProj;
		mD3DDevice->GetTransform( D3DTS_VIEW, &matViewProj );
		mD3DDevice->GetTransform( D3DTS_PROJECTION, &matProj );
		matViewProj *= matProj;
		MakeFrustumPlanes( &matViewProj );

		mRenderedMeshNum = 0; // 테스트용

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

	bool D3D9Renderer::MakeFrustumPlanes( D3DXMATRIXA16* pmatViewProj )
	{
		D3DXMATRIXA16	matInv;
		
		D3DXVECTOR3 m_vtx[8];
		m_vtx[0].x = -1.0f;	m_vtx[0].y = -1.0f;	m_vtx[0].z = 0.0f;
		m_vtx[1].x = 1.0f;	m_vtx[1].y = -1.0f;	m_vtx[1].z = 0.0f;
		m_vtx[2].x = 1.0f;	m_vtx[2].y = -1.0f;	m_vtx[2].z = 1.0f;
		m_vtx[3].x = -1.0f;	m_vtx[3].y = -1.0f;	m_vtx[3].z = 1.0f;
		m_vtx[4].x = -1.0f;	m_vtx[4].y = 1.0f;	m_vtx[4].z = 0.0f;
		m_vtx[5].x = 1.0f;	m_vtx[5].y = 1.0f;	m_vtx[5].z = 0.0f;
		m_vtx[6].x = 1.0f;	m_vtx[6].y = 1.0f;	m_vtx[6].z = 1.0f;
		m_vtx[7].x = -1.0f;	m_vtx[7].y = 1.0f;	m_vtx[7].z = 1.0f;
		
		D3DXMatrixInverse( &matInv, NULL, pmatViewProj );
		
		for( int i = 0; i < 8; i++ )
			D3DXVec3TransformCoord( &m_vtx[i], &m_vtx[i], &matInv );
		
		D3DXPlaneFromPoints( &mFrustumPlanes[0], m_vtx + 2, m_vtx + 6, m_vtx + 7 );		// far
		D3DXPlaneFromPoints( &mFrustumPlanes[1], m_vtx, m_vtx + 3, m_vtx + 7 );			// left
		D3DXPlaneFromPoints( &mFrustumPlanes[2], m_vtx + 1, m_vtx + 5, m_vtx + 6 );		// right
//		D3DXPlaneFromPoints( &mFrustumPlanes[3], m_vtx + 4, m_vtx + 7, m_vtx + 6 );		// top
//		D3DXPlaneFromPoints( &mFrustumPlanes[4], m_vtx, m_vtx + 1, m_vtx + 2 );			// bottom
//		D3DXPlaneFromPoints( &mFrustumPlanes[5], m_vtx, m_vtx + 4, m_vtx + 5 );			// near

		return true;
	}

}