
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

		//ī�޶��� ��ġ
		D3DXVECTOR3 mEyePt;
		//ī�޶� �ٶ󺸴� ����
		D3DXVECTOR3 mLookatPt;
		//������
		D3DXVECTOR3 mUpVec;

		D3DXMATRIXA16 mMatView;
		D3DXMATRIXA16 mMatProj;
	};
}