#pragma once
#include "Node.h"

namespace pooptube {

	//전방선언
	class FBXMesh;

	class SkinnedMesh : public Node {
	public:
		SkinnedMesh();
		virtual ~SkinnedMesh();

		static SkinnedMesh* Create(std::string FilePath);
		virtual bool Init(std::string FilePath);

		virtual void Render();
		virtual void Update(float dTime);

	protected:



	private:
		FBXMesh* mFBXMesh;

	};

}