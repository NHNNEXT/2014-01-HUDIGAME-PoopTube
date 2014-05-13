#include "stdafx.h"
#include "Creature.h"
#include "SkinnedMesh.h"
#include "CollisionBox.h"
#include "CollisionManager.h"
#include "XMesh.h"

Creature::Creature()
{
}


Creature::~Creature()
{
}

Creature *Creature::Create()
{
	Creature *pCreature( new Creature );

	if( pCreature->Init( pCreature ) )
		return pCreature;
	else
		return nullptr;
}

bool Creature::Init( Creature *pCreature )
{
	Node::Init();

	EnableKeyEvent();
	EnableMouseEvent();

	mXMesh = pooptube::XMesh::Create("tiger.x");
	mSkinnedMesh = pooptube::SkinnedMesh::Create("batman70.fbx");
	pooptube::CollisionBox* collisionBox = pooptube::CollisionBox::Create( pCreature );
	collisionBox->SetAABBCollisionBoxFromSkinnedMesh( mSkinnedMesh );
	collisionBox->SetCollisionType( pooptube::CollisionBox::COLLISION_TYPE::BLOCK );

	AddChild(mSkinnedMesh);
	AddChild( collisionBox );
	//AddChild(mXMesh.get());
	Creature::SetPosition( mInitialPosition );

	mStepSound = pooptube::SoundManager::GetInstance()->GetSound( "event:/Character/Footsteps" );
	mStepSound->setParameterValue( "Surface", 1.f );
	mEffectSound = pooptube::SoundManager::GetInstance()->GetSound( "event:/Character/Okay" );

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
	mSkinnedMesh->SetFrontVector(Node::GetFrontVector());
	mSkinnedMesh->Update(dTime);

	pooptube::CollisionBox* collisionResult = pooptube::CollisionManager::GetInstance()->CollisionCheckNode( this );

	if( collisionResult != nullptr && collisionResult->GetCollisionType() == pooptube::CollisionBox::COLLISION_TYPE::BLOCK ) {
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

	pooptube::SoundManager::GetInstance()->NodeToFmod3DAttribute( *this, mSoundPos );
	mStepSound->set3DAttributes( &mSoundPos );
}

CREATURE_STATE Creature::FSM()
{
	D3DXVECTOR3 CharacterPosition = pss->GetPosition();
	D3DXVECTOR3 CreaturePosition = GetPosition();
	D3DXVECTOR3 distance = pss->GetPosition() - GetPosition();

	if (mIdleDistance < D3DXVec3Length(&distance)) {

		if (mState != IDLE)

		SetState(CREATURE_STATE::IDLE);

		//printf("idle\n");
	} else if (mIdleDistance >= D3DXVec3Length(&distance) && D3DXVec3Length(&distance) >= mRageDistance) {
		SetState(ANGRY);
		//printf("angry\n");
	} else if (mRageDistance > D3DXVec3Length(&distance)) {
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
	if (IDLE == GetState() && D3DXVec3Length(&distance) < 0.5f) {
		RotationY(0.1f);
		SetPosition(mInitialPosition);
	}
	else //if (CreaturePosition != initialPosition) 
	{
		D3DXVECTOR3 dir = mInitialPosition - CreaturePosition;
		D3DXVec3Normalize(&dir, &dir);

		if (Turn(GetPosition(), mInitialPosition, 0.05f) == false)
			SetPosition(CreaturePosition + dir / 10);
			
		
	}
}

void Creature::DoAngry()
{
	D3DXVECTOR3 CharacterPosition = pss->GetPosition();
	D3DXVECTOR3 CreaturePosition = GetPosition();
	if( mStepSound != nullptr )
		pooptube::SoundManager::GetInstance()->PlayOnce( *mStepSound );
	Turn(GetPosition(), CharacterPosition, 0.05f);
	SetPosition(CreaturePosition + (CharacterPosition - CreaturePosition) / 100);
}

void Creature::DoRage()
{
	mStepSound->stop( FMOD_STUDIO_STOP_IMMEDIATE );
	if( mEffectSound != nullptr )
		pooptube::SoundManager::GetInstance()->PlayOnce( *mEffectSound );
	RotationY(0.4f);
}