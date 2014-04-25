#include "stdafx.h"
#include "Light.h"

namespace pooptube {

	DWORD Light::mCountIndex = 0;

	Light::Light() {

	}

	Light::~Light() {

	}

	std::shared_ptr<Light> Light::Create() {
		std::shared_ptr<Light> pLight(new Light);
		if (pLight->Init())
			return pLight;
		else
			return nullptr;
	}

	bool Light::Init() {
		Node::Init();

		mIndex = mCountIndex++;

		//이건 메쉬쪽으로
// 		D3DMATERIAL9 mtrl;
// 		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
// 		mtrl.Specular.r = mtrl.Diffuse.r = mtrl.Ambient.r = 0.0f;
// 		mtrl.Specular.g = mtrl.Diffuse.g = mtrl.Ambient.g = 0.0f;
// 		mtrl.Specular.b = mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
// 		mtrl.Specular.a = mtrl.Diffuse.a = mtrl.Ambient.a = 0.0f;
// 		GetDevice()->SetMaterial(&mtrl);

		//광원의 위치
		ZeroMemory(&mD3DLight, sizeof(D3DLIGHT9));
		//mD3DLight.Type = D3DLIGHT_DIRECTIONAL;
		
		mD3DLight.Type = D3DLIGHT_POINT;

		mD3DLight.Specular.r = 1.0f;
		mD3DLight.Specular.g = 1.0f;
		mD3DLight.Specular.b = 1.0f;
				
		mD3DLight.Diffuse.r = 0.5f;
		mD3DLight.Diffuse.g = 0.5f;
		mD3DLight.Diffuse.b = 0.5f;

		SetPosition(D3DXVECTOR3(10.f, 10.f, 10.f));
		mD3DLight.Range = 12.f;

		GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
		//연산량을 줄이기 위해 미리 엠비언트라이트를 먹여버린다.
		GetDevice()->SetRenderState(D3DRS_AMBIENT, 0x00202020);

		return true;
	}

	void Light::Update(float dTime) {
		Node::Update(dTime);

	}

	void Light::Render() {
		mD3DLight.Position = GetPosition();
		Node::Render();
		GetDevice()->SetLight(mIndex, &mD3DLight);
		GetDevice()->LightEnable(mIndex, mLightSwitch);
	}

}
