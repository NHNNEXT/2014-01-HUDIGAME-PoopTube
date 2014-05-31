
#pragma once

#include "Node.h"

namespace pooptube {
	class Ground;

	class Scene : public Node {
	public:
		Scene();
		virtual ~Scene();

		static Scene* Create();

		virtual bool Init();

		virtual void Render();
		virtual void Update( float dTime );

		Ground *GetGroundModule() const { return mGround; }

	protected:
		Ground				*mGround = nullptr;

	private :
		std::vector<Node *> mRenderList;
	};
}