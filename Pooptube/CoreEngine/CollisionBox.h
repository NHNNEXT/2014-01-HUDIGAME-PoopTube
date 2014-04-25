#pragma once
#include "Node.h"
#include "Mesh.h"

namespace pooptube {

	class CollisionBox : public Node {
	public:
		CollisionBox();
		CollisionBox(const D3DXVECTOR3& fMax, const D3DXVECTOR3& fMin);
		virtual ~CollisionBox();

		static std::shared_ptr<CollisionBox> Create();
		static std::shared_ptr<CollisionBox> Create(MESH_CUSTOM_VERTEX* vertices);

		bool Init();

		virtual void Render();
		virtual void Update( float dTime );

		void SetCollisionBoxFromVertices();

		bool CollisionCheck( const CollisionBox* target );

		void SetAxisLen( float val0, float val1, float val2 ) {
			mAxisLen[0] = val0;
			mAxisLen[1] = val1;
			mAxisLen[2] = val2;
		}

	protected:

	private:
		float mAxisLen[3];			// 상자의 세 축의 길이 절반.
	};
}
