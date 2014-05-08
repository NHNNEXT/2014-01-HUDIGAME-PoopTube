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

	void CollisionManager::AddCollisionBox( CollisionBox *pCollisionBox, Node* pNode )
	{
		mCollisionBoxList.push_back( std::make_pair(pCollisionBox, pNode) );
	}

	void CollisionManager::RemoveCollisionBox( CollisionBox *pCollisionBox )
	{
		for( auto iter = mCollisionBoxList.begin(); iter != mCollisionBoxList.end(); ++iter ) {
			//if( *iter == pCollisionBox ) {
			if( iter->first == pCollisionBox ) {
				mCollisionBoxList.erase( iter );
				return;
			}
		}
	}

	//void CollisionManager::CollisionCheck( CollisionBox* pTarget ) const
	Node* CollisionManager::CollisionCheck( CollisionBox* pTarget ) const
	{
		if( pTarget == nullptr )
			return nullptr;

		for( auto box : mCollisionBoxList ) {
			//if( box->CollisionCheck( pTarget ) ) {
			if( box.first->CollisionCheck( pTarget ) ) {
				return box.second;
				//box->GetParent()->CollsionReceive( pTarget->GetParent() );
				//pTarget->GetParent( )->CollsionFeedBack( box->GetParent( ) );
			}
		}

		return nullptr;
	}
}