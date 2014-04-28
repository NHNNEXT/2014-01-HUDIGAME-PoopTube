﻿#include "stdafx.h"
#include "Creature.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"

Creature::Creature()
{
}


Creature::~Creature()
{
}

std::shared_ptr<Creature> Creature::Create()
{
	std::shared_ptr<Creature> pCreature( new Creature );

	if( pCreature->Init( pCreature ) )
		return pCreature;
	else
		return nullptr;
}

bool Creature::Init( std::shared_ptr<Creature> pCreature )
{
	Node::Init();

	EnableKeyEvent();
	EnableMouseEvent();

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx");
	mCollisionBox = pooptube::CollisionBox::Create( pCreature.get() );
	mCollisionBox->SetAABBCollisionBoxFromSkinnedMesh(mSkinnedMesh);

	Creature::SetPosition(initialPosition);

	return true;

}

void Creature::Render()
{
	Node::Render();

	mSkinnedMesh->Render();
	mCollisionBox->Render();

	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx");
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

	Node* collisionResult = pooptube::CollisionManager::GetInstance()->CollisionCheck( mCollisionBox.get() );

	if( collisionResult != nullptr ) {
		D3DXVECTOR3 dPos = GetPosition() - collisionResult->GetPosition();
		D3DXVec3Normalize( &dPos, &dPos );
		dPos *= mSpeed;
		Translation( dPos );
	}

	switch (mState) {
	case IDLE:
		DoIdle(dTime);
		break;
	case ANGRY:
		DoAngry();
		break;
	case RAGE:
		DoRage();
		break;
	}
}

CREATURE_STATE Creature::FSM()
{
	D3DXVECTOR3 CharacterPosition = pss->GetPosition();
	D3DXVECTOR3 CreaturePosition = GetPosition();
	D3DXVECTOR3 distance = pss->GetPosition() - GetPosition();

	if (8 < D3DXVec3Length(&distance)) {
		SetState(IDLE);
		printf("idle\n");
	}

	if (8 >= D3DXVec3Length(&distance) && D3DXVec3Length(&distance) >= 2) {
		SetState(ANGRY);
		printf("angry\n");
	}

	if (2 > D3DXVec3Length(&distance)) {
		SetState(RAGE);
		printf("rage\n");
	}

	return IDLE;
}

void Creature::DoIdle(float dTime)
{
	float PI = 3.14f;
	static float temp = 0;
	temp += dTime;

	D3DXVECTOR3 CreaturePosition = GetPosition();
	CreaturePosition.x = initialPosition.x + 2 * sinf(temp);
	SetPosition(CreaturePosition);
	
	printf("%f\n", temp);
	printf("%f\n", CreaturePosition.x);
}

void Creature::DoAngry()
{
	D3DXVECTOR3 CharacterPosition = pss->GetPosition();
	D3DXVECTOR3 CreaturePosition = GetPosition();
	SetPosition(CreaturePosition + (CharacterPosition - CreaturePosition) / 100);
}

void Creature::DoRage()
{

}