
#pragma once

#include "Node.h"

namespace pooptube {
	class Camera : public Node {
	public:
		Camera();
		virtual ~Camera();

		static Camera* Create();

		virtual bool Init();

		virtual void Render();
		virtual void Update(float dTime);



	protected:

	private:

		//카메라의 위치
		D3DXVECTOR3 mEyePt;
		//카메라가 바라보는 방향
		D3DXVECTOR3 mLookatPt;
		//업벡터
		D3DXVECTOR3 mUpVec;

		D3DXMATRIXA16 mMatView;
		D3DXMATRIXA16 mMatProj;
	};
}