
#pragma once

#include "Node.h"

namespace pooptube {
	class Scene : public Node {
	public:
		Scene();
		virtual ~Scene();

		static Scene* Create();

		virtual bool Init();

		virtual void Render();
		virtual void Update( float dTime );
	};
}