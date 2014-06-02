#include "stdafx.h"
#include "Tree3.h"
#include "XMesh.h"
#include "SkinnedMesh.h"
#include "ResourceDef.h"
#include "CollisionBox.h"

Tree3::Tree3()
{
}


Tree3::~Tree3()
{
}

Tree3 * Tree3::Create() {
	Tree3 *pTree1(new Tree3);

	if (pTree1->Init()){
		return pTree1;
	}
	else
		return nullptr;
}

bool Tree3::Init() {
	Node::Init();

	// 	mSkinnedMesh = pooptube::SkinnedMesh::Create(PATH_Tree1_1);
	// 	//mSkinnedMesh->SetScale(0.5f, 0.5f, 0.5f);
	// 	AddChild(mSkinnedMesh);

	//멥툴에서는 아래로 사용하면 될듯
	mXMesh = pooptube::XMesh::Create(PATH_TREE_3);
	mXMesh->SetScale(0.2f, 0.5f, 0.2f);
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

void Tree3::Render() {
	Node::Render();
}

void Tree3::Update(float dTime) {
}

