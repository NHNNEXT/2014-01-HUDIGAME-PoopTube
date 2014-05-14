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
		mCollisionBoxList.push_back( std::make_pair( pNode, pCollisionBox ) );
//		mCollisionBoxList.insert( std::make_pair( pNode, pCollisionBox ) );
	}

	void CollisionManager::RemoveCollisionBox( CollisionBox *pCollisionBox )
	{
		for( auto iter = mCollisionBoxList.begin(); iter != mCollisionBoxList.end(); ++iter ) {
			//if( *iter == pCollisionBox ) {
			if( iter->second == pCollisionBox ) {
				mCollisionBoxList.erase( iter );
				return;
			}
		}
	}

	void CollisionManager::RemoveCollisionBoxByNode( Node *pNode )
	{
		for( auto iter = mCollisionBoxList.begin(); iter != mCollisionBoxList.end(); ++iter ) {
			if( iter->first == pNode ) {
				mCollisionBoxList.erase( iter );
			}
		}
	}

	CollisionBox* CollisionManager::CollisionCheckNode( Node* pTarget ) const
	{
		if( pTarget == nullptr )
			return nullptr;

		CollisionBox* result = nullptr;
		for( auto target : mCollisionBoxList ){
			if( target.first == pTarget )
				result = CollisionCheck( target.second );
			if( result != nullptr )
				return result;
		}

		return nullptr;
	}
	//void CollisionManager::CollisionCheck( CollisionBox* pTarget ) const
	CollisionBox* CollisionManager::CollisionCheck( CollisionBox* pTarget ) const
	{
		if( pTarget == nullptr )
			return nullptr;

		for( auto box : mCollisionBoxList ) {
			//if( box->CollisionCheck( pTarget ) ) {
			if( box.second->CollisionCheck( pTarget ) ) {
				return box.second;
				//box->GetParent()->CollsionReceive( pTarget->GetParent() );
				//pTarget->GetParent( )->CollsionFeedBack( box->GetParent( ) );
			}
		}

		return nullptr;
	}

	void CollisionManager::ReleaseInstance() {
		if (mInstance != nullptr)
			delete mInstance;
	}

}