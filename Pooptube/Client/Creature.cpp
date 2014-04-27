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

std::shared_ptr<Creature> Creature::Create()
{
	std::shared_ptr<Creature> pCollisionBox(new Creature);

	if (pCollisionBox->Init())
		return pCollisionBox;
	else
		return nullptr;
}

bool Creature::Init()
{
	Node::Init();

	EnableKeyEvent();
	EnableMouseEvent();

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx", pooptube::RESOURCE_FBX);
//	mCollisionBox = pooptube::CollisionBox::Create();
//	mCollisionBox->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);

	//Creature::SetPosition(initialPosition);

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

void Creature::ChangeState()
{
	//static float ssss = 0.0f;

	/*D3DXVECTOR3 CreaturePos = Creature::GetPosition();
	float PI = 3.14f;
	D3DXVECTOR3 distance = pss->GetPosition() - Creature::GetPosition();
	float distanceTemp = D3DXVec3Length(&distance);

	D3DXVECTOR3 temp = pss->GetPosition() - CreaturePos;*/
	//mDistanceFromMainCharater = 
	//if ()
}
