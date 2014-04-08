
#pragma once

#include "Node.h"

namespace pooptube {
	class Camera : public Node {
	public:
		Camera();
		virtual ~Camera();

		static Camera* Create();

		virtual bool Init();
	};
}