
#pragma once
#include "Camera.h"

namespace pooptube {
	class ThirdPersonCamera : public Camera {
	public:
		ThirdPersonCamera();
		virtual ~ThirdPersonCamera();

		static ThirdPersonCamera* Create();

		void SetTarget(Node *T) { mTarget = T; }
		virtual bool Init();
		virtual void Render();
		virtual void Update(float dTime);



	protected:

	private:
		Node *mTarget = nullptr;
	};
}