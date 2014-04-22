/**
* @author 양현찬
* @brief
* 프로젝션을 담당하는 카메라.
* 디폴트로 perspective projection을 한다.
* @date 2014/04/17
* @file SkinnedMesh.h
*/
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
		D3DXMATRIXA16 mMatView;
		D3DXMATRIXA16 mMatProj;

	private:

		
	};
}