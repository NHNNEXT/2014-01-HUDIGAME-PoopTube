#include "stdafx.h"
#include "Creature.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"
#include "XMesh.h"
#include "ResourceDef.h"
#include "MainCharacter.h"
#include "Ground.h"
#include "StageOne.h"


Creature::Creature()
{
}


Creature::~Creature()
{
}

Creature *Creature::Create()
{
	Creature *pCreature( new Creature );

	if( pCreature->Init() )
		return pCreature;
	else
		return nullptr;
}

bool Creature::Init()
{
	Node::Init();

	mClassName = "Creature";
	mObjectName = mObjectName = "Creature" + std::to_string(Node::ObjectNum++);

	mMesh = pooptube::SkinnedMesh::Create(PATH_WOLF);
	mMesh->RotateFrontVectorY(3.14f);
	mMesh->SetScale(2.f, 2.f, 2.f);
	mMesh->SetAnimationTrack(3);
	AddChild(mMesh);

	// 	mMesh = pooptube::SkinnedMesh::Create(PATH_TINY);
	// 	mMesh->SetAnimationTrack(3);
	// 	mMesh->SetScale(D3DXVECTOR3(0.005f, 0.005f, 0.005f));
	// 	mMesh->SetFrontVector(D3DXVECTOR3(0.f, 1.f, 0.f));
	// 	mMesh->SetUpVec(D3DXVECTOR3(0.f, 0.f, -1.f));

	//맵툴에서는 아래처럼
// 	mXMesh = pooptube::XMesh::Create(PATH_NEWDRAGON);
// 	mXMesh->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
// 	AddChild(mXMesh);

	pooptube::CollisionBox* collisionBox = pooptube::CollisionBox::Create( this );
	collisionBox->SetCollisionType( pooptube::CollisionBox::COLLISION_TYPE::BLOCK );
	collisionBox->Translation(0.f, 1.f, 0.f);
	collisionBox->SetScale(1.f, 1.f, 2.f);

	
	AddChild( collisionBox );
	Creature::SetPosition( mInitialPosition );

	mStepSound = pooptube::SoundManager::GetInstance()->GetSound( "event:/Character/Footsteps" );
	mStepSound->setParameterValue( "Surface", 1.f );
	mEffectSound = pooptube::SoundManager::GetInstance()->GetSound( "event:/Character/Okay" );

	return true;

}

void Creature::Render()
{
	mMesh->GetMeshData()->GetEffect()->SetTechnique("t0");
	Node::Render();	
}

void Creature::Update(float dTime)
{
	Node::Update(dTime);

	_CollsionHandle( pooptube::CollisionManager::GetInstance()->CollisionCheckNode( this ) );

	FSM();
	switch (mState) {
	case IDLE:
		DoIdle(dTime);
		break;
	case ANGRY:
		DoAngry();
		break;
	case RAGE:
		DoRage(dTime);
		break;
	}

	D3DXVECTOR3 pos = GetPosition();
	float height = pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGroundModule()->GetHeight(GetPosition().x, GetPosition().z);
	//float height = (dynamic_cast<StageOne*>(pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()))->GetGroundModule()->GetHeight(GetPosition().x, GetPosition().z);

	if (height != pos.y) {
		pos.y = height;
		SetPosition(pos);
	}

	pooptube::SoundManager::GetInstance()->NodeToFmod3DAttribute( *this, mSoundPos );
	mStepSound->set3DAttributes( &mSoundPos );
}

CREATURE_STATE Creature::FSM()
{
	D3DXVECTOR3 CharacterPosition = pss->GetPosition();
	D3DXVECTOR3 CreaturePosition = GetPosition();
	D3DXVECTOR3 distance = pss->GetPosition() - GetPosition();

	if (mIdleDistance * mIdleDistance < D3DXVec3LengthSq(&distance)) {

		if (mState != IDLE)

		SetState(CREATURE_STATE::IDLE);

		//printf("idle\n");
	}
	else if( mIdleDistance * mIdleDistance >= D3DXVec3LengthSq( &distance ) && D3DXVec3LengthSq( &distance ) >= mRageDistance * mRageDistance ) {
		SetState(ANGRY);
		//printf("angry\n");
	}
	else if( mRageDistance * mRageDistance > D3DXVec3LengthSq( &distance ) ) {
		SetState(RAGE);
		//printf("rage\n");
	}

	return IDLE;
}

void Creature::DoIdle(float dTime)
{
	D3DXVECTOR3 CreaturePosition = GetPosition();
	D3DXVECTOR3 distance = mInitialPosition - GetPosition();
	D3DXVECTOR3 CreatureFrontVector = GetFrontVector();
	mStepSound->stop( FMOD_STUDIO_STOP_IMMEDIATE );
	if (IDLE == GetState() && D3DXVec3LengthSq(&distance) < 0.25f) {
		//RotationY(0.1f);
		SetPosition(mInitialPosition);
	}
	else //if (CreaturePosition != initialPosition) 
	{
		D3DXVECTOR3 dir = mInitialPosition - CreaturePosition;
		D3DXVec3Normalize(&dir, &dir);

		if (Turn(GetPosition(), mInitialPosition, mSpeed) == false)
			SetPosition(CreaturePosition + dir / 10);
	}
}

void Creature::DoAngry()
{
	D3DXVECTOR3 CharacterPosition = pss->GetPosition();
	D3DXVECTOR3 CreaturePosition = GetPosition();
	if( mStepSound != nullptr )
		pooptube::SoundManager::GetInstance()->PlayOnce( *mStepSound );
	Turn(GetPosition(), CharacterPosition, mSpeed);
	SetPosition(CreaturePosition + (CharacterPosition - CreaturePosition) / 100);
}

bool Creature::DoRage(float dTime)
{
	mStepSound->stop( FMOD_STUDIO_STOP_IMMEDIATE );
	if( mEffectSound != nullptr )
		pooptube::SoundManager::GetInstance()->PlayOnce( *mEffectSound );
	RotationY(0.4f);

	mAttackTime += dTime;

	if (mAttackTime > mAttackRate) {
		mTotalDamage += mAttackDamage;
		//printf("%d \n", mTotalDamage);
		mAttackTime = 0.f;
		return true;
	}
	else
	{
		//printf("Not Yet\n");
		return false;
	}
}

void Creature::_CollsionHandle(pooptube::CollisionBox* collisionResult)
{
	if (collisionResult == nullptr)
		return;
	if (collisionResult->GetCollisionType() & pooptube::CollisionBox::COLLISION_TYPE::BLOCK) {
		D3DXVECTOR3 dPos = GetPosition() - collisionResult->GetParent()->GetPosition();
		D3DXVec3Normalize(&dPos, &dPos);
		dPos *= mSpeed;
		Translation(dPos);
	}
}
