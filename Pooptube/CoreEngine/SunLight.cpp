#include "stdafx.h"
#include "SunLight.h"

namespace pooptube {

	SunLight::SunLight() {
	}

	SunLight::~SunLight() {
	}

	std::shared_ptr<SunLight> SunLight::Create() {
		std::shared_ptr<SunLight> pSunLight(new SunLight);
		if (pSunLight->Init())
			return pSunLight;
		else
			return nullptr;
	}

	bool SunLight::Init() {
		if (!Light::Init())
			return false;

		mD3DLight.Type = D3DLIGHT_DIRECTIONAL;
		SetFrontPoint(D3DXVECTOR3(1.f, -1.f, -1.f));
		return true;
	}

	void SunLight::Render() {
		D3DXVec3Normalize((D3DXVECTOR3*)&mD3DLight.Direction, &Node::GetFrontPoint());
		Light::Render();
	}

	void SunLight::Update(float dTime) {
		Light::Update(dTime);
	}



}

