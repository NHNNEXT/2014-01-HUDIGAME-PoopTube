#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\SunLight.h"
#include "Light.h"

using namespace System;

namespace Core {

	public ref class SunLight : public Light
	{
		//protected:
	public:
		SunLight() { };//pInstance = &*pooptube::SunLight::Create(); };
		virtual ~SunLight() {};// delete Instance<pooptube::SunLight *>(pInstance); };

		static SunLight ^Create() {
			SunLight ^CreateInstance = gcnew SunLight();
			CreateInstance->pInstance = pooptube::SunLight::Create();
			return CreateInstance;
		}
		//CREATE(SunLight);
	};
}