/**
* @author ������
* @brief
* 3��Ī ī�޶�
* @date 2014/04/22
* @file Camera.h
*/
#pragma once
#include "Camera.h"

namespace pooptube {
	class ThirdPersonCamera : public Camera {
	public:
		ThirdPersonCamera();
		virtual ~ThirdPersonCamera();

		//@param Target 
		//@brief node���� ��ӵ� ��� ��ü�� ��밡��
		//@return ������ 3��Ī ī�޶� ��ȯ
		static std::shared_ptr<ThirdPersonCamera> Create(std::shared_ptr<Node> Target);

		virtual bool Init(std::shared_ptr<Node> Target);
		virtual void Render();
		virtual void Update(float dTime);

	protected:

	private:
		std::shared_ptr<Node> mTarget = nullptr;
	};
}