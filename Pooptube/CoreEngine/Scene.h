#pragma once

#include "Node.h"

namespace pooptube {
	class Ground;

	class Scene : public Node {
	public:
		Scene();
		virtual ~Scene();

		static Scene* Create();

		virtual bool Init();

		virtual void Render();
		virtual void Update( float dTime );

		Ground *GetGroundModule() const { return mGround; }

		void AddRenderZone( Node* srcNode, D3DXVECTOR3& centerPos, float radius )	{
			mRenderZone.push_back( std::make_tuple( srcNode, centerPos, radius ) );
		}
		void RemoveRenderZone( Node* targetNode );
// 		std::vector<SPHERE_ZONE>& GetRenderZone(){ return mLightZone; }
		bool CheckRenderZone( D3DXVECTOR3 pos, float radius );

	protected:
		std::vector<std::tuple<Node*, D3DXVECTOR3, float>>	mRenderZone;
		Ground				*mGround = nullptr;

	private :
// 		std::vector<Node *> mRenderList;
	};
}