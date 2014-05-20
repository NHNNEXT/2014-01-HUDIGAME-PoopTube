#include "stdafx.h"
#include "Tree.h"
#include "ResourceDef.h"

Tree::Tree()
{
}


Tree::~Tree()
{
}

Tree * Tree::Create() {
	Tree *pTree(new Tree);

	if (pTree->Init()){
		return pTree;
	}
	else
		return nullptr;
}

bool Tree::Init() {
	Node::Init();

	mSkinnedMesh = pooptube::SkinnedMesh::Create(PATH_TREE);
	mSkinnedMesh->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	AddChild(mSkinnedMesh);


	return true;
}

void Tree::Render() {
	Node::Render();
}

void Tree::Update(float dTime) {
}

