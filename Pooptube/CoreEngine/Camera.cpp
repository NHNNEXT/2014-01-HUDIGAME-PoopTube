
#include "stdafx.h"
#include "Camera.h"

#include "ObjectManager.h"

namespace pooptube {
	Camera::Camera() {
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
		return true;
	}
}