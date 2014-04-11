
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

		//�������� ����
		//perspective��������
		D3DXMatrixPerspectiveFovLH(&mMatProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		//������ �������� ������ ����̽��� ���� ����
		pDevice->SetTransform(D3DTS_PROJECTION, &mMatProj);

		//������� ����
		D3DXMatrixLookAtLH(&mMatView, &mEyePt, &mLookatPt, &mUpVec);
		//������ ������� ����
		pDevice->SetTransform(D3DTS_VIEW, &mMatView);
	}

	void Camera::Update(float dTime)
	{
		Node::Update(dTime);
	}


}