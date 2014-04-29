
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
		Node::Render();

		//뷰행렬을 생성
		D3DXMatrixLookAtLH(&mMatView, &Node::GetPosition(), &GetLookAtPt(), &Node::GetUpVector());
		//생성된 뷰행렬을 적용
		GetDevice()->SetTransform(D3DTS_VIEW, &mMatView);

		//프로젝션 설정
		//perspective프로젝션
		D3DXMatrixPerspectiveFovLH(&mMatProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);
		//생성한 프로젝션 정보를 디바이스를 통해 설정
		GetDevice()->SetTransform(D3DTS_PROJECTION, &mMatProj);
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