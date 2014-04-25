#include "stdafx.h"
#include "Light.h"

namespace pooptube {

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

		D3DMATERIAL9 mtrl;
		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Specular.r = mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
		mtrl.Specular.g = mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
		mtrl.Specular.b = mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
		mtrl.Specular.a = mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
		GetDevice()->SetMaterial(&mtrl);

		D3DXVECTOR3 vecDir;
		D3DLIGHT9 light;

		//광원의 위치
		vecDir = D3DXVECTOR3(10.f, -10.f, -10.f);

		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 1.0f;
		light.Diffuse.b = 1.0f;

		D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
		light.Range = 1000.0f;

		//디바이스에 광원을 설정합니다.
		GetDevice()->SetLight(0, &light);
		GetDevice()->LightEnable(0, TRUE);

		GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
		GetDevice()->SetRenderState(D3DRS_AMBIENT, 0x00202020);

		return true;
	}

	void Light::Update(float dTime) {

	}

}
