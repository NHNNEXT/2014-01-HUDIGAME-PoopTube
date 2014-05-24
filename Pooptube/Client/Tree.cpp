#include "stdafx.h"
#include "Tree.h"
#include "XMesh.h"
#include "SkinnedMesh.h"
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

	//멥툴에서는 아래로 사용하면 될듯
// 	mXMesh = pooptube::XMesh::Create(PATH_TREE);
// 	mXMesh->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
// 	AddChild(mXMesh);

	return true;
}

void Tree::Render() {
	Node::Render();
}

void Tree::Update(float dTime) {
}

