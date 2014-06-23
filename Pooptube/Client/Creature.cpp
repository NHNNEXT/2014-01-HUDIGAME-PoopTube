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
	mWalkSound->release();
	mAngrySound->release();
	mAttackSound->release();
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
	mMesh->SetVisible(false);
	AddChild(mMesh);

	mMesh2 = pooptube::SkinnedMesh::Create(PATH_WOLF2);
	mMesh2->RotateFrontVectorY(3.14f);
	mMesh2->SetScale(2.f, 2.f, 2.f);
	mMesh2->SetAnimationTrack(1);
	AddChild(mMesh2);

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
	collisionBox->SetAxisLenY(1.0f);
	collisionBox->SetAxisLenX(1.0f);
	collisionBox->SetAxisLenZ(2.0f);
	collisionBox->SetScale(1.f, 1.f, 2.f);
	
	AddChild( collisionBox );
	//Creature::SetPosition( mInitialPosition );

	mWalkSound = pooptube::SoundManager::GetInstance()->GetSound( "event:/Walk" );
	mAttackSound = pooptube::SoundManager::GetInstance()->GetSound( "event:/Bite" );
	mAngrySound = pooptube::SoundManager::GetInstance()->GetSound( "event:/Growling" );

	return true;
}

void Creature::Render()
{
	switch (mState) {
	case IDLE:
		mMesh2->SetVisible(true);
		mMesh->SetVisible(false);
		break;
	case ANGRY:
		mMesh2->SetVisible(false);
		mMesh->SetVisible(true);
		break;
	case RAGE:
		break;
	case GOBACK:
		break;
	default:
		break;
	}

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
	case GOBACK:
		DoGoBack();
	}

	D3DXVECTOR3 pos = GetPosition();
	float height = pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGroundModule()->GetHeight(GetPosition().x, GetPosition().z);
	//float height = (dynamic_cast<StageOne*>(pooptube::Application::GetInstance()->GetSceneManager()->GetCurrentScene()))->GetGroundModule()->GetHeight(GetPosition().x, GetPosition().z);

	if (height != pos.y) {
		pos.y = height;
		SetPosition(pos);
	}

	pooptube::SoundManager::GetInstance()->NodeToFmod3DAttribute( *this, mSoundPos );
	mWalkSound->set3DAttributes( &mSoundPos );
	mAngrySound->set3DAttributes( &mSoundPos );
	mAttackSound->set3DAttributes( &mSoundPos );
}

CREATURE_STATE Creature::FSM()
{
	D3DXVECTOR3 CharacterPosition = pss->GetPosition();
	D3DXVECTOR3 CreaturePosition = GetPosition();
	D3DXVECTOR3 distance = pss->GetPosition() - GetPosition();

	if (mIdleDistance * mIdleDistance < D3DXVec3LengthSq(&distance)) {

		if (mState != IDLE) {
			SetState(CREATURE_STATE::GOBACK);
			printf("GoBACK!\n");

			D3DXVECTOR3 distanceFromInitToCreature = mInitialPosition - CreaturePosition;

			if (D3DXVec3LengthSq(&distanceFromInitToCreature) < 0.25f)
			{
				SetPosition(mInitialPosition);
				SetState(CREATURE_STATE::IDLE);
			}
		}
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
	mAttackSound->stop( FMOD_STUDIO_STOP_ALLOWFADEOUT );
	mAngrySound->stop( FMOD_STUDIO_STOP_ALLOWFADEOUT );
	if( mWalkSound != nullptr )
		pooptube::SoundManager::GetInstance()->PlayOnce( *mWalkSound );
	if (IDLE == GetState() && D3DXVec3LengthSq(&distance) < 4.f) {
		//RotationY(0.1f);
		SetPosition(mInitialPosition);
	}
	else //if (CreaturePosition != initialPosition) 
	{
		D3DXVECTOR3 dir = mInitialPosition - CreaturePosition;
		D3DXVec3Normalize(&dir, &dir);

		if (Turn(GetPosition(), mInitialPosition, mTurnSpeed) == false)
			SetPosition(CreaturePosition + dir / 10);
	}
}

void Creature::DoAngry()
{
	D3DXVECTOR3 CharacterPosition = pss->GetPosition();
	D3DXVECTOR3 CreaturePosition = GetPosition();
	mAttackSound->stop( FMOD_STUDIO_STOP_ALLOWFADEOUT );
	if( mWalkSound != nullptr )
		pooptube::SoundManager::GetInstance()->PlayOnce( *mWalkSound );
	if( mAngrySound != nullptr )
		pooptube::SoundManager::GetInstance()->PlayOnce( *mAngrySound );
	Turn(GetPosition(), CharacterPosition, mTurnSpeed);
	SetPosition( CreaturePosition + (CharacterPosition - CreaturePosition) * mRunSpeed);
}

bool Creature::DoRage(float dTime)
{
	mWalkSound->stop( FMOD_STUDIO_STOP_ALLOWFADEOUT );
	if( mAngrySound != nullptr )
		pooptube::SoundManager::GetInstance()->PlayOnce( *mAngrySound );
	if( mAttackSound != nullptr )
		pooptube::SoundManager::GetInstance()->PlayOnce( *mAttackSound );
	//RotationY(0.4f);

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

void Creature::DoGoBack()
{
	D3DXVECTOR3 CreaturePosition = GetPosition();
	if (mStepSound != nullptr)
		pooptube::SoundManager::GetInstance()->PlayOnce(*mStepSound);
	Turn(GetPosition(), mInitialPosition, mTurnSpeed);
	SetPosition(CreaturePosition + (mInitialPosition - CreaturePosition) * mRunSpeed);
}

void Creature::_CollsionHandle(pooptube::CollisionBox* collisionResult)
{
	if (collisionResult == nullptr)
		return;
	if (collisionResult->GetCollisionType() & pooptube::CollisionBox::COLLISION_TYPE::BLOCK) {
		D3DXVECTOR3 dPos = GetPosition() - collisionResult->GetParent()->GetPosition();
		D3DXVec3Normalize(&dPos, &dPos);
		dPos *= mRunSpeed;
		Translation(dPos);
	}
}
