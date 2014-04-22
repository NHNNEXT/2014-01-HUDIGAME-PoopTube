#pragma once
#include "Node.h"

namespace pooptube {

	enum class COLLISION_TYPE : unsigned short {
		COLLISION_NONE = 0x0000,
		COLLISION_BLOCK = 0x0001,
		COLLISION_PUSH = 0x0002,
		COLLISION_BOUND = 0x0004,
		COLLISION_DAMAGE = 0x0008,
		COLLISION_ALLY = 0x0010,
		COLLISION_ENEMY = 0x0020,
	};

	class CollisionBox :
		public Node
	{
	private:
		CollisionBox();
		CollisionBox( COLLISION_TYPE collisionType, float bound, float mass );
	public:
		virtual ~CollisionBox();

		static std::shared_ptr<CollisionBox> Create();
		static std::shared_ptr<CollisionBox> Create( COLLISION_TYPE collisionType, float bound, float mass );

		virtual void Render();
		virtual void Update( float dTime );

		bool CollisionCheck( const CollisionBox* target );

		//void SetCollisionType( COLLISION_TYPE val ){ mCollisionType = val; }
		//void SetBound( float val ){ mBound = val; }
		//void SetMass( float val ){ mMass = val; }
		void SetSpeed( D3DXVECTOR3 val ) { mSpeed = val; }
		//void SetCenterPos( D3DXVECTOR3 val ) { Translation( val.x, val.y, val.z ); }
		//void SetAxisDir( D3DXVECTOR3 axisX, D3DXVECTOR3 axisY ) {
		//	D3DXVec3Normalize( &mAxisDir[0], &axisX );
		//	D3DXVec3Normalize( &mAxisDir[1], &axisY );
		//	D3DXVec3Cross( &mAxisDir[2], &mAxisDir[1], &mAxisDir[0] );
		//	D3DXVec3Normalize( &mAxisDir[2], &mAxisDir[2] );
		//}
		void SetAxisLen( float val0, float val1, float val2 ) {
			mAxisLen[0] = val0;
			mAxisLen[1] = val1;
			mAxisLen[2] = val2;
		}

		const COLLISION_TYPE GetCollisionType() const { return mCollisionType; }
		const float GetBound() const { return mBound; }
		const float GetMass() const { return mMass; }
		const D3DXVECTOR3 GetSpeed() const { return mSpeed; }

	private:
		COLLISION_TYPE mCollisionType;	// 충돌 종류
		float mBound;		// 반발계수
		float mMass;		// 질량
		D3DXVECTOR3 mSpeed;	// 속도

		float mAxisLen[3];			// 상자의 세 축의 길이 절반.
	};
}
