#pragma once
//#include "Mesh.h"
#include "Node.h"
#include "CollisionType.h"

namespace pooptube {

	class CollisionBox :
		public Node
	{
	private:
		CollisionBox();
		CollisionBox( COLLISION_TYPE collisionType, float bound, float mass );
	public:
		virtual ~CollisionBox();

		static CollisionBox* Create();
		static CollisionBox* Create( COLLISION_TYPE collisionType, float bound, float mass );

		virtual void Render();
		virtual void Update( float dTime );

		bool CollisionCheck( const CollisionBox* target );

		//void SetCollisionType( COLLISION_TYPE val ){ mCollisionType = val; }
		//void SetBound( float val ){ mBound = val; }
		//void SetMass( float val ){ mMass = val; }
		void SetSpeed( D3DXVECTOR3 val ) { mSpeed = val; }
		void SetCenterPos( D3DXVECTOR3 val ) { Translation( val.x, val.y, val.z ); }
		void SetAxisDir( D3DXVECTOR3 axisX, D3DXVECTOR3 axisY ) {
			D3DXVec3Normalize( &mAxisDir[0], &axisX );
			D3DXVec3Normalize( &mAxisDir[1], &axisY );
			D3DXVec3Cross( &mAxisDir[2], &mAxisDir[1], &mAxisDir[0] );
			D3DXVec3Normalize( &mAxisDir[2], &mAxisDir[2] );
		}
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
		COLLISION_TYPE mCollisionType;	// �浹 ����
		float mBound;		// �ݹ߰��
		float mMass;		// ����
		D3DXVECTOR3 mSpeed;	// �ӵ�

		D3DXVECTOR3 mCenterPos;		// ���� �߾��� ��ǥ
		D3DXVECTOR3 mAxisDir[3];	//���ڿ� ������ �� ���� ��������
		float mAxisLen[3];			// ������ ������ �� ���� ���� mAxisLen[n]�� mAxisDir[n]�� ���� �����Ѵ�.
	};
}
