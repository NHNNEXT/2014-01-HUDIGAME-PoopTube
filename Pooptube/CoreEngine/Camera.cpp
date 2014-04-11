
#include "stdafx.h"
#include "Camera.h"

#include "ObjectManager.h"
#include "Application.h"

namespace pooptube {
	Camera::Camera() 
		: mEyePt(0.0f, -3.0f, 5.0f), mLookatPt(0.0f, 0.0f, 0.0f), mUpVec(0.0f, 1.0f, 0.0f)
	{
	}
	Camera::~Camera() {
	}

	Camera* Camera::Create() {
		Camera* pCamera = new Camera();
		if ( pCamera->Init() ) {
			ObjectManager::GetInstance()->AddObject( pCamera );
		} else {
			delete pCamera;
			pCamera = nullptr;
		}

		return pCamera;
	}

	bool Camera::Init() {
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		return true;
	}

	void Camera::Render()
	{
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		Node::Render();

		//프로젝션 설정
		//perspective프로젝션
		D3DXMatrixPerspectiveFovLH(&mMatProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		//생성한 프로젝션 정보를 디바이스를 통해 설정
		pDevice->SetTransform(D3DTS_PROJECTION, &mMatProj);

		//뷰행렬을 생성
		D3DXMatrixLookAtLH(&mMatView, &mEyePt, &mLookatPt, &mUpVec);
		//생성된 뷰행렬을 적용
		pDevice->SetTransform(D3DTS_VIEW, &mMatView);
	}

	void Camera::Update(float dTime)
	{
		Node::Update(dTime);
	}


}