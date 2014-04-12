#pragma once
//#include "CollisionMesh.h"

namespace pooptube {
	class CollisionMesh;

	/*
	enum CollisionType{
		COLLISION_NONE		=	0x00000000,
		COLLISION_BLOCK		=	0x00000001,
		COLLISION_DAMAGE	=	0x00000002,
	};
	*/
	class CollisionManager
	{
	public:
		static CollisionManager* GetInstance();
		static void ReleaseInstance( );

		void AddCollisionMesh( CollisionMesh* pCollisionMesh );
		void RemoveCollisionMesh( CollisionMesh* pCollisionMesh );

		const CollisionMesh* CollisionCheck( const CollisionMesh* pTarget );
		//CollisionType CollisionCheck( const CollisionMesh& target );

	private:
		CollisionManager();
		~CollisionManager();

		static CollisionManager* mInstance;

		std::forward_list<std::shared_ptr<CollisionMesh>> mCollisionMeshList;
	};
}