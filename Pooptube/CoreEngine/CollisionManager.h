/**
* @author 조익성
* @brief 충돌상자들의 풀을 가지고 충돌검사를 수행하는 객체
* 싱글톤
* @date 2014/04/17
* @file CollisionManager.h
*/
#pragma once
#include "Object.h"

namespace pooptube {

	class CollisionBox;
	class Node;

	class CollisionManager : public Object
	{
	public:
		static CollisionManager* GetInstance();
		static void ReleaseInstance();

		void AddCollisionBox( CollisionBox *pCollisionBox, Node* pNode );
		void RemoveCollisionBox( CollisionBox *pCollisionBox );
		void RemoveCollisionBoxByNode( Node *pNode );

		//void CollisionCheck( CollisionBox* pTarget ) const;
		CollisionBox* CollisionCheck( CollisionBox* pTarget ) const;
		CollisionBox* CollisionCheckNode( Node* pTarget ) const;

	private:
		CollisionManager();
		~CollisionManager();

		static CollisionManager* mInstance;

		//@tobo : 매번 업데이트 할때마다 순회 해야하는 풀이니, 더 빠를 필요가 있음
		//어레이로 자료구조를 전환해야함
//		std::vector<std::shared_ptr<CollisionBox>> mCollisionBoxList;
		std::vector< std::pair<Node*, CollisionBox*> > mCollisionBoxList;
//		std::unordered_map< Node*, CollisionBox* > mCollisionBoxList;
	};
}