#include "stdafx.h"
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

	Creature::SetPosition(mInitialPosition);

	AddChild(&*mSkinnedMesh);
	AddChild(&*mCollisionBox);

	return true;

}

void Creature::Render()
{
	Node::Render();	
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

	FSM();
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

	D3DXVECTOR3 pos = GetPosition();
	float height = (dynamic_cast<StageOne*>(pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()))->GetGroundModule()->GetHeight(GetPosition().x, GetPosition().z);
	//float height = (dynamic_cast<StageOne*>(pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()))->GetGroundModule()->GetHeight(GetPosition().x, GetPosition().z);

	if (height != pos.y) {
		pos.y = height;
		SetPosition(pos);
	}
}

CREATURE_STATE Creature::FSM()
{
	D3DXVECTOR3 CharacterPosition = pss->GetPosition();
	D3DXVECTOR3 CreaturePosition = GetPosition();
	D3DXVECTOR3 distance = pss->GetPosition() - GetPosition();

	if (8 < D3DXVec3Length(&distance)) {

		if (mState != IDLE)

		SetState(CREATURE_STATE::IDLE);

		//printf("idle\n");
	} else if (8 >= D3DXVec3Length(&distance) && D3DXVec3Length(&distance) >= 2) {
		SetState(ANGRY);
		//printf("angry\n");
	} else if (2 > D3DXVec3Length(&distance)) {
		SetState(RAGE);
		//printf("rage\n");
	}

	return IDLE;
}

void Creature::DoIdle(float dTime)
{
	static float temp = 0;
	temp += dTime;

	float PI = 3.14f;

	D3DXVECTOR3 CreaturePosition = GetPosition();
	D3DXVECTOR3 distance = mInitialPosition - GetPosition();
	D3DXVECTOR3 distance2 = -distance;
	D3DXVECTOR3 CreatureFrontVector = GetFrontVector();

	if (IDLE == GetState() && D3DXVec3Length(&distance) < 0.5f) {
		RotationY(0.1f);
		SetPosition(mInitialPosition);
	}
	else //if (CreaturePosition != initialPosition) 
	{
		float dot = D3DXVec3Dot(&distance2, &CreatureFrontVector);
		float GoBackRotateAngle;
		float dis = abs((D3DXVec3Length(&distance2) * D3DXVec3Length(&CreatureFrontVector)));

		if (dot != 0.f && dis != 0.f)
		{
			float rotAngle = 0.05f;
			GoBackRotateAngle = (abs(dot / dis) >= 1.f) ? 0.f : sin(acos(dot / dis));

			if (GoBackRotateAngle > rotAngle)
				RotationY(rotAngle);
			else
				RotationY(GoBackRotateAngle);

			//RotationY(GoBackRotateAngle);
		}
		
		//sin(acos()))
		/*
		printf("dot  : %f\n", dot);
		printf("dis  : %f\n", dis);
		printf("acos : %f\n", acos(dot / abs((D3DXVec3Length(&distance2) * D3DXVec3Length(&CreatureFrontVector)))));
		printf("sin  : %f\n\n", sin(acos(dot / abs((D3DXVec3Length(&distance2) * D3DXVec3Length(&CreatureFrontVector))))));
			*/
		SetPosition(CreaturePosition + (mInitialPosition - CreaturePosition) / 100);
	}
	
	//printf("%f\n", temp);
	//printf("%f ", CreaturePosition.x);
	//printf("%f\n", CreaturePosition.z);
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