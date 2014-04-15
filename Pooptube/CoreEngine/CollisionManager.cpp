#include "stdafx.h"
#include "CollisionManager.h"
#include "CollisionBox.h"

namespace pooptube {
	CollisionManager* CollisionManager::mInstance = nullptr;
	CollisionManager* CollisionManager::GetInstance()
	{
		if( mInstance == nullptr ) {
			mInstance = new CollisionManager( );
		}

		return mInstance;
	}

	void CollisionManager::ReleaseInstance()
	{
		if( mInstance != nullptr ) {
			delete mInstance;
			mInstance = nullptr;
		}
	}

	void CollisionManager::AddCollisionBox( CollisionBox* pCollisionMesh )
	{
		mCollisionBoxList.push_front( std::shared_ptr<CollisionBox>( pCollisionMesh ) );
	}

	void CollisionManager::RemoveCollisionBox( CollisionBox* pCollisionMesh )
	{
		for( auto iter = mCollisionBoxList.begin( ); iter != mCollisionBoxList.end( ); iter++ ) {
			if( (*iter).get( ) == pCollisionMesh ) {
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

	CollisionManager::CollisionManager()
	{
	}

	CollisionManager::~CollisionManager()
	{
		mCollisionBoxList.clear( );
	}

}