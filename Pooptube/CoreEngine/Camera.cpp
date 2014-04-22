
#include "stdafx.h"
#include "Camera.h"

#include "ObjectManager.h"
#include "Application.h"

namespace pooptube {
	Camera::Camera() {
	}
	Camera::~Camera() {
	}

	Camera* Camera::Create() {
		Camera* pCamera = new Camera;

		if ( pCamera->Init() ) {
			ObjectManager::GetInstance()->AddObject( pCamera );

		} else {
			delete pCamera;
			pCamera = nullptr;
		}

		return pCamera;
	}

	bool Camera::Init() {
		Node::Init();

		return true;
	}

	void Camera::Render() {
		Node::Render();

		//뷰행렬을 생성
		D3DXMatrixLookAtLH(&mMatView, &Node::GetPosition(), &Node::GetFrontPoint(), &Node::GetUpVec());
		//생성된 뷰행렬을 적용
		GetDevice()->SetTransform(D3DTS_VIEW, &mMatView);

		//프로젝션 설정
		//perspective프로젝션
		D3DXMatrixPerspectiveFovLH(&mMatProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		//생성한 프로젝션 정보를 디바이스를 통해 설정
		GetDevice()->SetTransform(D3DTS_PROJECTION, &mMatProj);
	}

	void Camera::Update(float dTime) {
		Node::Update(dTime);
	}
}