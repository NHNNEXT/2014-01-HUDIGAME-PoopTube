#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\ThirdPersonCamera.h"
#include "Camera.h"

using namespace System;

namespace Core {
	public ref class ThirdPersonCamera : public Camera
	{
	public:
		ThirdPersonCamera() { };//pInstance = &*pooptube::ThirdPersonCamera::Create(); };
		virtual ~ThirdPersonCamera() { delete Instance<pooptube::ThirdPersonCamera *>(pInstance); };

		CREATE(ThirdPersonCamera);
	};
}
