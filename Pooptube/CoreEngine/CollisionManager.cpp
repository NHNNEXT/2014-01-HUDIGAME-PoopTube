#include "stdafx.h"
#include "CollisionManager.h"
#include "CollisionMesh.h"

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

	void CollisionManager::AddCollisionMesh( CollisionMesh* pCollisionMesh )
	{
		mCollisionMeshList.push_front( std::shared_ptr<CollisionMesh>( pCollisionMesh ) );
	}

	void CollisionManager::RemoveCollisionMesh( CollisionMesh* pCollisionMesh )
	{
		for( auto iter = mCollisionMeshList.begin( ); iter != mCollisionMeshList.end( ); iter++ ) {
			if( (*iter).get( ) == pCollisionMesh ) {
				mCollisionMeshList.erase_after( iter );
				break;
			}
		}
	}

	const CollisionMesh* CollisionManager::CollisionCheck( const CollisionMesh* pTarget )
	{
		if( pTarget == nullptr )
			return nullptr;

		for( auto iter = mCollisionMeshList.begin(); iter != mCollisionMeshList.end(); iter++ ) {
			if( (*iter).get( )->CollisionCheck( pTarget ) ) {
				return (*iter).get();
			}
		}

		return nullptr;
	}

	CollisionManager::CollisionManager()
	{
	}

	CollisionManager::~CollisionManager()
	{
		mCollisionMeshList.clear();
	}

}