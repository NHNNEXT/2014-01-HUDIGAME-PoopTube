
#include "stdafx.h"
#include "ThirdPersonCamera.h"

#include "ObjectManager.h"
#include "Application.h"

namespace pooptube {
	ThirdPersonCamera::ThirdPersonCamera() {
	}
	ThirdPersonCamera::~ThirdPersonCamera() {
	}

	std::shared_ptr<ThirdPersonCamera> ThirdPersonCamera::Create(std::shared_ptr<Node> Target) {
		std::shared_ptr<ThirdPersonCamera> pThirdPersonCamera(new ThirdPersonCamera);

		if (pThirdPersonCamera->Init(Target)) {
			return pThirdPersonCamera;
		}
		else 
			return nullptr;
	}

	bool ThirdPersonCamera::Init(std::shared_ptr<Node> Target) {
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
		FrontPoint.y += 3.f;
		Node::SetFrontPoint(FrontPoint);

		D3DXVECTOR3 Pos = 2.f * mTarget->GetPosition() - mTarget->GetFrontPoint();
		Pos.y += 3.f;
		Node::SetPosition(Pos);
	}


}