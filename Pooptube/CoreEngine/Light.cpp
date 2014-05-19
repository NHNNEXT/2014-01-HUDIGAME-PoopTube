#include "stdafx.h"
#include "Light.h"

namespace pooptube {

	DWORD Light::mCountIndex = 0;

	Light::Light() {

	}

	Light::~Light() {

	}

	Light *Light::Create() {
		Light *pLight = new Light;
		if (pLight->Init())
			return pLight;
		else
			return nullptr;
	}

	bool Light::Init() {
		Node::Init();

		mObjectName = "Light" + std::to_string(Node::ObjectNum-1);
		mClassName = "Light";

		mIndex = mCountIndex++;

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

		SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f));
		mD3DLight.Range = 12.f;
		
		GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
		//연산량을 줄이기 위해 미리 엠비언트라이트를 먹여버린다.
		GetDevice()->SetRenderState(D3DRS_AMBIENT, 0x00202020);
		//GetDevice()->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

		return true;
	}

	void Light::Update(float dTime) {
		Node::Update(dTime);

	}

	void Light::Render() {
		mD3DLight.Position = GetPosition();
		mD3DLight.Position.y += 2.f;
		Node::Render();
		GetDevice()->SetLight(mIndex, &mD3DLight);
		GetDevice()->LightEnable(mIndex, mLightSwitch);
	}

}
