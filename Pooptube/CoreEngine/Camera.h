/**
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

		static std::shared_ptr<Camera> Create();

		virtual bool Init();

		virtual void Render();
		virtual void Update(float dTime);
		
		void		 SetLookAtPt(D3DXVECTOR3 vec) { mLookAtPt = vec;}
		D3DXVECTOR3	 GetLookAtPt() { return mLookAtPt; }

	protected:
		D3DXMATRIXA16	mMatView;
		D3DXMATRIXA16	mMatProj;

	private:
		D3DXVECTOR3		mLookAtPt;
		
	};
}