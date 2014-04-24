#pragma once
#include "Object.h"

namespace pooptube {

	class CollisionBox;

	class CollisionManager : public Object
	{
	public:
		static CollisionManager* GetInstance();

		void AddCollisionBox( std::shared_ptr<CollisionBox> pCollisionBox );
		void RemoveCollisionBox( std::shared_ptr<CollisionBox> pCollisionBox );

		const CollisionBox* CollisionCheck( const CollisionBox* pTarget ) const; // TODO :

	private:
		CollisionManager();
		~CollisionManager();

		static CollisionManager* mInstance;

		std::forward_list<std::shared_ptr<CollisionBox>> mCollisionBoxList;
	};
}