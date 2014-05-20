#pragma once
#include "stdafx.h"
#include "Node.h"
#include "SkinnedMesh.h"

class Tree : public pooptube::Node{
public:
	Tree();
	virtual ~Tree();

	static Tree *Create();

	bool Init();

	void Render();
	void Update(float dTime);


private:

	pooptube::SkinnedMesh	*mSkinnedMesh;

};

