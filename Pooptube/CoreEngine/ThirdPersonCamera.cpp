
#include "stdafx.h"
#include "ThirdPersonCamera.h"

#include "ObjectManager.h"
#include "Application.h"

namespace pooptube {
	ThirdPersonCamera::ThirdPersonCamera() {
	}
	ThirdPersonCamera::~ThirdPersonCamera() {
	}

	ThirdPersonCamera* ThirdPersonCamera::Create(Node* Target) {
		ThirdPersonCamera* pThirdPersonCamera = new ThirdPersonCamera;

		if (pThirdPersonCamera->Init(Target)) {
			ObjectManager::GetInstance()->AddObject(pThirdPersonCamera);
		}
		else {
			delete pThirdPersonCamera;
			pThirdPersonCamera = nullptr;
		}

		return pThirdPersonCamera;
	}

	bool ThirdPersonCamera::Init(Node* Target) {
		Camera::Init();
		mTarget = Target;

		return true;
	}

	void ThirdPersonCamera::Render() {
		Camera::Render();
	}

	void ThirdPersonCamera::Update(float dTime) {
		Camera::Update(dTime);
		
		//���� ���汸����. �������� ��ȯ���� �����ؼ� �����ؾ���
		D3DXVECTOR3 FrontPoint = mTarget->GetPosition();
		FrontPoint.y += 2.f;
		Node::SetFrontPoint(FrontPoint);

		D3DXVECTOR3 Pos = 2.f * mTarget->GetPosition() - mTarget->GetFrontPoint();
		Pos.y += 3.f;
		Node::SetPosition(Pos);
	}


}