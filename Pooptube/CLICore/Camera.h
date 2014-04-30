#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\Camera.h"
#include "Node.h"

using namespace System;

namespace Core {

	public ref class Camera : public Node
	{
	public:
		Camera() { pInstance = &*pooptube::Camera::Create(); };
		virtual ~Camera() { delete pInstance; };
	};
}
