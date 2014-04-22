
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

		//�Ʒ� ���Լ��� ��������� �����ȴ�. �ذ����� ã�ƺ���
		SetPosition(D3DXVECTOR3(0.f, 3.f, 10.f));
		SetFrontPoint(D3DXVECTOR3(0.f, 0.f, 0.f));

		return true;
	}

	void Camera::Render() {
		Node::Render();

		//������� ����
		D3DXMatrixLookAtLH(&mMatView, &Node::GetPosition(), &Node::GetFrontPoint(), &Node::GetUpVec());
		//������ ������� ����
		GetDevice()->SetTransform(D3DTS_VIEW, &mMatView);

		//�������� ����
		//perspective��������
		D3DXMatrixPerspectiveFovLH(&mMatProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		//������ �������� ������ ����̽��� ���� ����
		GetDevice()->SetTransform(D3DTS_PROJECTION, &mMatProj);
	}

	void Camera::Update(float dTime) {
		Node::Update(dTime);
	}
}