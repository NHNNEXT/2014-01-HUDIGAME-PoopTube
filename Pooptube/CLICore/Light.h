#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\Light.h"
#include "Node.h"

using namespace System;

namespace Core {
	public ref class Light : public Node
	{
	public:
		Light() { pInstance = &*pooptube::Light::Create(); };
		virtual ~Light() { delete pInstance; };

		void SetLightState(bool isOn)	{ dynamic_cast<pooptube::Light *>(pInstance)->LightOnOff(isOn); };
		void SetRange(float range)		{ Instance<pooptube::Light *>(pInstance)->SetRange(range); };
		int GetIndex()					{ return Instance<pooptube::Light *>(pInstance)->GetIndex(); };
	};
}
