#pragma once
#include "Node.h"

namespace pooptube {

	//������ ���ؼ��� ����� �� �ֵ��� �������
	//todo : �ִϸ��̼ǵ� ����� �� �ֵ���
	//shader�� ������ �� �ֵ���
	class DrawObj : public Node
	{
	public:
		DrawObj();
		virtual ~DrawObj();

		void Render();
		void Update(float dTime);

	private:





	};
}