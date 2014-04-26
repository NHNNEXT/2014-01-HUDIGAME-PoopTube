/**
* @author 조익성
* @brief 충돌을 판별하는 객체 
* OBB충돌 알고리즘 이용
* @date 2014/04/17
* @file CollsionBox.h
*/
#pragma once
#include "Node.h"
#include "Mesh.h"

namespace pooptube {

	class SkinnedMesh;

	class CollisionBox : public Node {
	public:
		enum AXIS {
			AXIS_X = 0,
			AXIS_Y = 1,
			AXIS_Z = 2,
			AXIS_NUM = 3,
		};

		CollisionBox();
		CollisionBox(const D3DXVECTOR3& fMax, const D3DXVECTOR3& fMin);
		virtual ~CollisionBox();

		static std::shared_ptr<CollisionBox> Create();
		static std::shared_ptr<CollisionBox> Create(MESH_CUSTOM_VERTEX* vertices);

		bool Init();

		virtual void Render();
		virtual void Update( float dTime );

		void SetAABBCollisionBoxFromSkinnedMesh(std::shared_ptr<SkinnedMesh> pMesh);
		void SetAABBCollisionBoxFromVertices(MESH_CUSTOM_VERTEX* vertices, UINT Size);

		bool CollisionCheck( const CollisionBox* target );

		float GetAxisLenX() { return mAxisLen[AXIS_X]; }
		float GetAxisLenY() { return mAxisLen[AXIS_Y]; }
		float GetAxisLenZ() { return mAxisLen[AXIS_Z]; }

	protected:

		float mAxisLen[AXIS_NUM];

	private:
	};
}
