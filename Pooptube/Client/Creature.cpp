#include "stdafx.h"
#include "Creature.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"

Creature::Creature()
{
}


Creature::~Creature()
{
}

bool Creature::Init()
{
	Node::Init();

	EnableKeyEvent();
	EnableMouseEvent();

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx", pooptube::RESOURCE_FBX);
//	mCollisionBox = pooptube::CollisionBox::Create();
//	mCollisionBox->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);

	return true;

}

void Creature::Render()
{
	Node::Render();

	mSkinnedMesh->Render();
	mCollisionBox->Render();

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx", pooptube::RESOURCE_FBX);
//	mCollisionBox = pooptube::CollisionBox::Create();
//	mCollisionBox->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);
}

void Creature::Update(float dTime)
{
	Node::Update(dTime);

	mSkinnedMesh->SetPosition(Node::GetPosition());
	mSkinnedMesh->SetFrontPoint(Node::GetFrontPoint());
	mSkinnedMesh->Update(dTime);

	mCollisionBox->SetPosition(Node::GetPosition());
	mCollisionBox->Translation(D3DXVECTOR3(0.f, mCollisionBox->GetAxisLenY(), 0.f));
	mCollisionBox->SetFrontPoint(Node::GetFrontPoint());
	mCollisionBox->Update(dTime);
}
