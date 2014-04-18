#pragma once
#include "Object.h"

namespace pooptube {

	class CollisionBox;

	class CollisionManager : public Object
	{
	public:
		static CollisionManager* GetInstance();

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