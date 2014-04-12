#include "stdafx.h"
#include "CollisionMesh.h"
#include "CollisionManager.h"

namespace pooptube {
	CollisionMesh::CollisionMesh()
		: mCollisionType( COLLISION_NONE )
		, mBound( 0.0f )
		, mMass( 1.0f )
	{
	}

	CollisionMesh::CollisionMesh( COLLISION_TYPE collisionType, float bound, float mass )
		: mCollisionType( collisionType )
		, mBound( bound )
		, mMass( mass )
	{
	}

	CollisionMesh::~CollisionMesh()
	{
	}

	CollisionMesh* CollisionMesh::Create()
	{
		return Create( COLLISION_NONE, 0.0f, 1.0f );
	}
	CollisionMesh* CollisionMesh::Create( COLLISION_TYPE collisionType, float bound, float mass )
	{
		CollisionMesh* pCollisionMesh = new CollisionMesh( collisionType, bound, mass );
		if( pCollisionMesh->Init( ) ) {
			ObjectManager::GetInstance( )->AddObject( pCollisionMesh );
			CollisionManager::GetInstance()->AddCollisionMesh( pCollisionMesh );
		}
		else {
			delete pCollisionMesh;
			pCollisionMesh = nullptr;
		}
		return pCollisionMesh;
	}

	bool CollisionMesh::Init()
	{
		return true;
	}

	void CollisionMesh::Render()
	{
	}

	void CollisionMesh::Update( float dTime )
	{
		Node::Update( dTime );
	}

	bool CollisionMesh::CollisionCheck( const CollisionMesh* target )
	{
		// TO DO: OBB Collision Check
		return false;
	}

}