#pragma once
#include "Light.h"

namespace pooptube {

	class SunLight : public Light {
	public:
		SunLight();
		virtual ~SunLight();

		bool Init();

		static SunLight *Create();

		void Render();
		void Update(float dTime);

	private:

	};

}


