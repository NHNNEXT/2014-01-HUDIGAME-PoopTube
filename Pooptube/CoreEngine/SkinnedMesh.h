#pragma once
#include "Node.h"

namespace pooptube {

	class SkinnedMesh : public Node {
	public:
		SkinnedMesh();
		virtual ~SkinnedMesh();

		static SkinnedMesh* Create();
		virtual bool Init();

	protected:



	private:

	};

}