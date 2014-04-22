
#include "stdafx.h"
#include "ThirdPersonCamera.h"

#include "ObjectManager.h"
#include "Application.h"

namespace pooptube {
	ThirdPersonCamera::ThirdPersonCamera()
		//: mEyePt(0.0f, 3.0f, 5.0f), mLookatPt(0.0f, 0.0f, 0.0f), mUpVec(0.0f, 1.0f, 0.0f)
	{
	}
	ThirdPersonCamera::~ThirdPersonCamera() {
	}

	ThirdPersonCamera* ThirdPersonCamera::Create() {
		ThirdPersonCamera* pThirdPersonCamera = new ThirdPersonCamera;

		if (pThirdPersonCamera->Init()) {
			ObjectManager::GetInstance()->AddObject(pThirdPersonCamera);
		}
		else {
			delete pThirdPersonCamera;
			pThirdPersonCamera = nullptr;
		}

		return pThirdPersonCamera;
	}

	bool ThirdPersonCamera::Init() {
		Node::Init();

		return true;
	}

	void ThirdPersonCamera::Render()
	{
		Node::Render();

//		D3DXMATRIXA16 temp = mTarget->GetMatrix();

		//뷰행렬을 생성
		D3DXMatrixLookAtLH(&mMatView, &mEyePt, &mLookatPt, &mUpVec);
		//생성된 뷰행렬을 적용
		Node::GetDevice()->SetTransform(D3DTS_VIEW, &mMatView);

		//프로젝션 설정
		//perspective프로젝션
		D3DXMatrixPerspectiveFovLH(&mMatProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		//생성한 프로젝션 정보를 디바이스를 통해 설정
		Node::GetDevice()->SetTransform(D3DTS_PROJECTION, &mMatProj);
	}

	void ThirdPersonCamera::Update(float dTime)
	{
		Node::Update(dTime);

		mEyePt = mTarget->GetPosition();
		mEyePt.x -= mTarget->GetFrontVector().x;
		mEyePt.y += 3.f;
		mEyePt.z -= mTarget->GetFrontVector().z;

		mLookatPt = mTarget->GetPosition();

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