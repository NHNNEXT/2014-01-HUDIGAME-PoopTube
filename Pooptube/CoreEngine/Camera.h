﻿/**
* @author 김지환
* @brief
* 프로젝션을 담당하는 카메라.
* 디폴트로 perspective projection을 한다.
* @date 2014/04/22
* @file Camera.h
*/
#pragma once
#include "Node.h"

namespace pooptube {
	class Camera : public Node {
	public:
		Camera();
		virtual ~Camera();

		static Camera *Create();

		virtual bool Init();

		virtual void Render();
		virtual void Update(float dTime);
		
		void		 SetLookAtPt(D3DXVECTOR3 vec) { mLookAtPt = vec;}
		D3DXVECTOR3	 GetLookAtPt() { return mLookAtPt; }

		float		 GetFarPlane() const { return mFarPlane; }
		void		 SetFarPlane(float val) { mFarPlane = val; }

	protected:


	private:
		D3DXVECTOR3		mLookAtPt;

		float			mNearPlane = 1.0f;
		float			mFarPlane = 300.0f;
	};
}