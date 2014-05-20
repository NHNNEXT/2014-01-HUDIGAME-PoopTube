
#include "stdafx.h"
#include "Camera.h"

#include "ObjectManager.h"
#include "Application.h"

namespace pooptube {
	Camera::Camera() {
	}
	Camera::~Camera() {
	}

	Camera *Camera::Create() {
		Camera *pCamera(new Camera);
		if (pCamera->Init())
			return pCamera;
		else 
			return nullptr;
	}

	bool Camera::Init() {
		Node::Init();

		//아래 두함수의 적용순서가 고정된다. 해결방법을 찾아보자
		SetPosition(D3DXVECTOR3(0.f, 3.f, 10.f));
		SetFrontVector(D3DXVECTOR3(0.f, 0.f, 0.f));

		mObjectName = "Camera" + std::to_string(ObjectNum-1);
		mClassName = "Camera";

		return true;
	}

	void Camera::Render() {
		Node::Render();

		D3DXMATRIXA16	MatView;
		D3DXMATRIXA16	MatProj;

		//뷰행렬을 생성
		D3DXMatrixLookAtLH(&MatView, &Node::GetPosition(), &mLookAtPt, &Node::GetUpVector());
		//생성된 뷰행렬을 적용
		GetDevice()->SetTransform(D3DTS_VIEW, &MatView);

		//프로젝션 설정
		//perspective프로젝션
		D3DXMatrixPerspectiveFovLH(&MatProj, D3DX_PI / 4, 1.0f, 1.0f, 10000.0f);
		//생성한 프로젝션 정보를 디바이스를 통해 설정
		GetDevice()->SetTransform(D3DTS_PROJECTION, &MatProj);
	}

	void Camera::Update(float dTime) {
		Node::Update(dTime);

		mLookAtPt = GetPosition() + GetFrontVector();
	}
}