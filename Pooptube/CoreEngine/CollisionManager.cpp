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
		mCollisionBoxList.push_front( pCollisionBox );
	}

	void CollisionManager::RemoveCollisionBox( std::shared_ptr<CollisionBox> pCollisionBox )
	{
		for( auto iter = mCollisionBoxList.begin( ); iter != mCollisionBoxList.end( ); iter++ ) {
			if( (*iter) == pCollisionBox ) {
				mCollisionBoxList.erase_after( iter );
				break;
			}
		}
	}

	const CollisionBox* CollisionManager::CollisionCheck( const CollisionBox* pTarget ) const
	{
		if( pTarget == nullptr )
			return nullptr;

		CollisionBox * temp = nullptr;
		for( auto iter = mCollisionBoxList.begin( ); iter != mCollisionBoxList.end( ); iter++ ) {
			temp = (*iter).get();
			if( temp != pTarget && temp->CollisionCheck( pTarget ) ) {
				return temp;
			}
		}

		return nullptr;
	}
}