
#include "stdafx.h"
#include "ThirdPersonCamera.h"

#include "ObjectManager.h"
#include "Application.h"

namespace pooptube {
	ThirdPersonCamera::ThirdPersonCamera() {
	}
	ThirdPersonCamera::~ThirdPersonCamera() {
	}

	ThirdPersonCamera *ThirdPersonCamera::Create(Node *Target) {
		ThirdPersonCamera *pThirdPersonCamera(new ThirdPersonCamera);

		if (pThirdPersonCamera->Init(Target)) {
			return pThirdPersonCamera;
		}
		else 
			return nullptr;
	}

	bool ThirdPersonCamera::Init(Node *Target) {
		Camera::Init();
		mTarget = Target;

		mObjectName = "ThirdPersonCamera" + std::to_string(Node::ObjectNum-1);
		mClassName = "ThirdPersonCamera";

		return true;
	}
	
	void ThirdPersonCamera::Render() {
		Camera::Render();
	}

	void ThirdPersonCamera::Update(float dTime) {
		Camera::Update(dTime);

		D3DXVECTOR3 LookPt = mTarget->GetFrontVector() + mTarget->GetPosition();
		LookPt.y += 2.f;
		SetLookAtPt(LookPt);

		D3DXVECTOR3 Pos = mTarget->GetPosition() - mTarget->GetFrontVector() * 8.f;
		Pos.y += 5.f;
		Node::SetPosition(Pos);
	}


}