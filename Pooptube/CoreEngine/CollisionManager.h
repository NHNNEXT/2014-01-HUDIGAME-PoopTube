#pragma once

namespace pooptube {

	class CollisionBox;

	class CollisionManager
	{
	public:
		static CollisionManager* GetInstance();
		static void ReleaseInstance( );

		void AddCollisionBox( CollisionBox* pCollisionMesh );
		void RemoveCollisionBox( CollisionBox* pCollisionMesh );

		const CollisionBox* CollisionCheck( const CollisionBox* pTarget ) const;

	private:
		CollisionManager();
		~CollisionManager();

		static CollisionManager* mInstance;

		std::forward_list<std::shared_ptr<CollisionBox>> mCollisionBoxList;
	};
}