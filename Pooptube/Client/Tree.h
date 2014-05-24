#pragma once
#include "stdafx.h"
#include "Node.h"

namespace pooptube {
	class XMesh;
	class SkinnedMesh;
}

class Tree : public pooptube::Node{
public:
	Tree();
	virtual ~Tree();

	static Tree *Create();

	bool Init();

	void Render();
	void Update(float dTime);


private:

	//������ �Ž�
	pooptube::XMesh			*mXMesh;
	//Ŭ��� skinnedmesh
	pooptube::SkinnedMesh	*mSkinnedMesh;

};

