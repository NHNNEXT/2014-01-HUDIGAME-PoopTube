
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

		void SetEye(D3DXVECTOR3 value) { mEyePt = value; }
		void SetLook(D3DXVECTOR3 value) { mLookatPt = value; }
		void SetUp(D3DXVECTOR3 value) { mUpVec = value; }

		//void Rotate(D3DXVECTOR3 dAngle);
		void MoveSide(FLOAT	fSpeed);
		void MoveForward(FLOAT	fSpeed, FLOAT fY = 0);



	protected:
		//카메라의 위치
		D3DXVECTOR3 mEyePt = {0.f, 3.f, 10.f};
		//카메라가 바라보는 방향
		D3DXVECTOR3 mLookatPt = {0.f, 0.f, 0.f};
		//업벡터
		D3DXVECTOR3 mUpVec = {0.f, 1.f, 0.f};

		D3DXMATRIXA16 mMatView;
		D3DXMATRIXA16 mMatProj;

	private:

		
	};
}