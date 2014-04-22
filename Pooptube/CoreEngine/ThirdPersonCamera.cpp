
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

	void ThirdPersonCamera::Update(float dTime)
	{
		Camera::Update(dTime);

// 		D3DXVECTOR3 temp = mTarget->GetPosition();
// 		temp.y += 3.f;
// 		Node::SetPosition(temp);
// 
// 
// 
// 
// 		mEyePt = mTarget->GetPosition();
// 		mEyePt.x -= mTarget->GetFrontVector().x;
// 		mEyePt.y += 3.f;
// 		mEyePt.z -= mTarget->GetFrontVector().z;
// 
// 		mLookatPt = mTarget->GetPosition();

		//D3DXMATRIXA16 pos = mTarget->GetMatrix();
		//mEyePt = D3DXVECTOR3(pos._41 - mTarget->GetFrontVector().x, pos._42 + 3.f, pos._43 - mTarget->GetFrontVector().z);
		//mLookatPt = D3DXVECTOR3(0, 1.f, -4.f);
		//mLookatPt = D3DXVECTOR3(pos._41, pos._42 + 2.5f, pos._43);
		//mLookatPt = mEyePt - mTarget->GetFrontVector();

// 		mEyePt.z -= 1;
// 		mLookatPt.x = mEyePt.x;
// 		mLookatPt.y = mEyePt.y - 1.0f;
// 		mLookatPt.z = mEyePt.z +1;
		// mLookatPt.y -= 0.5f;
	}


}