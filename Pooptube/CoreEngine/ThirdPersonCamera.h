
#pragma once
#include "Camera.h"

namespace pooptube {
	class ThirdPersonCamera : public Camera {
	public:
		ThirdPersonCamera();
		virtual ~ThirdPersonCamera();

		static ThirdPersonCamera* Create(Node* Target);

		virtual bool Init(Node* Target);
		virtual void Render();
		virtual void Update(float dTime);

	protected:

	private:
		Node *mTarget = nullptr;
	};
}