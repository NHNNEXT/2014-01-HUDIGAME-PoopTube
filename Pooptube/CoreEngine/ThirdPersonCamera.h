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
		static ThirdPersonCamera* Create(Node* Target);

		virtual bool Init(Node* Target);
		virtual void Render();
		virtual void Update(float dTime);

	protected:

	private:
		Node *mTarget = nullptr;
	};
}