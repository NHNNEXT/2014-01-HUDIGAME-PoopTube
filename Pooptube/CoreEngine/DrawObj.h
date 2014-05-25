#pragma once
#include "Node.h"

namespace pooptube {

	//나무에 대해서만 적용될 수 있도록 만들어짐
	//todo : 애니메이션도 적용될 수 있도록
	//shader를 적용할 수 있도록
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