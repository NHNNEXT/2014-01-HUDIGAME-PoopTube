#include "stdafx.h"
#include "Tree.h"
#include "XMesh.h"
#include "SkinnedMesh.h"
#include "ResourceDef.h"
#include "CollisionBox.h"

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
	//mSkinnedMesh->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	AddChild(mSkinnedMesh);

	pooptube::CollisionBox* collisionBox = pooptube::CollisionBox::Create(this);
	collisionBox->Translation(D3DXVECTOR3(0.f, 1.2f, 0.f));
	collisionBox->SetCollisionType(pooptube::CollisionBox::BLOCK);
	collisionBox->SetAxisLenY(1.2f);
	collisionBox->SetAxisLenX(1.2f);
	collisionBox->SetAxisLenZ(1.2f);
	AddChild(collisionBox);

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

