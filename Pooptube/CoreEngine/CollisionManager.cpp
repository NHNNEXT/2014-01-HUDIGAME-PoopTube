#include "stdafx.h"
#include "CollisionManager.h"
#include "CollisionBox.h"

namespace pooptube {
	CollisionManager* CollisionManager::mInstance = nullptr;

	CollisionManager* CollisionManager::GetInstance() {
		if (mInstance == nullptr) {
			mInstance = new CollisionManager();
			ObjectManager::GetInstance()->AddObject(mInstance);
		}

		return mInstance;
	}

	CollisionManager::CollisionManager() {
	}

	CollisionManager::~CollisionManager() {
	}

	void CollisionManager::AddCollisionBox( std::shared_ptr<CollisionBox> pCollisionBox )
	{
		mCollisionBoxList.push_back( pCollisionBox );
	}

	void CollisionManager::RemoveCollisionBox( std::shared_ptr<CollisionBox> pCollisionBox )
	{
		for( auto iter = mCollisionBoxList.begin(); iter != mCollisionBoxList.end(); ++iter ) {
			if( *iter == pCollisionBox ) {
				mCollisionBoxList.erase( iter );
				return;
			}
		}
	}

	void CollisionManager::CollisionCheck( CollisionBox* pTarget ) const
	{
		if( pTarget == nullptr )
			return;

		for( auto box : mCollisionBoxList ) {
			if( box->CollisionCheck( pTarget ) ) {
				box->GetParent()->CollsionReceive( pTarget->GetParent() );
				pTarget->GetParent( )->CollsionFeedBack( box->GetParent( ) );
			}
		}

		return;
	}
}