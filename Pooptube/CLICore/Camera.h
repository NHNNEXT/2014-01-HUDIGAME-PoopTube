#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\Camera.h"
#include "Node.h"

using namespace System;

namespace Core {
	public ref class Camera : public Node
	{
	public:
		Camera() { };//pInstance = &*pooptube::Camera::Create(); };
		virtual ~Camera() { delete Instance<pooptube::Camera *>(pInstance); };

		CREATE(Camera);

		void		 SetLookAtPt(float x, float y, float z) { Instance<pooptube::Camera *>(pInstance)->SetLookAtPt(D3DXVECTOR3(x, y, z)); };
	};
}
