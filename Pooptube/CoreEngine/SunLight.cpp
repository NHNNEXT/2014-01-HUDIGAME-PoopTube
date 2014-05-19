#include "stdafx.h"
#include "SunLight.h"

namespace pooptube {

	SunLight::SunLight() {
	}

	SunLight::~SunLight() {
	}

	SunLight *SunLight::Create() {
		SunLight *pSunLight(new SunLight);
		if (pSunLight->Init())
			return pSunLight;
		else
			return nullptr;
	}

	bool SunLight::Init() {
		if (!Light::Init())
			return false;

		mD3DLight.Type = D3DLIGHT_DIRECTIONAL;
		SetFrontVector(D3DXVECTOR3(1.f, -1.f, -1.f));

		mObjectName = "SunLight" + std::to_string(Node::ObjectNum-1);
		mClassName = "SunLight";

		return true;
	}

	void SunLight::Render() {
		D3DXVec3Normalize((D3DXVECTOR3*)&mD3DLight.Direction, &Node::GetFrontVector());
		Light::Render();
	}

	void SunLight::Update(float dTime) {
		Light::Update(dTime);
	}



}

