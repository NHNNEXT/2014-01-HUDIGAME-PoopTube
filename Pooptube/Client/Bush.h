#pragma once
#include "stdafx.h"
#include "Node.h"

namespace pooptube {
	class XMesh;
	class SkinnedMesh;
}

class Bush : public pooptube::Node{
public:
	Bush();
	virtual ~Bush();

	static Bush *Create();

	bool Init();

	void Render();
	void Update(float dTime);


private:

	//������ �Ž�
	pooptube::XMesh			*mXMesh;
	//Ŭ��� skinnedmesh
	pooptube::SkinnedMesh	*mSkinnedMesh;

};
