#include "stdafx.h"
#include "SoundBox.h"
#include "CollisionBox.h"
#include "CollisionManager.h"

namespace pooptube {
	SoundBox::SoundBox()
	{
	}

	SoundBox::~SoundBox()
	{
		mSound->release();
	}

	SoundBox * SoundBox::Create( FMOD::Studio::EventInstance* soundInstance )
	{
		SoundBox *pSoundBox( new SoundBox );

		if( pSoundBox->Init( soundInstance ) ){
			return pSoundBox;
		}
		else
			return nullptr;
	}

	bool SoundBox::Init( FMOD::Studio::EventInstance* soundInstance )
	{
		Node::Init();
		mSound = soundInstance;

		mObjectName = "SoundBox" + std::to_string(Node::ObjectNum++);
		mClassName = "SoundBox";

		return true;
	}

	void SoundBox::Update( float dTime )
	{
		Node::Update( dTime );

// 		mCollisionBox->SetPosition( Node::GetPosition() );
// 		mCollisionBox->Translation( D3DXVECTOR3( 0.f, mCollisionBox->GetAxisLenY(), 0.f ) );
// 		mCollisionBox->SetFrontVector( Node::GetFrontVector() );
// 		mCollisionBox->Update( dTime );
		_CollsionHandle( pooptube::CollisionManager::GetInstance()->CollisionCheckNode( this ) );
	}

	void SoundBox::_CollsionHandle( CollisionBox* collisionResult )
	{
		if( collisionResult == nullptr || !(collisionResult->GetCollisionType() & CollisionBox::COLLISION_TYPE::PLAYER) ){
			mSound->stop( FMOD_STUDIO_STOP_ALLOWFADEOUT );
		}
		else{
			if( mSound != nullptr )
				SoundManager::GetInstance()->PlayOnce( *mSound );
		}
	}
}
