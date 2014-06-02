#include "stdafx.h"
#include "Bush.h"
#include "XMesh.h"
#include "SkinnedMesh.h"
#include "ResourceDef.h"
#include "CollisionBox.h"

Bush::Bush()
{
}


Bush::~Bush()
{
}

Bush * Bush::Create() {
	Bush *pTree(new Bush);

	if (pTree->Init()){
		return pTree;
	}
	else
		return nullptr;
}

bool Bush::Init() {
	Node::Init();

	// 	mSkinnedMesh = pooptube::SkinnedMesh::Create(PATH_TREE_1);
	// 	//mSkinnedMesh->SetScale(0.5f, 0.5f, 0.5f);
	// 	AddChild(mSkinnedMesh);

	//멥툴에서는 아래로 사용하면 될듯
	mXMesh = pooptube::XMesh::Create(PATH_BUSH);
	AddChild(mXMesh);

	pooptube::CollisionBox* collisionBox = pooptube::CollisionBox::Create(this);
	collisionBox->Translation(D3DXVECTOR3(0.f, 1.2f, 0.f));
	collisionBox->SetCollisionType(pooptube::CollisionBox::BLOCK);
	collisionBox->SetAxisLenY(1.2f);
	collisionBox->SetAxisLenX(1.2f);
	collisionBox->SetAxisLenZ(1.2f);
	AddChild(collisionBox);

	return true;
}

void Bush::Render() {
	Node::Render();
}

void Bush::Update(float dTime) {
}

