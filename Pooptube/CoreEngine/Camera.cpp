
#include "stdafx.h"
#include "Camera.h"

#include "ObjectManager.h"
#include "Application.h"

namespace pooptube {
	Camera::Camera() 
		: mEyePt(0.0f, 3.0f, 5.0f), mLookatPt(0.0f, 0.0f, 0.0f), mUpVec(0.0f, 1.0f, 0.0f)
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

		//뷰행렬을 생성
		D3DXMatrixLookAtLH(&mMatView, &mEyePt, &mLookatPt, &mUpVec);
		//생성된 뷰행렬을 적용
		pDevice->SetTransform(D3DTS_VIEW, &mMatView);

		//프로젝션 설정
		//perspective프로젝션
		D3DXMatrixPerspectiveFovLH(&mMatProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		//생성한 프로젝션 정보를 디바이스를 통해 설정
		pDevice->SetTransform(D3DTS_PROJECTION, &mMatProj);
	}

	void Camera::Update(float dTime)
	{
		Node::Update(dTime);
	}

	void Camera::Rotate(D3DXVECTOR3 dAngle)
	{
		float m_fYaw = D3DXToRadian(dAngle.x * 0.1f);
		float m_fPitch = D3DXToRadian(dAngle.y * 0.1f);

		D3DXMATRIX rot;
		D3DXVECTOR3 vcZ = mLookatPt - mEyePt;
		D3DXVECTOR3 vcX;
		D3DXMatrixRotationY(&rot, m_fYaw);
		D3DXVec3TransformCoord(&vcZ, &vcZ, &rot);
		D3DXVec3TransformCoord(&mUpVec, &mUpVec, &rot);

		mLookatPt = vcZ + mEyePt;
		D3DXMatrixLookAtLH(&mMatView, &mEyePt, &mLookatPt, &mUpVec);


		vcZ = mLookatPt - mEyePt;
		vcX = D3DXVECTOR3(mMatView._11, mMatView._21, mMatView._31);

		D3DXMatrixRotationAxis(&rot, &vcX, m_fPitch);
		D3DXVec3TransformCoord(&vcZ, &vcZ, &rot);
		D3DXVec3TransformCoord(&mUpVec, &mUpVec, &rot);

		mLookatPt = vcZ + mEyePt;
	}

	void Camera::MoveSide(FLOAT fSpeed)
	{
		D3DXVECTOR3 tmp(mMatView._11, 0, mMatView._31);
		D3DXVec3Normalize(&tmp, &tmp);

		mEyePt += tmp * fSpeed;
		mLookatPt += tmp * fSpeed;
	}

	void Camera::MoveForward(FLOAT fSpeed, FLOAT fY /*= 0*/)
	{
		D3DXVECTOR3 tmp(mMatView._13, mMatView._23*fY, mMatView._33);
		D3DXVec3Normalize(&tmp, &tmp);

		mEyePt += tmp * fSpeed;
		mLookatPt += tmp * fSpeed;
	}


}