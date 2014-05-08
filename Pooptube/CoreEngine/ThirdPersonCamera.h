﻿/**
* @author 김지환
* @brief
* 3인칭 카메라
* @date 2014/04/22
* @file ThirdPersonCamera.h
*/
#pragma once
#include "Camera.h"

namespace pooptube {
	class ThirdPersonCamera : public Camera {
	public:
		ThirdPersonCamera();
		virtual ~ThirdPersonCamera();

		//@param Target 
		//@brief node에서 상속된 모든 객체에 사용가능
		//@return 생성한 3인칭 카메라 반환
		static ThirdPersonCamera *Create(Node *Target);

		virtual bool Init(Node *Target);
		virtual void Render();
		virtual void Update(float dTime);

	protected:

	private:
		Node *mTarget = nullptr;
	};
}