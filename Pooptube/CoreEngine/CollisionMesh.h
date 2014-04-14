#pragma once
//#include "Mesh.h"
#include "Node.h"

namespace pooptube {

	// c++11의 enum class를 사용하는건 어떨까요?
	// 혹은 enum class COLLISION_TYP : unsigned short은 어떨까요?
	enum COLLISION_TYPE // ���� ���°� ������ ���� �ѵ�...
	{
		COLLISION_NONE		=	0x00000000,
		COLLISION_BLOCK		=	0x00000001,
		COLLISION_PUSH		=	0x00000002,
		COLLISION_BOUND		=	0x00000004,
		COLLISION_DAMAGE	=	0x00000008,
		COLLISION_ALLY		=	0x00000010,
		COLLISION_ENEMY		=	0x00000020,
	};

	struct VECTOR3D // TODO: �̳༮�� ���߿� �ٲ���...
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

	class CollisionMesh :
		public Node
	{
	private:
		CollisionMesh();
		CollisionMesh( COLLISION_TYPE collisionType, float bound, float mass );
	public:
		virtual ~CollisionMesh();

		static CollisionMesh* Create();
		static CollisionMesh* Create( COLLISION_TYPE collisionType, float bound, float mass );

		virtual bool Init();

		virtual void Render();
		virtual void Update( float dTime );

		bool CollisionCheck( const CollisionMesh* target );

		//void SetCollisionType( COLLISION_TYPE val ){ mCollisionType = val; }
		//void SetBound( float val ){ mBound = val; }
		//void SetMass( float val ){ mMass = val; }
		void SetSpeed( VECTOR3D val ) { mSpeed = val; }

		COLLISION_TYPE GetCollisionType(){ return mCollisionType; }
		float GetBound(){ return mBound; }
		float GetMass(){ return mMass; }
		VECTOR3D GetSpeed() { return mSpeed; }

	private:
		COLLISION_TYPE mCollisionType;	// �浹 ����
		float mBound;		// �ݹ߰���
		float mMass;		// ����
		VECTOR3D mSpeed;	// �ӵ�
	};
}
